#include "player.hpp"
#include "script.h"

bool PlayerMod::godMode = false;

void PlayerMod::ToggleGodMode() { godMode = !godMode; }
bool* PlayerMod::GodModePtr() { return &godMode; }
bool  PlayerMod::IsGodMode() { return godMode; }
void  PlayerMod::SetGodMode(bool v) { godMode = v; }

void PlayerMod::Tick() {
    Ped ped = PLAYER::PLAYER_PED_ID();
    if (godMode) {
        ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
        PED::SET_PED_CAN_BE_TARGETTED(ped, false);
    }
    else {
        ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
        PED::SET_PED_CAN_BE_TARGETTED(ped, true);
    }
}
