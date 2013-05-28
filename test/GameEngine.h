#ifndef GAMEENGINE_H
#define GAMEENGINE_H

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
    void AddScene(std::unique_ptr<GameScene> scene);

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

    GameEngine(GameEngine const&);
    void operator = (GameEngine const&);

    GameScene*    currentScene_;
    STATE     engineState_;
    //std::unique_ptr<Physics> physics_;
    //script engine, etc
};

#endif