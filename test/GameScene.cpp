#include "GameScene.h"
#include "CompiledMesh.h"
#include "IGraphicsContext.h"

void GameScene::Render(IGraphicsContext& graphicsContext)
{
    for (auto cIter = staticObjects_.cbegin(); cIter != staticObjects_.cend(); ++cIter)
    {
        // set transforms etc
        graphicsContext.DrawMesh(*cIter);
    }
}

void GameScene::Update(core::real timeDelta)
{

}