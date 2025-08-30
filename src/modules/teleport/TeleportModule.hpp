#pragma once
#include <memory>

class Menu;

namespace TeleportMod {
    void AttachTo(const std::shared_ptr<Menu>& mainMenu);
    void Tick();
}
