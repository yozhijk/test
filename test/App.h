/////////////////////////////////////////////////////
/// @file App.h
///	    Platform independent application
///
/// @author Dmitry Kolzov
///
#ifndef APP_H
#define APP_H

#include "ISystemListener.h"
#include <memory>

class ISystem;
class IGraphicsContext;
class GameEngine;

/////////////////////////////////////////////////////
/// 
///	 Represents an app as a set of callback functions
///  responding to different OS events, ISystem iface
///  should be provided externally
///
class App : private ISystemListener
{
public:
	App(ISystem& os);
	~App();

	/// ISystemListener overrides
	void OnStartup(Window const& window);
	void OnShutdown();
	void OnUpdate(core::real timeDelta);
	void OnResizeWindow(core::ui_size const& size);

private:
	App(App const&);// = delete;
	App& operator = (App const&);// = delete;

	ISystem& os_;
	std::unique_ptr<GameEngine>       gameEngine_; 
	std::unique_ptr<IGraphicsContext> graphicsContext_;
};


#endif // APP_H