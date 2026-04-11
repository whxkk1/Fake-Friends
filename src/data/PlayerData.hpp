#pragma once
#include <matjson.hpp>
#include <utility>

struct PlayerData {
    std::string name;
    int account_id;
    int user_id;
    int color1;
    int color2;
    int color3;
    int icon_id;
    IconType icon_type;
    bool glow_enabled;

    PlayerData() = default;

    PlayerData(std::string& n, int acc, int user, int c1, int c2, int c3, int icon, IconType it, bool glow);
    geode::Ref<GJUserScore> get_user_score();
};


template<>
struct matjson::Serialize<PlayerData> {
    static geode::Result<PlayerData> fromJson(matjson::Value const& value) {
        GEODE_UNWRAP_INTO(std::string name, value["name"].asString());
        GEODE_UNWRAP_INTO(int account_id, value["accountID"].asInt());
        GEODE_UNWRAP_INTO(int user_id, value["userID"].asInt());
        GEODE_UNWRAP_INTO(int color1, value["color1"].asInt());
        GEODE_UNWRAP_INTO(int color2, value["color2"].asInt());
        GEODE_UNWRAP_INTO(int color3, value["color3"].asInt());
        GEODE_UNWRAP_INTO(int icon_id, value["iconID"].asInt());
        GEODE_UNWRAP_INTO(int icon_type_int, value["iconType"].asInt());
        GEODE_UNWRAP_INTO(bool glow_enabled, value["glowEnabled"].asBool());
        IconType icon_type = static_cast<IconType>(icon_type_int);
        return geode::Ok(PlayerData{name, account_id, user_id, color1, color2, color3, icon_id, icon_type, glow_enabled});
    }

    static matjson::Value toJson(PlayerData const& value) {
        auto obj = matjson::Value();
        obj["name"] = value.name;
        obj["accountID"] = value.account_id;
        obj["userID"] = value.user_id;
        obj["color1"] = value.color1;
        obj["color2"] = value.color2;
        obj["color3"] = value.color3;
        obj["iconID"] = value.icon_id;
        obj["iconType"] = static_cast<int>(value.icon_type);
        obj["glowEnabled"] = value.glow_enabled;
        return obj;
    }
};

std::optional<size_t> find_user(const std::vector<PlayerData>& users, int to_find);
