#include "GameScene.h"
#include "Mesh.h"
#include "CompiledMesh.h"
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "IInput.h"
#include "StaticObject.h"
#include "ScnParser.h"
#include "utils.h"

#include <map>
#include <string>
#include <functional>
#include <memory>

using namespace core;
using namespace std;

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
    cameras_["first"] = unique_ptr<Camera>(new Camera());
    cameras_["first"]->SetFrustum(frustum(static_cast<real>(M_PI/3), 640.f/480.f, 0.1f, 100.f));
    cameras_["first"]->LookAt(vector3(0,0,0), vector3(0,0,1), vector3(0,1,0));
#endif
}

void GameScene::Render(IGraphicsContext& graphicsContext)
{

    graphicsContext.SetViewMatrix(GetActiveCamera().GetViewMatrix());
    graphicsContext.SetFrustum(GetActiveCamera().GetFrustum());

    for (auto cIter = staticObjects_.cbegin(); cIter != staticObjects_.cend(); ++cIter)
    {
        StaticObject& currentObject = *(*cIter);
        graphicsContext.SetWorldMatrix(currentObject.GetWorldMatrix());
        graphicsContext.DrawMesh(currentObject.GetCompiledMesh());
        // set transforms etc
#ifdef _TEST

#endif
    }
}

void GameScene::Update(real timeDelta, IInput& input)
{
#ifdef _TEST
    angle_ += 0.2f * timeDelta;

    real camRotY = 0.f;
    real camRotX = 0.f;

    if (input.IsMouseButtonPressed(MMB_LEFT))
    {
        const real MOUSE_SENSITIVITY = 0.005f;
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
        GetActiveCamera().MoveForward(timeDelta * 1.5f);
    }


#endif
}

Camera& GameScene::GetActiveCamera() const
{
    return *(cameras_.begin())->second;
}

unique_ptr<GameScene> GameScene::LoadFromFile(string const& name, IResourceManager& resourceManager)
{
    unique_ptr<GameScene> scene(new GameScene());

    map<string, shared_ptr<Mesh> > meshCache;

    ScnParser scnParser(name);

    scnParser.OnStaticObject = [&](string const& meshTag, string const& meshFile, matrix4x4 const& worldMatrix)
    {
        auto iter = meshCache.find(meshFile);

        if (iter == meshCache.end())
        {
            string meshFileName = meshFile;
            meshFileName.append(".objm");
            meshCache[meshFile] = Mesh::CreateFromObj(meshFileName);

            assert(meshCache[meshFile].use_count() == 1);
        }

        /// FIXME: Use weak ptr instead of Mesh& in IResourceManager
        scene->AddStaticObject(unique_ptr<StaticObject>(new StaticObject(resourceManager.CompileMesh(*meshCache[meshFile]), worldMatrix)));
    };

    scnParser.Parse();



    scene->Init(resourceManager);

    return scene;
}

void GameScene::AddStaticObject(unique_ptr<StaticObject> obj)
{
    staticObjects_.push_back(move(obj));
}
