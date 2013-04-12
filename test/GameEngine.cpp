#include "GameEngine.h"
#include "IGraphicsContext.h"
#include "IResourceManager.h"
#include "GameScene.h"

#include <cassert>

GameEngine::GameEngine()
	: currentScene_(nullptr)
	, engineState_(STATE_RUNNING)
{

}

void GameEngine::Init(IResourceManager& resourceManager/*GameConfig const&*/)
{
	/// Initialization code here
}

void GameEngine::Shutdown()
{
	/// Engine shutdown code here
}

void GameEngine::Update(core::real timeDelta)
{
	assert(currentScene_);

	/// State handling and scene graph update code here
	if (STATE_RUNNING == engineState_)
	{
		currentScene_->Update(timeDelta);
	}
}

void GameEngine::RenderScene(IGraphicsContext& graphicsContext)
{
	// assert(currentScene_);
	/// State handling and rendering code here
	if (STATE_RUNNING == engineState_)
	{
		graphicsContext.Clear(core::color_rgba(0.0, 1.0, 0.0, 1.0));
		
	    currentScene_->Render(graphicsContext);
	}
	else if (STATE_TRANSITIONING == engineState_)
	{
		/// Show transitioning screen etc
	}
}