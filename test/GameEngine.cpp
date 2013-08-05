#include "GameEngine.h"
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "IInput.h"
#include "GameScene.h"

#include <cassert>

using namespace core;

GameEngine::GameEngine()
    : currentSceneIndex_(SCENE_INDEX_MAX)
    , engineState_(STATE_RUNNING)
{

}

GameEngine::~GameEngine()
{
}

void GameEngine::Init(IResourceManager& resourceManager/*GameConfig const&*/)
{
    /// Initialization code here
    /// TEST CODE
}

void GameEngine::Shutdown()
{
    /// Engine shutdown code here
}

void GameEngine::Update(core::real timeDelta, IInput& input)
{
    assert(currentSceneIndex_ < SCENE_INDEX_MAX);
    /// State handling and scene graph update code here
    if (STATE_RUNNING == engineState_)
    {
        scenes_[currentSceneIndex_]->Update(timeDelta, input);
    }
}

void GameEngine::RenderScene(IGraphicsContext& graphicsContext)
{
    assert(currentSceneIndex_ < SCENE_INDEX_MAX);
    /// State handling and rendering code here
    if (STATE_RUNNING == engineState_)
    {
        graphicsContext.Clear(core::color_rgba(0.0, 0.0, 0.0, 1.0));
        scenes_[currentSceneIndex_]->Render(graphicsContext);
    }
    else if (STATE_TRANSITIONING == engineState_)
    {
        /// Show transitioning screen etc
    }
}

void GameEngine::OnResize(core::ui_size size)
{
    if (currentSceneIndex_ < SCENE_INDEX_MAX)
    {
        scenes_[currentSceneIndex_]->GetActiveCamera().SetFrustum(frustum(M_PI/3, (real)size.w/size.h, 0.1f, 100.f));
    }
}

void GameEngine::AddScene(std::unique_ptr<GameScene> scene)
{
    scenes_.push_back(std::move(scene));
}

void GameEngine::SetActiveScene(core::uint sceneIndex)
{
    assert(sceneIndex < scenes_.size());
    currentSceneIndex_ = sceneIndex;
}