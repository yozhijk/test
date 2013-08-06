#include "App.h"
#include "Window.h"
#include "ISystem.h"
#include "IInput.h"
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
    /// Subtle bug here, game engine could be deleted after the graphics context
    /// and model callbacks might cause a crash
    /// probably better to use shared_ptr for graphics context
    /// workaround for now
    gameEngine_.reset();
}

/// ISystemListener overrides
void App::OnStartup(Window const& window)
{
    graphicsContext_ = os_.CreateGraphicsContext(window);
    graphicsContext_->Init();

    input_ = os_.CreateInput();
    // Configure input

    gameEngine_.reset(new GameEngine());
    gameEngine_->Init(graphicsContext_->GetResourceManager());

    gameEngine_->AddScene(GameScene::LoadFromFile("many_monkeys_scene.scn", graphicsContext_->GetResourceManager()));
    gameEngine_->SetActiveScene(0);

    OnResizeWindow(core::ui_size(window.rect.w, window.rect.h));
}

void App::OnShutdown()
{
    gameEngine_->Shutdown();
}

void App::OnUpdate(core::real timeDelta)
{
    /// Get input
    input_->UpdateState();
    /// Advance objects
    gameEngine_->Update(timeDelta, *input_);
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

    gameEngine_->OnResize(size);
}