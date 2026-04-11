#include <Geode/modify/GameManager.hpp> 


class $modify(GameManagerHook, GameManager) {
    void reportAchievementWithID(char const* key, int percent, bool dontNotify) {
        if(geode::Mod::get()->getSettingValue<std::string>("fake-friends-mode") != "Disabled" && std::string_view(key).contains("geometry.ach.friends0")) return;
        GameManager::reportAchievementWithID(key, percent, dontNotify);
    }
}; 