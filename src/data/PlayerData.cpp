#include "PlayerData.hpp"



PlayerData::PlayerData(std::string& n, int acc, int user, int c1, int c2, int c3, int icon, IconType it, bool glow) 
    : name(n),
    account_id(acc),
    user_id(user),
    color1(c1),
    color2(c2),
    color3(c3),
    icon_id(icon),
    icon_type(it),
    glow_enabled(glow)
    {}



std::optional<size_t> find_user(const std::vector<PlayerData>& users, int to_find) {
    for(size_t i = 0; i < users.size(); i++) {
        if(users[i].account_id == to_find) {
            return i;
        }
    }

    return std::nullopt; 
}