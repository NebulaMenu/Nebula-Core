#include "app.hpp"
#include "modules/player/PlayerMenu.hpp"
#include "modules/player/PlayerMod.hpp"
#include "modules/vehicle/VehicleMenu.hpp"
#include "modules/vehicle/VehicleModule.hpp"
#include "modules/teleport/TeleportModule.hpp"
#include "script.h" 
#include <Windows.h>
#include <memory>

static void DisableControlsForMenu()
{
    const int INPUT_GROUP = 0;

    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 27, true);    // INPUT_PHONE
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 172, true);  // INPUT_FRONTEND_UP
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 173, true);  // INPUT_FRONTEND_DOWN
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 174, true);  // INPUT_FRONTEND_LEFT
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 175, true);  // INPUT_FRONTEND_RIGHT
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 176, true);  // INPUT_FRONTEND_ACCEPT
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 177, true);  // INPUT_FRONTEND_CANCEL
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 199, true);  // INPUT_FRONTEND_PAUSE
    CONTROLS::DISABLE_CONTROL_ACTION(INPUT_GROUP, 200, true);  // INPUT_FRONTEND_PAUSE_ALTERNATE

    CONTROLS::SET_INPUT_EXCLUSIVE(INPUT_GROUP, 172);
    CONTROLS::SET_INPUT_EXCLUSIVE(INPUT_GROUP, 173);
    CONTROLS::SET_INPUT_EXCLUSIVE(INPUT_GROUP, 174);
    CONTROLS::SET_INPUT_EXCLUSIVE(INPUT_GROUP, 175);
    CONTROLS::SET_INPUT_EXCLUSIVE(INPUT_GROUP, 176);
    CONTROLS::SET_INPUT_EXCLUSIVE(INPUT_GROUP, 177);
}

static void ShowNebulaWelcomeNotification()
{
    UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
    UI::_ADD_TEXT_COMPONENT_STRING((char*)"Welcome to ~p~Nebula~w~! Press ~y~F5~w~ to open.");
    UI::_DRAW_NOTIFICATION(false, true);
}


void App::Init() {
    static bool s_welcomeShown = false;
    auto mainMenu = std::make_shared<Menu>("Nebula Menu");

    PlayerMenu::Attach(mainMenu);
    VehicleMenu::Attach(mainMenu);
    TeleportMod::AttachTo(mainMenu);

    root = mainMenu;
    stack.clear();
}

void App::Tick() {
    if (menuOpen && root) {
        DisableControlsForMenu();
        root->Render();
    }
    PlayerMod::Tick();
    VehicleModule::Tick();
    TeleportMod::Tick();
}

void App::OnKey(int vk) {
    if (vk == VK_F5) {
        menuOpen = !menuOpen;
        return;
    }
    if (!menuOpen || !root) return;

    if (vk == VK_UP)    root->Up();
    else if (vk == VK_DOWN)  root->Down();
    else if (vk == VK_RETURN) {
        if (auto next = root->Select()) { stack.push_back(root); root = next; }
    }
    else if (vk == VK_RIGHT) {
        if (auto next = root->CurrentSubmenu()) { stack.push_back(root); root = next; }
    }
    else if (vk == VK_LEFT || vk == VK_BACK) {
        if (!stack.empty()) { root = stack.back(); stack.pop_back(); }
    }
}
