#include "GameScene.h"
#include "Mesh.h"
#include "CompiledMesh.h"
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "utils.h"

using namespace core;

GameScene::GameScene()
{
}

void GameScene::Init(IResourceManager& resourceManager)
{
    // TEST CODE
#ifdef _TEST
    std::unique_ptr<Mesh> mesh = Mesh::CreateFromObj("C:\\temp\\cube.obj");
    staticObjects_.push_back(resourceManager.CompileMesh(*mesh));
#endif
}

void GameScene::Render(IGraphicsContext& graphicsContext)
{
    for (auto cIter = staticObjects_.cbegin(); cIter != staticObjects_.cend(); ++cIter)
    {
        // set transforms etc
#ifdef _TEST
        graphicsContext.SetWorldMatrix(translation_matrix(vector3(-0.5,-0.5,0)));
        graphicsContext.SetViewMatrix(lookat_matrix_lh_dx(vector3(-2,2,-5), vector3(0,0,0), vector3(0,1,0)));
#endif
        graphicsContext.DrawMesh(**cIter);
    }
}

void GameScene::Update(core::real timeDelta)
{

}