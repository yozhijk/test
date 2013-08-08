#include "GameScene.h"
#include "Mesh.h"
#include "CompiledMesh.h"
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "IInput.h"
#include "StaticObject.h"
#include "ScnParser.h"
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

    graphicsContext.SetViewMatrix(GetActiveCamera().GetViewMatrix());
    graphicsContext.SetFrustum(GetActiveCamera().GetFrustum());

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

Camera& GameScene::GetActiveCamera()
{
    assert(cameras_.find(activeCameraTag_) != cameras_.end());
    return *(cameras_[activeCameraTag_]);
}

unique_ptr<GameScene> GameScene::LoadFromFile(string const& name, IResourceManager& resourceManager)
{
    unique_ptr<GameScene> scene(new GameScene());

    map<string, shared_ptr<Mesh> > meshCache;

    ScnParser scnParser(name);

    scnParser.OnStaticObject = [&](string const& meshTag, string const& meshFile, matrix4x4 const& worldMatrix)
    {
        auto iter = meshCache.find(meshFile);

        if (iter == meshCache.end())
        {
            string meshFileName = meshFile;
            meshFileName.append(".objm");
            meshCache[meshFile] = Mesh::CreateFromObj(meshFileName);

            assert(meshCache[meshFile].use_count() == 1);
        }

        /// FIXME: Use weak ptr instead of Mesh& in IResourceManager
        scene->AddStaticObject(unique_ptr<StaticObject>(new StaticObject(resourceManager.CompileMesh(*meshCache[meshFile]), worldMatrix)));
    };

    scnParser.OnCamera = [&](string const& tag, vector3 const& pos, vector3 const& at, vector3 const& up, frustum const& frustum, bool bActive)
    {
        unique_ptr<Camera> camera(new Camera());

        camera->SetFrustum(frustum);
        camera->LookAt(pos, at, up);

        scene->AddCamera(tag, move(camera));

        if (bActive)
        {
            scene->SetActiveCamera(tag);
        }
    };

    scnParser.Parse();



    scene->Init(resourceManager);

    return scene;
}

void GameScene::AddStaticObject(unique_ptr<StaticObject> obj)
{
    staticObjects_.push_back(move(obj));
}

void GameScene::OnResize(core::ui_size size)
{
    GetActiveCamera().SetAspectRatio(static_cast<real>(size.w)/size.h);
}

void GameScene::AddCamera(std::string const& name, std::unique_ptr<Camera> camera)
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