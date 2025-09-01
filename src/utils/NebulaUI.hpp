#pragma once
#include <string>

namespace NebulaUI {
    void ShowSubtitle(const std::string& message, int duration = 2000);
    void ShowNotification(const std::string& body,
        const char* title = "NEBULA",
        const char* subtitle = "",
        const char* dict = "CHAR_SOCIAL_CLUB",
        const char* icon = "CHAR_SOCIAL_CLUB");
}
