#include "PlayerMenu.hpp"
#include "menu.hpp"
#include "PlayerMod.hpp"

namespace PlayerMenu {

    void Attach(const std::shared_ptr<Menu>& mainMenu) {
        mainMenu->AddFolder("Player", [](std::shared_ptr<Menu> m) {

            m->AddToggle("God Mode", PlayerMod::GodModePtr());
            m->AddToggle("Never Wanted", PlayerMod::NeverWantedPtr());
            m->AddToggle("No Ragdoll", PlayerMod::NoRagdollPtr());
            m->AddToggle("Super Jump", PlayerMod::SuperJumpPtr());
            m->AddToggle("Fast Run", PlayerMod::FastRunPtr());

            m->AddAction("Heal & Armor", []() { PlayerMod::HealFull(); });
            m->AddAction("Clear Wanted", []() { PlayerMod::ClearWanted(); });
            });
    }

}
