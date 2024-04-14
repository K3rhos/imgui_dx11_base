#include <pch.h>



BOOL APIENTRY DllMain(HMODULE _module, DWORD _reason_for_call, LPVOID _reserved)
{
    switch (_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            client::app::get()->initialize(_module);
        }
        break;
        case DLL_PROCESS_DETACH:
        {
            client::app::get()->shutdown();
        }
        break;
    }

    return TRUE;
}