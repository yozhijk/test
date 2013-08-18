/////////////////////////////////////////////////////
/// @file ISystemListener.h
///	 Callbacks from the operating system
///
/// @author Dmitry Kolzov
///
#ifndef ISYSTEM_LISTENER_H
#define ISYSTEM_LISTENER_H

#include "common_types.h"

struct Window;

/////////////////////////////////////////////////////
/// A set of callbacks to maintain the application's 
/// lifecycle: handle input events, timings, etc
///
class ISystemListener
{
public:
	virtual ~ISystemListener() = 0;

	/// Gets called on application startup 
	/// ??? Decide whether this should be called on any new subscriber ???
	virtual void OnStartup(Window const& window) = 0;
	/// Gets called on application shutdown
	virtual void OnShutdown() = 0;
	/// Gets called every run-loop iteration
	virtual void OnUpdate(core::real timeDelta) = 0;
	/// Gets called on window resize event
	virtual void OnResizeWindow(core::ui_size const& size) = 0;
};

inline ISystemListener::~ISystemListener(){}


#endif //ISYSTEM_LISTENER