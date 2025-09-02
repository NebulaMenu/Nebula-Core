#include "TeleportModule.hpp"
#include "../../utils/NebulaUI.hpp"
#include "menu.hpp"
#include "script.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace {
    struct Location {
        const char* name;
        float x, y, z;
        bool safeForVehicle;
    };

    struct LocationCategory {
        const char* name;
        std::vector<Location> locations;
    };


	// Example Teleport Locations
    static const std::vector<LocationCategory> kCategories = {
        {
            "Landmarks", {
                {"Mount Chiliad Peak", 450.718f, 5566.614f, 806.183f, true},
                {"Maze Bank Tower", -74.94243f, -818.63446f, 326.174347f, false},
                {"Vinewood Sign", 711.362f, 1198.134f, 348.526f, false},
                {"Observatory", -408.952f, 1170.656f, 325.764f, true},
                {"Fort Zancudo Tower", -2356.0940f, 3248.645f, 101.4505f, false},
                {"Playboy Mansion", -1475.234f, 167.088f, 55.841f, true},
                {"FIB Building", 135.733f, -749.216f, 258.152f, false},
                {"IAA Building", 117.220f, -620.938f, 206.046f, false},
            }
        },
        {
            "Safehouses", {
                {"Michael's House", -813.603f, 179.474f, 72.153f, true},
                {"Franklin's House (Hills)", 7.119f, 536.615f, 176.028f, true},
                {"Franklin's Aunt House", -14.380f, -1438.510f, 31.102f, true},
                {"Trevor's Trailer", 1972.610f, 3817.040f, 33.428f, true},
                {"Floyd's Apartment", -1150.703f, -1520.713f, 10.633f, false},
                {"Lester's House", 1273.898f, -1719.304f, 54.771f, true},
            }
        },
        {
            "Airports & Helipads", {
                {"LSIA Runway", -1336.077f, -3044.110f, 13.944f, true},
                {"LSIA Terminal", -1070.906f, -2972.122f, 13.955f, true},
                {"Sandy Shores Airfield", 1747.0f, 3273.7f, 41.1f, true},
                {"McKenzie Airfield", 2121.7f, 4796.3f, 41.1f, true},
                {"Fort Zancudo Runway", -2446.711f, 3142.811f, 32.828f, true},
                {"Noose HQ Helipad", 2511.374f, -426.771f, 118.195f, true},
                {"Hospital Helipad", 338.2f, -587.7f, 74.2f, true},
                {"Police Station Helipad", 449.5f, -981.3f, 43.7f, true},
            }
        },
        {
            "Special Interiors", {
                {"FIB Top Floor", 135.733f, -749.216f, 258.152f, false},
                {"Humane Labs Entrance", 3613.750f, 3744.209f, 28.690f, true},
                {"Humane Labs Tunnel", 3525.495f, 3705.301f, 20.992f, false},
                {"IAA Office", 117.220f, -620.938f, 206.046f, false},
                {"LifeInvader Office", -1047.900f, -233.000f, 39.000f, false},
                {"Lester's Warehouse", 707.324f, -966.986f, 30.412f, false},
                {"Strip Club", 127.4f, -1307.7f, 29.2f, false},
                {"Torture Room", 147.170f, -2201.804f, 4.688f, false},
            }
        },
        {
            "Underwater", {
                {"Submarine Wreck", 3002.5f, -4518.5f, -65.0f, false},
                {"Plane Wreck", -942.350f, 6608.752f, -20.912f, false},
                {"UFO Wreck", 762.0f, 7380.0f, -50.0f, false},
                {"Hatch", 4273.950f, 2975.714f, -170.0f, false},
            }
        },
        {
            "Secret Locations", {
                {"North Yankton Bank", 5309.519f, -1432.480f, 47.511f, false},
                {"North Yankton Cemetery", 3360.190f, -4849.670f, 111.800f, false},
                {"Yacht", -2043.974f, -1031.582f, 11.981f, false},
                {"Carrier", 3069.330f, -4632.4f, 16.043f, true},
            }
        }
    };

    static std::vector<Vector3> savedPositions;
    static int currentSavedSlot = 0;

    void TeleportEntity(Entity e, float x, float y, float z, bool findGround = true) {
        if (!ENTITY::DOES_ENTITY_EXIST(e)) return;

        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, x, y, z, false, false, true);

        if (findGround && z < 500.0f) {
            float groundZ;
            for (int i = 0; i < 10; i++) {
                WAIT(50);
                if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(x, y, 1000.0f, &groundZ, false)) {
                    ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, x, y, groundZ + 1.0f, false, false, true);
                    break;
                }
            }
        }

        if (ENTITY::IS_ENTITY_A_VEHICLE(e)) {
            VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(e);
        }
    }

    Entity GetTeleportEntity() {
        Ped playerPed = PLAYER::PLAYER_PED_ID();
        if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)) {
            return PED::GET_VEHICLE_PED_IS_USING(playerPed);
        }
        return playerPed;
    }
    
	// Moved to utils/NebulaUI
    //void ShowSubtitle(const std::string& message, int duration = 2000) {
    //    UI::_SET_TEXT_ENTRY_2((char*)"STRING");
    //    UI::_ADD_TEXT_COMPONENT_STRING((char*)message.c_str());
    //    UI::_DRAW_SUBTITLE_TIMED(duration, true);
    //}

    bool GetWaypointCoords(float& x, float& y) {
        int blip = UI::GET_FIRST_BLIP_INFO_ID(8);
        if (UI::DOES_BLIP_EXIST(blip)) {
            Vector3 coords = UI::GET_BLIP_INFO_ID_COORD(blip);
            x = coords.x;
            y = coords.y;
            return true;
        }
        return false;
    }

    void TeleportToWaypoint() {
        float x, y;
        if (!GetWaypointCoords(x, y)) {
            NebulaUI::ShowSubtitle("~r~No waypoint set!");
            return;
        }

        Entity e = GetTeleportEntity();
        TeleportEntity(e, x, y, 1000.0f, true);
        NebulaUI::ShowSubtitle("~g~Teleported to waypoint");
    }

    void TeleportToObjective() {
        int blips[] = { 1, 57, 128, 129, 130, 143, 144, 145, 146, 271, 286, 287, 288 };

        for (int i = 0; i < sizeof(blips) / sizeof(blips[0]); i++) {
            int blip = UI::GET_FIRST_BLIP_INFO_ID(blips[i]);
            if (UI::DOES_BLIP_EXIST(blip)) {
                Vector3 coords = UI::GET_BLIP_INFO_ID_COORD(blip);
                Entity e = GetTeleportEntity();
                TeleportEntity(e, coords.x, coords.y, coords.z, true);
                NebulaUI::ShowSubtitle("~g~Teleported to objective");
                return;
            }
        }

        NebulaUI::ShowSubtitle("~r~No objective found!");
    }

    void SaveCurrentPosition(int slot) {
        if (slot < 0 || slot >= 10) return;

        if (savedPositions.size() <= slot) {
            savedPositions.resize(slot + 1);
        }

        Entity e = GetTeleportEntity();
        savedPositions[slot] = ENTITY::GET_ENTITY_COORDS(e, true);

        NebulaUI::ShowSubtitle("~g~Position saved to slot " + std::to_string(slot + 1));
    }

    void LoadSavedPosition(int slot) {
        if (slot < 0 || slot >= savedPositions.size()) {
            NebulaUI::ShowSubtitle("~r~No saved position in slot " + std::to_string(slot + 1));
            return;
        }

        Vector3 pos = savedPositions[slot];
        if (pos.x == 0 && pos.y == 0 && pos.z == 0) {
            NebulaUI::ShowSubtitle("~r~No saved position in slot " + std::to_string(slot + 1));
            return;
        }

        Entity e = GetTeleportEntity();
        TeleportEntity(e, pos.x, pos.y, pos.z, false);
        NebulaUI::ShowSubtitle("~g~Teleported to saved position " + std::to_string(slot + 1));
    }

    void TeleportForward(float distance) {
        Entity e = GetTeleportEntity();
        Vector3 pos = ENTITY::GET_ENTITY_COORDS(e, true);
        float heading = ENTITY::GET_ENTITY_HEADING(e);

        float rad = heading * 3.14159f / 180.0f;
        float newX = pos.x + distance * sin(rad);
        float newY = pos.y + distance * cos(rad);

        TeleportEntity(e, newX, newY, pos.z, false);
        NebulaUI::ShowSubtitle("~g~Teleported forward " + std::to_string((int)distance) + "m");
    }
}

namespace TeleportMod {
    static float forwardDistance = 10.0f;

    void AttachTo(const std::shared_ptr<Menu>& mainMenu) {
        mainMenu->AddFolder("Teleport", [](std::shared_ptr<Menu> m) {

            m->AddSeparator("[ Quick actions ]");
            m->AddAction("Teleport to Waypoint", []() { TeleportToWaypoint(); });
            m->AddAction("Teleport to Objective", []() { TeleportToObjective(); });
            m->AddNumber("Teleport Forward", &forwardDistance, 1.0f, 100.0f, 1.0f);
            m->AddAction("Go Forward", []() { TeleportForward(forwardDistance); });

            m->AddSeparator("[ Saved locations ]");
            auto savedMenu = m->AddFolder("Saved Locations");
            for (int i = 0; i < 10; i++) {
                const int slotIndex = i;
                const std::string slotName = "Slot " + std::to_string(i + 1);
                savedMenu->AddAction("Save to " + slotName, [slotIndex]() { SaveCurrentPosition(slotIndex); });
                savedMenu->AddAction("Load " + slotName, [slotIndex]() { LoadSavedPosition(slotIndex); });
            }

            m->AddSeparator("[ Location categories ]");
            for (const auto& category : kCategories) {
                auto catMenu = m->AddFolder(category.name);
                for (const auto& loc : category.locations) {
                    catMenu->AddAction(loc.name, [loc]() {
                        Entity e = GetTeleportEntity();
                        if (!loc.safeForVehicle && ENTITY::IS_ENTITY_A_VEHICLE(e)) {
                            NebulaUI::ShowSubtitle("~y~Warning: Location not safe for vehicles!");
                        }
                        TeleportEntity(e, loc.x, loc.y, loc.z, false);
                        NebulaUI::ShowSubtitle("~g~Teleported to " + std::string(loc.name));
                        });
                }
            }
            });
    }

    void Tick() {
        static int lastGameTime = 0;
        int currentTime = GAMEPLAY::GET_GAME_TIMER();

        if (currentTime - lastGameTime > 1000) {
            lastGameTime = currentTime;

            if (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
                STREAMING::STOP_PLAYER_SWITCH();
            }
        }
    }
}