#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "common_types.h"

class IGraphicsContext;
class IResourceManager;
class GameScene;

class GameEngine
{
public:
    GameEngine();
    void Init (IResourceManager& resourceManager/*GameConfig const&*/);
    void Shutdown();
    void Update(core::real timeDelta);
    void RenderScene(IGraphicsContext& graphicsContext);

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

    GameScene*    currentScene_;
    STATE     engineState_;

    //std::unique_ptr<Physics> physics_;
};

#endif