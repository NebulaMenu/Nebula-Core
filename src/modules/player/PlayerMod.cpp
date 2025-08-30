#include "PlayerMod.hpp"
#include "script.h"

bool PlayerMod::godMode = false;
bool PlayerMod::neverWanted = false;
bool PlayerMod::noRagdoll = false;
bool PlayerMod::superJump = false;
bool PlayerMod::fastRun = false;

void PlayerMod::ToggleGodMode() { godMode = !godMode; }
void PlayerMod::ToggleNeverWanted() { neverWanted = !neverWanted; }
void PlayerMod::ToggleNoRagdoll() { noRagdoll = !noRagdoll; }
void PlayerMod::ToggleSuperJump() { superJump = !superJump; }
void PlayerMod::ToggleFastRun() { fastRun = !fastRun; }

void PlayerMod::HealFull() {
    Ped ped = PLAYER::PLAYER_PED_ID();
    int maxHealth = ENTITY::GET_ENTITY_MAX_HEALTH(ped);
    ENTITY::SET_ENTITY_HEALTH(ped, maxHealth);
    PED::SET_PED_ARMOUR(ped, 100);
}
void PlayerMod::ClearWanted() {
    Player pid = PLAYER::PLAYER_ID();
    PLAYER::SET_PLAYER_WANTED_LEVEL(pid, 0, false);
    PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(pid, false);
}

void PlayerMod::Tick() {
    Ped ped = PLAYER::PLAYER_PED_ID();
    Player pid = PLAYER::PLAYER_ID();

    if (godMode) {
        ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
        PED::SET_PED_CAN_BE_TARGETTED(ped, false);
    }
    else {
        ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
        PED::SET_PED_CAN_BE_TARGETTED(ped, true);
    }

    if (neverWanted) {
        PLAYER::SET_PLAYER_WANTED_LEVEL(pid, 0, false);
        PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(pid, false);
    }

    PED::SET_PED_CAN_RAGDOLL(ped, !noRagdoll);

    if (superJump) {
        GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(pid);
    }

    if (fastRun) PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(pid, 1.49f);
    else         PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(pid, 1.0f);
}

bool* PlayerMod::GodModePtr() { return &godMode; }
bool* PlayerMod::NeverWantedPtr() { return &neverWanted; }
bool* PlayerMod::NoRagdollPtr() { return &noRagdoll; }
bool* PlayerMod::SuperJumpPtr() { return &superJump; }
bool* PlayerMod::FastRunPtr() { return &fastRun; }

bool PlayerMod::IsGodMode() { return godMode; }
bool PlayerMod::IsNeverWanted() { return neverWanted; }
bool PlayerMod::IsNoRagdoll() { return noRagdoll; }
bool PlayerMod::IsSuperJump() { return superJump; }
bool PlayerMod::IsFastRun() { return fastRun; }

void PlayerMod::SetGodMode(bool v) { godMode = v; }
void PlayerMod::SetNeverWanted(bool v) { neverWanted = v; }
void PlayerMod::SetNoRagdoll(bool v) { noRagdoll = v; }
void PlayerMod::SetSuperJump(bool v) { superJump = v; }
void PlayerMod::SetFastRun(bool v) { fastRun = v; }
