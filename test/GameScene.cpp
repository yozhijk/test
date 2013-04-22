#include "GameScene.h"
#include "Mesh.h"
#include "CompiledMesh.h"
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "utils.h"

using namespace core;

GameScene::GameScene()
#ifdef _TEST
    : angle_(0)
#endif
{
}

void GameScene::Init(IResourceManager& resourceManager)
{
    // TEST CODE
#ifdef _TEST
    std::unique_ptr<Mesh> mesh = Mesh::CreateFromObj("monkey.objm");
    staticObjects_.push_back(resourceManager.CompileMesh(*mesh));
#endif
}

void GameScene::Render(IGraphicsContext& graphicsContext)
{
    for (auto cIter = staticObjects_.cbegin(); cIter != staticObjects_.cend(); ++cIter)
    {
        // set transforms etc
#ifdef _TEST
        graphicsContext.SetWorldMatrix(rotation_matrix_y(angle_) * translation_matrix(vector3(-0.5, -0.5, -0.5)));
        graphicsContext.SetViewMatrix(lookat_matrix_lh_dx(vector3(0,2,-5), vector3(0,0,0), vector3(0,1,0)));
#endif
        graphicsContext.DrawMesh(**cIter);
    }
}

void GameScene::Update(core::real timeDelta)
{
#ifdef _TEST
    angle_ += timeDelta * 0.2; // 0.2 radians per second
#endif
}