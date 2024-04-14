#ifndef PCH_H
#define PCH_H



// Windows/C headers.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <map>
#include <dxgi.h>
#include <d3d11.h>
#include <functional>



// Included resources headers.
#include <MinHook/MinHook.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>



// Client app headers.
#include "source/headers/defines.h"
#include "source/headers/singleton.h"
#include "source/headers/events_handler.h"
#include "source/headers/utils.h"

#include "source/log/log.h"

#include "source/hooking/minhook.h"

#include "source/directx_hook/directx_hook.h"
#include "source/inputs_hook/inputs_hook.h"

#include "source/imgui/windows/window.h"

#include "source/imgui/imgui.h"

#include "source/app/app.h"



#endif