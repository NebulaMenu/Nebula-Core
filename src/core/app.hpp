#pragma once
#include "menu.hpp"
#include <memory>
#include <vector>

class App {
    bool menuOpen = false;
    std::shared_ptr<Menu> root;
    std::vector<std::shared_ptr<Menu>> stack;
public:
    void Init();
    void Tick();
    void OnKey(int vk);
};
