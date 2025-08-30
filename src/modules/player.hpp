#pragma once

class PlayerMod {
    static bool godMode;
public:
    static void ToggleGodMode();
    static void Tick();
    static bool* GodModePtr();
    static bool  IsGodMode();
    static void  SetGodMode(bool v);
};
