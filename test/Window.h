#ifndef WINDOW_H
#define WINDOW_H

#include "common_types.h"

struct Window
{
	core::ui_rect rect;
	void* handle;
};


#endif //WINDOW_H