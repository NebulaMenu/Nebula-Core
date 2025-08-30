#include "VehicleModule.hpp"
#include "SpeedBoost.hpp"

namespace VehicleModule {
    void Tick() {
        VehicleMod::SpeedBoost::Tick();
    }
}
