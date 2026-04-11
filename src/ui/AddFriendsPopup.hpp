#pragma once
#include "../data/PlayerData.hpp"

class AddFriendsPopup : public geode::Popup {
private:
    GJUserScore* current_user_score = nullptr;
    cocos2d::CCSize popup_size;
    cocos2d::CCNode* create_player(IconType type);
    void select_player(cocos2d::CCObject* sender);
    std::vector<PlayerData> data;
protected:
    bool init(GJUserScore* user, std::vector<PlayerData> player_data);
public:
    static AddFriendsPopup* create(GJUserScore* user, std::vector<PlayerData> player_data);
};