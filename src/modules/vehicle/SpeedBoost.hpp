#pragma once
#include <memory>

class Menu;

namespace VehicleMod {
    namespace SpeedBoost {
        extern bool Enabled;
        extern float PowerMult;

        void Attach(const std::shared_ptr<Menu>& vehicleMenu); 
        void Tick();
    }
}
