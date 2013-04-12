#include "WinOS.h"

#include "ISystemListener.h"
#include "IGraphicsContext.h"
#include "DX11Context.h"

#include <memory>
#include <functional>
#include <algorithm>

#include "Mesh.h"

using namespace std;
using namespace core;
	
WinOS::WinOS()
{
	//std::for_each(listeners_.begin(), listeners);
	unique_ptr<Mesh> modelPtr = Mesh::CreateFromObj("C:\\temp\\cube.obj");

}

WinOS::~WinOS()
{

}

void WinOS::AddListener( ISystemListener* systemListener )
{
	listeners_.push_back(systemListener);
}

unique_ptr<IGraphicsContext> WinOS::CreateGraphicsContext(Window const& window)
{
	return unique_ptr<IGraphicsContext>(new DX11Context(static_cast<HWND>(window_.handle)));
}

void WinOS::SetWindowParams(HWND hWnd, ui_rect const& rect)
{
	/// Set window parameters
	window_.handle = static_cast<void*>(hWnd);
	window_.rect = rect;

	/// Fire an event
	for_each(listeners_.begin(), listeners_.end(), bind(&(ISystemListener::OnStartup), placeholders::_1, cref(window_)));
}

void WinOS::Loop(real timeDelta)
{
	/// Fire an event
	for_each(listeners_.begin(), listeners_.end(), bind(&(ISystemListener::OnUpdate), placeholders::_1, timeDelta));
}

void WinOS::ResizeWindow(ui_size const& size)
{
	/// Fire an event
	for_each(listeners_.begin(), listeners_.end(), bind(&(ISystemListener::OnResizeWindow), placeholders::_1, size));
}