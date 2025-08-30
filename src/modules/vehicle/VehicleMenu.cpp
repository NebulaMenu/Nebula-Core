#include "VehicleMenu.hpp"
#include "menu.hpp"
#include "SpeedBoost.hpp"

namespace VehicleMenu {
    void Attach(const std::shared_ptr<Menu>& mainMenu) {
        mainMenu->AddFolder("Vehicle", [](std::shared_ptr<Menu> m) {
            VehicleMod::SpeedBoost::Attach(m);
            });
    }
}
