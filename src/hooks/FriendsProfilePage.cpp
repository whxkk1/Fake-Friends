#include <Geode/modify/FriendsProfilePage.hpp>
#include "../data/PlayerData.hpp"


enum class SettingsMode {
    Disabled,
    AddFriends,
    OverrideFriends
};

inline SettingsMode get_mode() {
    auto value = geode::Mod::get()->getSettingValue<std::string>("fake-friends-mode");
    if (value == "Disabled") {
        return SettingsMode::Disabled;
    }
    else if (value == "Add fake friends to friend list") {
        return SettingsMode::AddFriends;
    }
    else if (value == "Show only fake friends") {
        return SettingsMode::OverrideFriends;
    }
    return SettingsMode::Disabled;
}

void sort_names(cocos2d::CCArray* scores) {
    if(!scores) return;
    geode::cocos::CCArrayExt<GJUserScore*> scores_ext = scores;
    std::sort(scores_ext.begin(), scores_ext.end(), [](GJUserScore* a, GJUserScore* b) {
        std::string users[2] = {
            a->m_userName,
            b->m_userName,
        };

        for(int i = 0; i < 2; i++) {
            std::transform(users[i].begin(), users[i].end(), users[i].begin(), [](unsigned char c) {
                return std::tolower(c);
            });
        }

        return users[0] < users[1];
    });
}

class $modify(FriendsProfilePageHook, FriendsProfilePage) {
    GJUserScore* create_user(const PlayerData& user) {
        auto u = GJUserScore::create();
        u->m_userName = user.name;
        u->m_accountID = user.account_id;
        u->m_userID = user.user_id;
        u->m_color1 = user.color1;
        u->m_color2 = user.color2;
        u->m_color3 = user.color3;
        u->m_iconID = user.icon_id;
        u->m_iconType = user.icon_type;
        u->m_glowEnabled = user.glow_enabled;
        u->m_messageState = 1;

        return u;
    }
    
    void getUserListFinished(cocos2d::CCArray* scores, UserListType type) {
        auto mode = get_mode();
        if(mode == SettingsMode::Disabled || type != UserListType::Friends) return FriendsProfilePage::getUserListFinished(scores, type);
        switch (mode) {
            case SettingsMode::Disabled:
                return FriendsProfilePage::getUserListFinished(scores, type);
                break;
            case SettingsMode::AddFriends: {
                auto saved_users = geode::Mod::get()->getSavedValue<std::vector<PlayerData>>("users");

                for(const auto& user : saved_users) {

                    bool should_skip = false;
                    if(!scores) return FriendsProfilePage::getUserListFinished(scores, type);
                    for(size_t i = 0; i < scores->count(); i++) {
                        if(static_cast<GJUserScore*>(scores->objectAtIndex(i))->m_accountID == user.account_id) {
                            should_skip = true;
                            break;
                        }
                    }
                    if(should_skip) continue;

                    auto u = create_user(user);
                    scores->addObject(u);
                }
                sort_names(scores);
                return FriendsProfilePage::getUserListFinished(scores, type);
                break;
            }
            case SettingsMode::OverrideFriends: {
                cocos2d::CCArray* new_scores = cocos2d::CCArray::create();
                auto saved_users = geode::Mod::get()->getSavedValue<std::vector<PlayerData>>("users");

                for(const auto& user : saved_users) {
                    auto u = create_user(user);
                    new_scores->addObject(u);
                }
                sort_names(scores);
                return FriendsProfilePage::getUserListFinished(new_scores, type);
                break;
                
            }
            default:
                return FriendsProfilePage::getUserListFinished(scores, type);
                break;
        }
    
    }
    void onClose(CCObject* sender) {
        if(!m_users) return;
        auto saved_users = geode::Mod::get()->getSavedValue<std::vector<PlayerData>>("users");
        for(const auto& user : saved_users) {
            for(size_t i = 0; i < m_users->count(); i++) {
                if(static_cast<GJUserScore*>(m_users->objectAtIndex(i))->m_accountID == user.account_id) {
                    m_users->removeObjectAtIndex(i);
                }
            }
        }
        FriendsProfilePage::onClose(sender);
    }
};