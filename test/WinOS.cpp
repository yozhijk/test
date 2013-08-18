#include "WinOS.h"

#include "ISystemListener.h"
#include "IGraphicsContext.h"
#include "IInput.h"
#include "DX11Context.h"
#include "DXInput.h"

#include <memory>
#include <functional>
#include <algorithm>

using namespace std;
using namespace core;

WinOS::WinOS()
{

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

unique_ptr<IInput> WinOS::CreateInput()
{
	return unique_ptr<IInput>(new DXInput(static_cast<HWND>(window_.handle)));
}

void WinOS::Log(string const& message)
{
	OutputDebugStringA(message.c_str());
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
	for_each(listeners_.begin(), listeners_.end(), bind(&(ISystemListener::OnResizeWindow), placeholders::_1, cref(size)));
}

void WinOS::Shutdown()
{
	/// Fire an event
	for_each(listeners_.begin(), listeners_.end(), bind(&(ISystemListener::OnShutdown), placeholders::_1));

	listeners_.clear();
}