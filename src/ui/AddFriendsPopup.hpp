#pragma once
#include "../data/PlayerData.hpp"

class AddFriendsPopup : public geode::Popup {
private:
    GJUserScore* current_user_score = nullptr;
    cocos2d::CCSize popup_size;
    cocos2d::CCNode* create_player(IconType type);
    void select_player(cocos2d::CCObject* sender);
    std::shared_ptr<std::vector<PlayerData>> data;
    geode::WeakRef<CCMenuItemSpriteExtra> button = nullptr;
protected:
    bool init(GJUserScore* user, std::shared_ptr<std::vector<PlayerData>> player_data, geode::WeakRef<CCMenuItemSpriteExtra> button_caller);
public:
    static AddFriendsPopup* create(GJUserScore* user, std::shared_ptr<std::vector<PlayerData>> player_data, geode::WeakRef<CCMenuItemSpriteExtra> button_caller);
};