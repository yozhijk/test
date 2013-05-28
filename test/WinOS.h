/////////////////////////////////////////////////////
/// @file WinOS.h
///     Windows implementation of ISystem iface
///
/// @author Dmitry Kolzov
///
#ifndef WINOS_H
#define WINOS_H

#include "common_types.h"

#include "ISystem.h"
#include "Window.h"

#include <windows.h>
#include <vector>

class IGraphicsContext;

/////////////////////////////////////////////////////
/// WinOS
///     
///
class WinOS : public ISystem
{
public:
    WinOS();
    ~WinOS();

    /// ISystem overrides
    void AddListener(ISystemListener* systemListener);
    std::unique_ptr<IGraphicsContext> CreateGraphicsContext(Window const& window);
    std::unique_ptr<IInput> CreateInput();
    void Log(std::string const& message);

    /// Own methods
    void SetWindowParams(HWND hWnd, core::ui_rect const& rect);
    void Loop(core::real timeDelta);
    void ResizeWindow(core::ui_size const& size);
    void Shutdown();

private:
    std::vector<ISystemListener*> listeners_;
    Window window_;
};


#endif // WINOS_H