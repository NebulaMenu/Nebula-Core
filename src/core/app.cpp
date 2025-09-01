#include "app.hpp"
#include "modules/player/PlayerMenu.hpp"
#include "modules/player/PlayerMod.hpp"
#include "modules/vehicle/VehicleMenu.hpp"
#include "modules/vehicle/VehicleModule.hpp"
#include "modules/teleport/TeleportModule.hpp"
#include "utils/NebulaUI.hpp"
#include "menu.hpp"
#include "script.h"
#include <Windows.h>
#include <memory>
#include <vector>

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

enum class Cmd {
    ToggleMenu,
    Up, Down, Left, Right,
    Select, Back
};

static std::vector<Cmd> g_cmdQueue;
static inline void Enqueue(Cmd c) { g_cmdQueue.push_back(c); }

static void ShowNebulaWelcomeNotificationOnce() {
    static bool shown = false;
    if (shown) return;
    shown = true;

    NebulaUI::ShowNotification(
        "Welcome to ~w~NEBULA~w~. Press ~y~F5~w~ to open.~n~"
        "~y~Version: 0.0.1 Loaded!~n~"
        "By Elio",
        "NEBULA",
        "Menu loaded"
    );
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
    if (!g_cmdQueue.empty()) {
        for (Cmd c : g_cmdQueue) {
            switch (c) {
            case Cmd::ToggleMenu:
                menuOpen = !menuOpen;
                if (menuOpen && root) {
                    root->Open();
                    AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"SELECT", (char*)"HUD_FRONTEND_DEFAULT_SOUNDSET", false);
                }
                else {
                    AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"BACK", (char*)"HUD_FRONTEND_DEFAULT_SOUNDSET", false);
                    if (root) root->Close();
                }
                break;

            case Cmd::Up:
                if (menuOpen && root) root->Up();
                break;
            case Cmd::Down:
                if (menuOpen && root) root->Down();
                break;
            case Cmd::Left:
                if (menuOpen && root) root->Left();
                break;
            case Cmd::Right:
                if (menuOpen && root) root->Right();
                break;
            case Cmd::Select:
                if (menuOpen && root) {
                    if (auto next = root->Select()) { stack.push_back(root); root = next; }
                }
                break;
            case Cmd::Back:
                if (menuOpen && root) {
                    AUDIO::PLAY_SOUND_FRONTEND(-1, (char*)"BACK", (char*)"HUD_FRONTEND_DEFAULT_SOUNDSET", false);
                    if (!stack.empty()) {
                        root = stack.back(); stack.pop_back();
                    }
                    else {
                        menuOpen = false;
                        root->Close();
                    }
                }
                break;
            }
        }
        g_cmdQueue.clear();
    }

    if (menuOpen && root) {
        root->Render();
    }

    PlayerMod::Tick();
    VehicleModule::Tick();
    TeleportMod::Tick();
}

void App::OnKey(int vk) {
    if (vk == VK_F5) { Enqueue(Cmd::ToggleMenu); return; }
    if (!menuOpen || !root) return;

    if (vk == VK_UP)       Enqueue(Cmd::Up);
    else if (vk == VK_DOWN)     Enqueue(Cmd::Down);
    else if (vk == VK_LEFT)     Enqueue(Cmd::Left);
    else if (vk == VK_RIGHT)    Enqueue(Cmd::Right);
    else if (vk == VK_RETURN)   Enqueue(Cmd::Select);
    else if (vk == VK_BACK)     Enqueue(Cmd::Back);
}
