#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include <memory>

#include "common_types.h"

class IGraphicsContext;
class IResourceManager;
class IInput;
class GameScene;

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Init (IResourceManager& resourceManager);
	void Shutdown();
	void Update(core::real timeDelta, IInput& input);
	void RenderScene(IGraphicsContext& graphicsContext);
	void SetActiveScene(core::uint sceneIndex);
	void AddScene(std::unique_ptr<GameScene> scene);

	void OnResize(core::ui_size size);

	//protected:
	//typedef std::function<void(void*)> SCENE_CALLBACK;
	//void NextScene(SCENE_CALLBACK callback);

private:
	enum STATE
	{
		STATE_RUNNING = 0x1,
		STATE_TRANSITIONING,
		STATE_PAUSED
	};

	static const core::uint SCENE_INDEX_MAX = 0xffffffff;

	GameEngine(GameEngine const&);
	void operator = (GameEngine const&);

	core::uint	currentSceneIndex_;
	STATE	 engineState_;
	std::vector< std::unique_ptr<GameScene> > scenes_;
};

#endif