#include "app.hpp"
#include "../modules/player.hpp"
#include <Windows.h>
#include <memory>

void App::Init() {
    auto mainMenu = std::make_shared<Menu>("Nebula Menu");
    auto playerMenu = std::make_shared<Menu>("Player");

    // Toggle che punta allo stato reale in PlayerMod
    playerMenu->AddToggle("God Mode", PlayerMod::GodModePtr());

    // Submenu
    mainMenu->AddSubmenu("Player", playerMenu);

    root = mainMenu;
    stack.clear();
}

void App::Tick() {
    if (menuOpen && root) {
        root->Render();
    }
    PlayerMod::Tick();
}

void App::OnKey(int vk) {
    if (vk == VK_F5) {          // open/close menu
        menuOpen = !menuOpen;
        return;
    }
    if (!menuOpen || !root) return;

    if (vk == VK_UP)      root->Up();
    else if (vk == VK_DOWN)  root->Down();
    else if (vk == VK_RETURN) {
        auto next = root->Select();     // entra nel submenu o toggla
        if (next) {                     // se c'è un submenu, entra
            stack.push_back(root);
            root = next;
        }
    }
    else if (vk == VK_BACK) {           // torna indietro
        if (!stack.empty()) {
            root = stack.back();
            stack.pop_back();
        }
    }
}
