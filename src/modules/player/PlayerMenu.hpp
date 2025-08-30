#pragma once
#include <memory>

class Menu;

namespace PlayerMenu {
    void Attach(const std::shared_ptr<Menu>& mainMenu);
}
