#include "app.hpp"
#include "modules/player/PlayerMenu.hpp"
#include "modules/player/PlayerMod.hpp"
#include "modules/vehicle/VehicleMenu.hpp"
#include "modules/vehicle/VehicleModule.hpp"
#include "modules/teleport/TeleportModule.hpp"
#include "script.h"
#include <Windows.h>
#include <memory>

namespace Ctrl {
    constexpr int UP = 172;
    constexpr int DOWN = 173;
    constexpr int LEFT = 174;
    constexpr int RIGHT = 175;
    constexpr int ACCEPT = 176;
    constexpr int CANCEL = 177;
    constexpr int PAUSE = 199;
    constexpr int PAUSE_2 = 200;

    constexpr int PHONE = 27;

    constexpr int ATTACK = 24;
    constexpr int AIM = 25;

    constexpr int VEH_ATTACK = 69;
    constexpr int VEH_AIM = 68;
}

static inline void HandleMenuControls(bool menuOpen)
{
    const int GROUP = 0;

    if (menuOpen) {
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::PHONE, true);

        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::UP, true);
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::DOWN, true);
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::LEFT, true);
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::RIGHT, true);
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::ACCEPT, true);
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::CANCEL, true);
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::PAUSE, true);
        CONTROLS::DISABLE_CONTROL_ACTION(GROUP, Ctrl::PAUSE_2, true);

        // CONTROLS::DISABLE_CONTROL_ACTION(GROUP, 201, true);
        // CONTROLS::DISABLE_CONTROL_ACTION(GROUP, 202, true);

        CONTROLS::SET_INPUT_EXCLUSIVE(GROUP, Ctrl::UP);
        CONTROLS::SET_INPUT_EXCLUSIVE(GROUP, Ctrl::DOWN);
        CONTROLS::SET_INPUT_EXCLUSIVE(GROUP, Ctrl::LEFT);
        CONTROLS::SET_INPUT_EXCLUSIVE(GROUP, Ctrl::RIGHT);
        CONTROLS::SET_INPUT_EXCLUSIVE(GROUP, Ctrl::ACCEPT);
        CONTROLS::SET_INPUT_EXCLUSIVE(GROUP, Ctrl::CANCEL);

        // CONTROLS::ENABLE_CONTROL_ACTION(GROUP, Ctrl::ATTACK, true);
        // CONTROLS::ENABLE_CONTROL_ACTION(GROUP, Ctrl::AIM,    true);
        // CONTROLS::ENABLE_CONTROL_ACTION(GROUP, Ctrl::VEH_ATTACK, true);
        // CONTROLS::ENABLE_CONTROL_ACTION(GROUP, Ctrl::VEH_AIM,    true);
    }
}

static inline void PostNotificationWithIcon(const char* body,
    const char* title,
    const char* subtitle)
{
    char* dict = (char*)"nebula";
    char* tx = (char*)"nebula_logo";

    GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT(dict, false);
    if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED(dict)) {
        dict = (char*)"CHAR_SOCIAL_CLUB";
        tx = (char*)"CHAR_SOCIAL_CLUB";
    }

    UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
    UI::_ADD_TEXT_COMPONENT_STRING((char*)body);


    // _SET_NOTIFICATION_MESSAGE(txDict, txName, flash, iconType, sender, subject)
    UI::_SET_NOTIFICATION_MESSAGE(dict, tx, false, 4, (char*)title, (char*)subtitle);

    UI::_DRAW_NOTIFICATION(false, true);

    //if (dict == (char*)"nebula")
    //    GRAPHICS::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED((char*)"nebula");
}

static void ShowNebulaWelcomeNotificationOnce()
{
    static bool shown = false;
    if (shown) return;
    shown = true;

    const char* body =
        "Welcome to ~w~NEBULA~w~. Press ~y~F5~w~ to open.~n~"
        "~y~Version: 0.0.1 Loaded!~n~"
        "By Elio";

    PostNotificationWithIcon(body, "NEBULA", "Menu loaded");
}

void App::Init() {
    auto mainMenu = std::make_shared<Menu>("Nebula Menu");
    PlayerMenu::Attach(mainMenu);
    VehicleMenu::Attach(mainMenu);
    TeleportMod::AttachTo(mainMenu);

    root = mainMenu;
    stack.clear();

    ShowNebulaWelcomeNotificationOnce();
}

void App::Tick() {
    HandleMenuControls(menuOpen);

    if (menuOpen && root) {
        root->Render();
    }

    PlayerMod::Tick();
    VehicleModule::Tick();
    TeleportMod::Tick();
}

void App::OnKey(int vk) {
    if (vk == VK_F5) {
        menuOpen = !menuOpen;
        if (menuOpen && root) root->Open();
        else if (root) root->Close();
        return;
    }
    if (!menuOpen || !root) return;

    if (vk == VK_UP)       root->Up();
    else if (vk == VK_DOWN)     root->Down();
    else if (vk == VK_LEFT)     root->Left();
    else if (vk == VK_RIGHT)    root->Right();
    else if (vk == VK_RETURN) { if (auto next = root->Select()) { stack.push_back(root); root = next; } }
    else if (vk == VK_BACK) { if (!stack.empty()) { root = stack.back(); stack.pop_back(); } }
}
