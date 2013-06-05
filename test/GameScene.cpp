#include "GameScene.h"
#include "Mesh.h"
#include "CompiledMesh.h"
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "IInput.h"
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
    cameras_["first"]->LookAt(vector3(0,0,0), vector3(0,0,1), vector3(0,1,0));
#endif
}

void GameScene::Render(IGraphicsContext& graphicsContext)
{

    graphicsContext.SetViewMatrix(GetActiveCamera().GetViewMatrix());
    graphicsContext.SetFrustum(GetActiveCamera().GetFrustum());

    for (auto cIter = staticObjects_.cbegin(); cIter != staticObjects_.cend(); ++cIter)
    {
        // set transforms etc
#ifdef _TEST
        //cameras_["first"]->RotateCamera(vector3(0,1,0), 0.00125);
        //cameras_["first"]->RotateCamera(vector3(1,0,0), 0.00125);
        for (int i=0;i<10;++i)
            for (int j=0;j<10;++j)
                for (int k=0;k<10;++k)
            {
                graphicsContext.SetWorldMatrix( translation_matrix(vector3(-20 + i*3, -20 + j*3, -20 + k*3)) * rotation_matrix_y(angle_) );
                graphicsContext.DrawMesh(**cIter);
            }
#endif
    }
}

void GameScene::Update(core::real timeDelta, IInput& input)
{
#ifdef _TEST
    angle_ += 0.2 * timeDelta;

    real camRotY = 0.f;
    real camRotX = 0.f;

    if (input.IsMouseButtonPressed(MMB_LEFT))
    {
        const real MOUSE_SENSITIVITY = 0.005;
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
        GetActiveCamera().MoveForward(timeDelta * 1.5);
    }


#endif
}

Camera& GameScene::GetActiveCamera() const
{
    return *(cameras_.begin())->second;
}

std::unique_ptr<GameScene> LoadFromFile(std::string const& name)
{
    assert(false);
    return nullptr;
}