#include "script.h"
#include "app.hpp"

static App gApp;

void ScriptMain() {
    gApp.Init();
    while (true) {
        gApp.Tick();
        WAIT(0);
    }
}

BOOL APIENTRY DllMain(HMODULE h, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        scriptRegister(h, ScriptMain);
        keyboardHandlerRegister([](DWORD key, WORD, BYTE, BOOL, BOOL, BOOL, BOOL isUpNow) {
            if (!isUpNow) gApp.OnKey((int)key);
            });
    }
    else if (reason == DLL_PROCESS_DETACH) {
        scriptUnregister(h);
    }
    return TRUE;
}