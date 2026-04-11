#include <Geode/modify/ProfilePage.hpp>
#include "../data/PlayerData.hpp"
#include "../ui/AddFriendsPopup.hpp"

class $modify(ProfilePageHook, ProfilePage) {
    void loadPageFromUserInfo(GJUserScore* score) {
        ProfilePage::loadPageFromUserInfo(score);


        auto left_menu = this->getChildByIDRecursive("left-menu");
        if(!left_menu) {
            geode::log::warn("Left menu not found.");
            return;
        }

        auto add_button_spr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
        add_button_spr->setScale(0.7f);
		auto add_btn = CCMenuItemSpriteExtra::create(add_button_spr, this, menu_selector(ProfilePageHook::save_user_data));

        auto remove_button_spr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
        remove_button_spr->setScale(0.7f);
		auto remove_btn = CCMenuItemSpriteExtra::create(remove_button_spr, this, menu_selector(ProfilePageHook::remove_user_data));

        left_menu->addChild(add_btn);
        left_menu->addChild(remove_btn);
        left_menu->updateLayout();
    }
    void save_user_data(cocos2d::CCObject* sender) {
        auto saved_users = geode::Mod::get()->getSavedValue<std::vector<PlayerData>>("users");
        for(const auto& user : saved_users) {
            if(user.account_id == m_score->m_accountID) {
                geode::Notification::create("User already saved!", geode::NotificationIcon::Info)->show();
                return;
            }
        }
        AddFriendsPopup::create(m_score, std::move(saved_users))->show();
    }
    void remove_user_data(cocos2d::CCObject* sender) {
        if(!m_score) return;

        auto saved_users = geode::Mod::get()->getSavedValue<std::vector<PlayerData>>("users");
        auto index = find_user(saved_users, m_score->m_accountID);
        if(!index) {
            geode::Notification::create("User doesnt exist!", geode::NotificationIcon::Info)->show();
            return;
        }
        saved_users.erase(saved_users.begin() + index.value());
        geode::Mod::get()->setSavedValue("users", saved_users);
        
    }
};


