#include "NebulaUI.hpp"
#include "script.h"

namespace NebulaUI {

    void ShowSubtitle(const std::string& message, int duration) {
        UI::_SET_TEXT_ENTRY_2((char*)"STRING");
        UI::_ADD_TEXT_COMPONENT_STRING((char*)message.c_str());
        UI::_DRAW_SUBTITLE_TIMED(duration, true);
    }

    void ShowNotification(const std::string& body,
        const char* title,
        const char* subtitle,
        const char* dict,
        const char* icon)
    {
        GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT((char*)dict, false);
        if (!GRAPHICS::HAS_STREAMED_TEXTURE_DICT_LOADED((char*)dict)) {
            dict = (char*)"CHAR_SOCIAL_CLUB";
            icon = (char*)"CHAR_SOCIAL_CLUB";
        }

        UI::_SET_NOTIFICATION_TEXT_ENTRY((char*)"STRING");
        UI::_ADD_TEXT_COMPONENT_STRING((char*)body.c_str());
        UI::_SET_NOTIFICATION_MESSAGE((char*)dict, (char*)icon, false, 4, (char*)title, (char*)subtitle);
        UI::_DRAW_NOTIFICATION(false, true);
    }
}
