#include "App.h"
#include "Window.h"
#include "ISystem.h"
#include "IGraphicsContext.h"
#include "GameEngine.h"
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
#ifdef _TEST
        core::matrix4x4 m = core::perspective_proj_fovy_matrix_lh_dx(M_PI/4, (core::real)size.w/size.h, 0.1f, 1000.f);
        graphicsContext_->SetProjectionMatrix(m);
#endif
        graphicsContext_->ResizeBuffer(size);
    }
}