#include "MacOS.h"

#include "ISystemListener.h"
#include "IGraphicsContext.h"
#include "IInput.h"
#include "OGLContext.h"
#include "Mesh.h"

#include <memory>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace std;

MacOS::MacOS()
{
}

MacOS::~MacOS()
{
    
}

void MacOS::AddListener( ISystemListener* systemListener )
{
	listeners_.push_back(systemListener);
}

unique_ptr<IGraphicsContext> MacOS::CreateGraphicsContext(Window const& window)
{
	return unique_ptr<IGraphicsContext>(new OGLContext());
}

unique_ptr<IInput> MacOS::CreateInput()
{
    assert(false);
    return nullptr;
}

void MacOS::SetWindowParams(core::ui_rect const& rect)
{
	/// Set window parameters
	window_.handle = nullptr;
	window_.rect = rect;
    
	/// Fire an event
	for_each(listeners_.begin(), listeners_.end(), bind(&ISystemListener::OnStartup, placeholders::_1, cref(window_)));
}

void MacOS::Loop(core::real timeDelta)
{
	/// Fire an event
	for_each(listeners_.begin(), listeners_.end(), bind(&ISystemListener::OnUpdate, placeholders::_1, timeDelta));
}

void MacOS::ResizeWindow(core::ui_size const& size)
{
	/// Fire an event
	for_each(listeners_.begin(), listeners_.end(), bind(&ISystemListener::OnResizeWindow, placeholders::_1, size));
}

void MacOS::Log(std::string const& message)
{
    cout << message << "\n";
}