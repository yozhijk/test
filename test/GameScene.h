#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "common_types.h"
#include <vector>
//#include "Camera.h"

class CompiledMesh;
class IGraphicsContext;
class IResourceManager;

class GameScene
{
public:
    GameScene();
    ~GameScene();
    void Init(IResourceManager& resourceManager);
    void Render(IGraphicsContext& graphicsContext);
    void Update(core::real timeDelta);

private:
    // SceneGraph sceneGraph_;
    // static geometry
    // dynamic objects
    // etc
    std::vector<std::unique_ptr<CompiledMesh> > staticObjects_;
    //Camera camera_;
#ifdef _TEST
    core::real angle_;
#endif
};


#endif // GAMESCENE_H