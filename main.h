#pragma once

// #define _DEBUG_MODE

#include <windows.h>
#include <cstdint>
#include <map>

#include "hooks/hooks.h"

class c_entry
{
	FILE *file {nullptr};
	void *timer_ptr {reinterpret_cast<void *>(0x561B10U)};
public:
	c_entry();
	~c_entry();
} entry;