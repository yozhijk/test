/////////////////////////////////////////////////////
/// @file MacOS.h
///	    Mac OS X implementation of ISystem iface
///
/// @author Dmitry Kolzov
///
#ifndef MACOS_H
#define MACOS_H

#include "common_types.h"
#include "ISystem.h"
#include "Window.h"

#include <vector>

struct Window;

class MacOS : public ISystem
{
public:
    MacOS();
	~MacOS();
    
	/// ISystem overrides
	void AddListener(ISystemListener* systemListener);
	std::unique_ptr<IGraphicsContext> CreateGraphicsContext(Window const& window);
    std::unique_ptr<IInput> CreateInput();
    
	/// Own methods
	void SetWindowParams(core::ui_rect const& rect);
	void Loop(core::real timeDelta);
	void ResizeWindow(core::ui_size const& size);
    
private:
	std::vector<ISystemListener*> listeners_;
	Window window_;
};



#endif // MACOS_H
