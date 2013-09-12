#include "GameScene.h"

#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "IInput.h"
#include "ICamera.h"

#include "Mesh.h"
#include "CompiledMesh.h"
#include "StaticObject.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ScnParser.h"

/// Scene shouldn't know about the concrete camera class
/// this should be moved into the factory
#include "QuaternionCamera.h"
#include "utils.h"

#include <map>
#include <string>
#include <functional>
#include <memory>

using namespace core;
using namespace std;

GameScene::GameScene()
#ifdef _TEST
	: angle_(0)
	, activeCameraTag_("")
#endif
{
}

GameScene::~GameScene()
{
}

void GameScene::Init(IResourceManager& resourceManager)
{
}

void GameScene::Render(IGraphicsContext& graphicsContext)
{
    graphicsContext.Clear(color_rgba(0,0,0,0));
    
	graphicsContext.SetViewMatrix(GetActiveCamera().GetViewMatrix());
	graphicsContext.SetFrustum(GetActiveCamera().GetFrustum());

	/// Enable no more that max point lights 
	/// Shadow casting is unsupported yet
	for (core::uint i = 0; i < min(static_cast<size_t>(IGraphicsContext::POINT_LIGHT_MAX), pointLights_.size()); ++i)
	{
		IGraphicsContext::PointLightIndex index = static_cast<IGraphicsContext::PointLightIndex>(i);
		graphicsContext.SetPointLight(index, *pointLights_[i]);
		graphicsContext.SetPointLightEnabled(index, true);
	}

	for (core::uint i = 0; i < min(static_cast<size_t>(IGraphicsContext::SPOT_LIGHT_MAX), spotLights_.size()); ++i)
	{
		IGraphicsContext::SpotLightIndex index = static_cast<IGraphicsContext::SpotLightIndex>(i);
		graphicsContext.SetSpotLight(index, *spotLights_[i]);
		graphicsContext.SetSpotLightEnabled(index, true);
	}

	/// Commit per-frame states
	graphicsContext.CommitState();

	/// Render static geometry
	for (auto cIter = staticObjects_.cbegin(); cIter != staticObjects_.cend(); ++cIter)
	{
		StaticObject& currentObject = *(*cIter);
		graphicsContext.SetWorldMatrix(currentObject.GetWorldMatrix());
		graphicsContext.DrawMesh(currentObject.GetCompiledMesh());
	}
}

void GameScene::Update(real timeDelta, IInput& input)
{
#ifdef _TEST
	angle_ += 0.2f * timeDelta;

	real camRotY = 0.f;
	real camRotX = 0.f;

	if (input.IsMouseButtonPressed(MMB_LEFT))
	{
		const real MOUSE_SENSITIVITY = 0.005f;
		vector2 delta = input.GetRelativePosition() * vector2(MOUSE_SENSITIVITY, MOUSE_SENSITIVITY);
		camRotX = -delta.y();
		camRotY = -delta.x();
	}

	if (camRotY != 0.f)
		GetActiveCamera().Rotate(camRotY);

	if (camRotX != 0.f)
		GetActiveCamera().Tilt(camRotX);

	if (input.IsKeyPressed(MK_UP))
	{
		GetActiveCamera().MoveForward(timeDelta * 1.5f);
	}

	if (input.IsKeyPressed(MK_DOWN))
	{
		GetActiveCamera().MoveForward(-timeDelta * 1.5f);
	}



#endif
}

ICamera& GameScene::GetActiveCamera()
{
	assert(activeCameraTag_ != "");

	auto iter = cameras_.find(activeCameraTag_);
	if (iter != cameras_.end())
	{
		return *((*iter).second);
	}
	else
	{
		throw std::runtime_error("No camera named " + activeCameraTag_);
	}
}

unique_ptr<GameScene> GameScene::LoadFromFile(string const& name, IResourceManager& resourceManager)
{
	unique_ptr<GameScene> scene(new GameScene());

	map<string, shared_ptr<Mesh> > meshCache;

	ScnParser scnParser(name);

	// FIXME: this creation requests should be routed to global factory finally
	scnParser.OnStaticObject = [&](StaticObjectDesc const& staticObjectDesc)
	{
		auto iter = meshCache.find(staticObjectDesc.fileName);

		if (iter == meshCache.end())
		{
			string meshFileName = staticObjectDesc.fileName;
			meshFileName.append(".objm");
			meshCache[staticObjectDesc.fileName] = Mesh::CreateFromObj(meshFileName);

			assert(meshCache[staticObjectDesc.fileName].use_count() == 1);
		}

		scene->AddStaticObject(unique_ptr<StaticObject>(new StaticObject(resourceManager.CompileMesh(*meshCache[staticObjectDesc.fileName]), staticObjectDesc.worldMatrix)));
	};

	scnParser.OnCamera = [&](CameraDesc const& cameraDesc)
	{
		unique_ptr<QuaternionCamera> camera(new QuaternionCamera());

		camera->SetFrustum(cameraDesc.frustum);
		camera->LookAt(cameraDesc.pos, cameraDesc.at, cameraDesc.up);

		scene->AddCamera(cameraDesc.tag, move(camera));

		if (cameraDesc.bActive)
		{
			scene->SetActiveCamera(cameraDesc.tag);
		}
	};

	scnParser.OnPointLight = [&](PointLightDesc const& pointLightDesc)
	{
		unique_ptr<PointLight> light(new PointLight());

		light->SetPosition(pointLightDesc.pos);
		light->SetColor(pointLightDesc.color);
		light->SetCastShadow(pointLightDesc.bCastShadow);

		scene->AddPointLight(std::move(light));
	};

	scnParser.OnSpotLight = [&](SpotLightDesc const& spotLightDesc)
	{
		unique_ptr<SpotLight> spotLight(new SpotLight());

		spotLight->SetPosition(spotLightDesc.pos);
		spotLight->SetDirection(spotLightDesc.dir);
		spotLight->SetColor(spotLightDesc.color);
		spotLight->SetAngles(spotLightDesc.innerAngle, spotLightDesc.outerAngle);
		spotLight->SetCastShadow(spotLightDesc.bCastShadow);

		scene->AddSpotLight(move(spotLight));
	};

	scnParser.Parse();

	scene->Init(resourceManager);

	return scene;
}

void GameScene::AddStaticObject(unique_ptr<StaticObject> obj)
{
	staticObjects_.push_back(move(obj));
}

void GameScene::AddPointLight(std::unique_ptr<PointLight> pointLight)
{
	pointLights_.push_back(move(pointLight));
}

void GameScene::AddSpotLight(std::unique_ptr<SpotLight> spotLight)
{
	spotLights_.push_back(move(spotLight));
}

void GameScene::OnResize(core::ui_size size)
{
	if (activeCameraTag_ != "")
		GetActiveCamera().SetAspectRatio(static_cast<real>(size.w)/size.h);
}

void GameScene::AddCamera(std::string const& name, std::unique_ptr<ICamera> camera)
{
	if (cameras_.find(name) != cameras_.end())
	{
		throw std::runtime_error("Duplicate camera");
	}
		
	cameras_[name] = std::move(camera);
}

void GameScene::RemoveCamera(std::string const& name)
{
	cameras_.erase(name);
}

void GameScene::SetActiveCamera(std::string const& name)
{
	if (cameras_.find(name) == cameras_.end())
	{
		throw std::runtime_error("Camera " + name + "not found in a scene");
	}

	activeCameraTag_ = name;
}