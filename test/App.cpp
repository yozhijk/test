#include "App.h"
#include "Window.h"
#include "ISystem.h"
#include "IGraphicsContext.h"
#include "GameEngine.h"


App::App(ISystem& os) 
	: os_(os)
{
	os_.AddListener(this);
}

App::~App()
{
}

/// ISystemListener overrides
void App::OnStartup(Window const& window)
{
	graphicsContext_ = os_.CreateGraphicsContext(window);
	graphicsContext_->Init();

	gameEngine_.reset(new GameEngine());
	gameEngine_->Init(graphicsContext_->GetResourceManager());
}

void App::OnShutdown()
{
	gameEngine_->Shutdown();
}

void App::OnUpdate(core::real timeDelta)
{
	/// Advance objects
	gameEngine_->Update(timeDelta);
	/// Draw objects
	gameEngine_->RenderScene(*graphicsContext_);
	/// Present rendered stuff
	graphicsContext_->Present();
}

void App::OnResizeWindow(core::ui_size const& size)
{
	graphicsContext_->ResizeBuffer(size);
}