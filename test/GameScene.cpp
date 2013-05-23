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

GameScene::~GameScene()
{
}

void GameScene::Init(IResourceManager& resourceManager)
{
    // TEST CODE
#ifdef _TEST
    std::unique_ptr<Mesh> mesh = Mesh::CreateFromObj("monkey.objm");
    staticObjects_.push_back(resourceManager.CompileMesh(*mesh));

    cameras_["first"] = std::unique_ptr<Camera>(new Camera());
    cameras_["first"]->SetFrustum(core::frustum(M_PI/3, 640.f/480.f, 0.1f, 100.f));
    cameras_["first"]->LookAt(vector3(0,0,-5), vector3(0,0,0), vector3(0,1,0));
#endif
}

void GameScene::Render(IGraphicsContext& graphicsContext)
{
    for (auto cIter = staticObjects_.cbegin(); cIter != staticObjects_.cend(); ++cIter)
    {
        // set transforms etc
#ifdef _TEST
        cameras_["first"]->RotateCamera(vector3(0,1,0), 0.00125);
        cameras_["first"]->RotateCamera(vector3(1,0,0), 0.00125);
        graphicsContext.SetWorldMatrix( rotation_matrix_y(angle_) * translation_matrix(vector3(-0.5, -0.5, -0.5)));
        graphicsContext.SetViewMatrix(GetActiveCamera().GetViewMatrix());
        graphicsContext.SetFrustum(GetActiveCamera().GetFrustum());
#endif
        graphicsContext.DrawMesh(**cIter);
    }
}

void GameScene::Update(core::real timeDelta)
{
#ifdef _TEST
    angle_ += timeDelta * 0.2f; // 0.2 radians per second
#endif
}

Camera const& GameScene::GetActiveCamera() const
{
    return *(cameras_.begin())->second;
}