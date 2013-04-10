#include "WinOS.h"

#include "ISystemListener.h"
#include "IGraphicsContext.h"
#include "DX11Context.h"

#include <memory>
#include <functional>
#include <algorithm>
	
WinOS::WinOS()
{
	//std::for_each(listeners_.begin(), listeners);

}

WinOS::~WinOS()
{

}

void WinOS::AddListener( ISystemListener* systemListener )
{
	listeners_.push_back(systemListener);
}

std::unique_ptr<IGraphicsContext> WinOS::CreateGraphicsContext(Window const& window)
{
	return std::unique_ptr<IGraphicsContext>(new DX11Context(static_cast<HWND>(window_.handle)));
}

void WinOS::SetWindowParams(HWND hWnd, core::ui_rect const& rect)
{
	/// Set window parameters
	window_.handle = static_cast<void*>(hWnd);
	window_.rect = rect;

	/// Fire an event
	std::for_each(listeners_.begin(), listeners_.end(), std::bind(&(ISystemListener::OnStartup), std::placeholders::_1, std::cref(window_)));
}

void WinOS::Loop(core::real timeDelta)
{
	/// Fire an event
	std::for_each(listeners_.begin(), listeners_.end(), std::bind(&(ISystemListener::OnUpdate), std::placeholders::_1, timeDelta));
}

void WinOS::ResizeWindow(core::ui_size const& size)
{
	/// Fire an event
	std::for_each(listeners_.begin(), listeners_.end(), std::bind(&(ISystemListener::OnResizeWindow), std::placeholders::_1, size));
}