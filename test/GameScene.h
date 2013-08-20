#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common_types.h"
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <map>
#include "Camera.h"

class CompiledMesh;
class IGraphicsContext;
class IResourceManager;
class IInput;
class StaticObject;
class PointLight;
class SpotLight;

class GameScene
{
public:

	static std::unique_ptr<GameScene> LoadFromFile(std::string const& name, IResourceManager& resourceManager);

	GameScene();
	~GameScene();

	void Init(IResourceManager& resourceManager);
	void Render(IGraphicsContext& graphicsContext);
	void Update(core::real timeDelta, IInput& input);
	void OnResize(core::ui_size size);

	void AddCamera(std::string const& name, std::unique_ptr<Camera> camera);
	void RemoveCamera(std::string const& name);

	void AddStaticObject(std::unique_ptr<StaticObject> obj);
	void AddDynamicObject(std::string const& name/*, std::unique_ptr<DynamicObject> obj*/);
	void AddPointLight(std::unique_ptr<PointLight> pointLight);
	void AddSpotLight(std::unique_ptr<SpotLight> spotLight);

	void RemoveStaticObject(std::string const& name);
	void RemoveDynamicObject(std::string const& name);

	//void AddParticleEmitter();
	//void RemoveParticleEmitter();

	Camera& GetActiveCamera();
	Camera const& GetCamera(std::string const&) const;
	void SetActiveCamera(std::string const& name);

private:
	// SceneGraph sceneGraph_;
	// static geometry
	// dynamic objects
	// etc
	std::vector<std::unique_ptr<StaticObject> > staticObjects_;
	std::vector<std::unique_ptr<PointLight> > pointLights_;
	std::vector<std::unique_ptr<SpotLight> > spotLights_;
	// Camera camera_;
	std::map<std::string, std::unique_ptr<Camera> > cameras_;
	std::string activeCameraTag_;
#ifdef _TEST
	core::real angle_;
#endif
};


#endif // GAMESCENE_H