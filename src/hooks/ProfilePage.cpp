#include <Geode/modify/ProfilePage.hpp>
#include "../data/PlayerData.hpp"
#include "../ui/AddFriendsPopup.hpp"
#include "../include.hpp"

class $modify(ProfilePageHook, ProfilePage) {
    struct Fields {
        bool has_loaded = false;
        std::shared_ptr<std::vector<PlayerData>> player_data;
        bool is_friend_added = false;
    };
    void loadPageFromUserInfo(GJUserScore* score) {
        
        ProfilePage::loadPageFromUserInfo(score);

        if(m_fields->has_loaded) return;

        m_fields->has_loaded = true;
        auto left_menu = this->getChildByIDRecursive("left-menu");
        if(!left_menu) {
            geode::log::warn("Left menu not found.");
            return;
        }
        m_fields->player_data = std::make_shared<std::vector<PlayerData>>(
            geode::Mod::get()->getSavedValue<std::vector<PlayerData>>("users")
        );

        cocos2d::CCSprite* add_button_spr = cocos2d::CCSprite::createWithSpriteFrameName("AddFriend.png"_spr);
        cocos2d::CCSprite* remove_button_spr = cocos2d::CCSprite::createWithSpriteFrameName("RemoveFriend.png"_spr);
        add_button_spr->setScale(0.7f);
        remove_button_spr->setScale(0.7f);

        auto idx = find_user(*m_fields->player_data, score->m_accountID);
    
        auto button = CCMenuItemSpriteExtra::create(idx ? remove_button_spr : add_button_spr, this, menu_selector(ProfilePageHook::on_button));
        button->setID("fake-friends-btn"_spr);
        if(idx) {
            m_fields->is_friend_added = true;
            button->setTag(FakeFriends::REMOVE_BUTTON_TAG);
        } else {
            button->setTag(FakeFriends::ADD_BUTTON_TAG);
        }
        
        left_menu->addChild(button);
        left_menu->updateLayout();
    }
    void on_button(cocos2d::CCObject* sender) {
        auto sender_button = static_cast<CCMenuItemSpriteExtra*>(sender);
        switch(sender_button->getTag()) {
            case FakeFriends::ADD_BUTTON_TAG: // 1
                AddFriendsPopup::create(m_score, m_fields->player_data, static_cast<CCMenuItemSpriteExtra*>(sender))->show();
                break;
            case FakeFriends::REMOVE_BUTTON_TAG: { // 2
                auto idx = find_user(*m_fields->player_data, m_score->m_accountID);
                if(!idx) {
                    geode::Notification::create("User doesnt exist!", geode::NotificationIcon::Warning)->show();
                    return;
                }
                m_fields->player_data->erase(m_fields->player_data->begin() + idx.value());
                geode::Mod::get()->setSavedValue("users", m_fields->player_data);
                geode::Notification::create("User Removed.", geode::NotificationIcon::Success)->show();
                sender_button->setTag(FakeFriends::ADD_BUTTON_TAG);

                cocos2d::CCSprite* add_button_spr = cocos2d::CCSprite::createWithSpriteFrameName("AddFriend.png"_spr);
                add_button_spr->setScale(0.7f);
                sender_button->setSprite(add_button_spr);
                sender_button->updateSprite();
                break;
            }
            default:
                return;
        }
    }
};


