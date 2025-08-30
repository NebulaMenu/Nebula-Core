#include "SpeedBoost.hpp"
#include "menu.hpp"
#include "script.h"

namespace VehicleMod {
    namespace SpeedBoost {

        bool  Enabled = false;
        float PowerMult = 2.0f;

        void Attach(const std::shared_ptr<Menu>& vehicleMenu) {
            vehicleMenu->AddToggle("Speed Boost", &Enabled);
        }

        void Tick() {
            if (!Enabled) return;

            Ped ped = PLAYER::PLAYER_PED_ID();
            if (!PED::IS_PED_IN_ANY_VEHICLE(ped, false)) return;

            Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(ped, false);
            if (!veh) return;

            float cur = ENTITY::GET_ENTITY_SPEED(veh);
            float bump = 5.0f * PowerMult;
            float target = cur + bump;

            if (target > 100.0f) target = 100.0f;

            VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh, target);

            // ENTITY::APPLY_FORCE_TO_ENTITY(veh, 1, 0.0f, 25.0f * PowerMult, 0.0f,
            //     0.0f, 0.0f, 0.0f, 0, true, true, true, true, true);
        }

    }
}
