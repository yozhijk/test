#include "App.h"
#include "Window.h"
#include "ISystem.h"
#include "IGraphicsContext.h"
#include "GameEngine.h"
#include "GameScene.h"
#include "utils.h"


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

    //gameEngine_->AddScene(GameScene::LoadFromFile("Scene1"));
    //gameEngine_->SetActiveScene("Scene1");

    OnResizeWindow(core::ui_size(window.rect.w, window.rect.h));
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
    if (graphicsContext_)
    {
        /// this should be removed
        graphicsContext_->ResizeBuffer(size);
        graphicsContext_->SetViewport(core::ui_rect(0, 0, size.w, size.h));
    }
}