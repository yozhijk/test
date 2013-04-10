#include "MacOS.h"

#include "ISystemListener.h"
#include "IGraphicsContext.h"
#include "OGLContext.h"

#include <memory>
#include <functional>
#include <algorithm>

MacOS::MacOS()
{
	//std::for_each(listeners_.begin(), listeners);
    
}

MacOS::~MacOS()
{
    
}

void MacOS::AddListener( ISystemListener* systemListener )
{
	listeners_.push_back(systemListener);
}

std::unique_ptr<IGraphicsContext> MacOS::CreateGraphicsContext(Window const& window)
{
	return std::unique_ptr<IGraphicsContext>(new OGLContext());
}

void MacOS::SetWindowParams(core::ui_rect const& rect)
{
	/// Set window parameters
	window_.handle = nullptr;
	window_.rect = rect;
    
	/// Fire an event
	std::for_each(listeners_.begin(), listeners_.end(), std::bind(&ISystemListener::OnStartup, std::placeholders::_1, std::cref(window_)));
}

void MacOS::Loop(core::real timeDelta)
{
	/// Fire an event
	std::for_each(listeners_.begin(), listeners_.end(), std::bind(&ISystemListener::OnUpdate, std::placeholders::_1, timeDelta));
}

void MacOS::ResizeWindow(core::ui_size const& size)
{
	/// Fire an event
	std::for_each(listeners_.begin(), listeners_.end(), std::bind(&ISystemListener::OnResizeWindow, std::placeholders::_1, size));
}