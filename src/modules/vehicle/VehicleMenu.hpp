#pragma once
#include <memory>

class Menu;

namespace VehicleMenu {
    void Attach(const std::shared_ptr<Menu>& mainMenu);
}
