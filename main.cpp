
#define _CRT_SECURE_NO_WARNINGS

#include "main.h"

c_entry::c_entry()
{
#if defined(_DEBUG_MODE)
    AllocConsole();
    freopen_s(&file, "CONOUT$", "w", stdout);
#endif

    g_options.load(true);

    MH_Initialize();

    MH_CreateHook(timer_ptr, &hook::timer_update, 
        reinterpret_cast<void **>(&original::timer_update));
    MH_EnableHook(timer_ptr);
}

c_entry::~c_entry()
{
    g_options.save();

    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();

#if defined(_DEBUG_MODE)
    fclose(file);
    FreeConsole();
#endif
}