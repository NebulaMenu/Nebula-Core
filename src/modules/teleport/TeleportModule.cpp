#include "TeleportModule.hpp"
#include "menu.hpp"
#include "script.h"

namespace {
    void TpTo(float x, float y, float z) {
        Ped ped = PLAYER::PLAYER_PED_ID();
        ENTITY::SET_ENTITY_COORDS(ped, x, y, z, false, false, false, true);
    }
}

namespace TeleportMod {
    void AttachTo(const std::shared_ptr<Menu>& mainMenu) {
        mainMenu->AddFolder("Teleport", [](std::shared_ptr<Menu> m) {
            m->AddAction("Safehouse (Franklin)", []() { TpTo(-14.568f, -1438.12f, 31.1015f); });
            m->AddAction("Mount Chiliad", []() { TpTo(450.718f, 5566.614f, 806.183f); });
            });
    }
    void Tick() {}
}
