#pragma once

#include <minhook/MinHook.h>

#include "../render/d3drender.h"
#include "../nametags/nametags.h"
#include "../other/call.hpp"

namespace prototype
{
	using present_t = HRESULT(__stdcall *)(IDirect3DDevice9 *, const RECT *, const RECT *, HWND, const RGNDATA *);
	using reset_t = HRESULT(__stdcall *)(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
	using void_t = void(__cdecl *)();
}

namespace original
{
	inline prototype::present_t device_present;
	inline prototype::reset_t device_reset;
	inline prototype::void_t timer_update;
	inline prototype::void_t tags_draw;
	inline prototype::void_t bars_draw;
	inline prototype::void_t bubbles_draw;
}

namespace hook
{
	void timer_update();
	void tags_draw();
	void bars_draw();
	void bubbles_draw();
	HRESULT __stdcall device_reset(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
	HRESULT __stdcall device_present(IDirect3DDevice9 *, const RECT *, const RECT *, HWND , const RGNDATA *);
}