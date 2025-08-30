#pragma once

class PlayerMod {
    static bool godMode;
    static bool neverWanted;
    static bool noRagdoll;
    static bool superJump;
    static bool fastRun;

public:
    static void ToggleGodMode();
    static void ToggleNeverWanted();
    static void ToggleNoRagdoll();
    static void ToggleSuperJump();
    static void ToggleFastRun();

    static void HealFull();
    static void ClearWanted();

    static void Tick();

    static bool* GodModePtr();
    static bool* NeverWantedPtr();
    static bool* NoRagdollPtr();
    static bool* SuperJumpPtr();
    static bool* FastRunPtr();

    static bool  IsGodMode();
    static bool  IsNeverWanted();
    static bool  IsNoRagdoll();
    static bool  IsSuperJump();
    static bool  IsFastRun();

    static void  SetGodMode(bool v);
    static void  SetNeverWanted(bool v);
    static void  SetNoRagdoll(bool v);
    static void  SetSuperJump(bool v);
    static void  SetFastRun(bool v);
};
