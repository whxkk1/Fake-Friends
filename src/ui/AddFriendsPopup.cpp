#include "AddFriendsPopup.hpp"
#include "../data/PlayerData.hpp"

cocos2d::CCNode* AddFriendsPopup::create_player(IconType type) {
    int id = 0;
    switch(type) {
        case IconType::Cube: id = current_user_score->m_playerCube; break;
        case IconType::Ship: id = current_user_score->m_playerShip; break;
        case IconType::Ball: id = current_user_score->m_playerBall; break;
        case IconType::Ufo: id = current_user_score->m_playerUfo; break;
        case IconType::Wave: id = current_user_score->m_playerWave; break;
        case IconType::Robot: id = current_user_score->m_playerRobot; break;
        case IconType::Spider: id = current_user_score->m_playerSpider; break;
        case IconType::Swing: id = current_user_score->m_playerSwing; break;
        case IconType::Jetpack: id = current_user_score->m_playerJetpack; break;
        default: id = 0; break;
    }

    auto player = SimplePlayer::create(1);
    if(!player) return nullptr;

    auto game_manager = GameManager::get();

    player->updatePlayerFrame(id, type);
    player->setColor(game_manager->colorForIdx(current_user_score->m_color1));
    player->setSecondColor(game_manager->colorForIdx(current_user_score->m_color2));

    if(current_user_score->m_glowEnabled) {
        player->setGlowOutline(game_manager->colorForIdx(current_user_score->m_color3));
    }

    //player->setScale(1.25f);

    const float node_size = popup_size.height * 0.5;
    auto node = CCNode::create();
    node->setContentSize({ node_size, node_size });
    player->setPosition({ node_size / 2, node_size / 2 });
    player->setScale((node->getContentHeight() / 30.f) * 0.65);
    node->addChild(player);
    
    return node;
}

bool AddFriendsPopup::init(GJUserScore* user, std::vector<PlayerData> player_data) {
    auto win_size = cocos2d::CCDirector::get()->getWinSize();
    popup_size = cocos2d::CCSize(win_size.width * 0.75f, win_size.height * 0.45f);

    if(!geode::Popup::init(popup_size) || !user) return false;

    this->m_noElasticity = true;
    this->setTitle("Display Icon");

    current_user_score = user;
    data = std::move(player_data);

    cocos2d::CCMenu* menu = cocos2d::CCMenu::create();
    menu->setContentSize(popup_size * 0.95);
    menu->ignoreAnchorPointForPosition(false);

    auto layout = geode::RowLayout::create();
    layout->setGap(20);
    layout->setCrossAxisOverflow(true);

    menu->setLayout(layout);

    for(size_t i = 0; i < 9; i++) {
        auto spr = CCMenuItemSpriteExtra::create(create_player(static_cast<IconType>(i)), this, menu_selector(AddFriendsPopup::select_player));
        spr->setTag(i + 1);
        menu->addChild(spr);
    }

    menu->updateLayout();
    this->addChild(menu);
        
    return true;
}

AddFriendsPopup* AddFriendsPopup::create(GJUserScore* user, std::vector<PlayerData> player_data) {
    auto ret = new AddFriendsPopup();
    if(ret->init(user, std::move(player_data))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void AddFriendsPopup::select_player(cocos2d::CCObject* sender) {
    IconType icon_type;
    int id;
    switch (sender->getTag()) {
        case 1: icon_type = IconType::Cube; id = current_user_score->m_playerCube; break;
        case 2: icon_type = IconType::Ship; id = current_user_score->m_playerShip; break;
        case 3: icon_type = IconType::Ball; id = current_user_score->m_playerBall; break;
        case 4: icon_type = IconType::Ufo; id = current_user_score->m_playerUfo; break;
        case 5: icon_type = IconType::Wave; id = current_user_score->m_playerWave; break;
        case 6: icon_type = IconType::Robot; id = current_user_score->m_playerRobot; break;
        case 7: icon_type = IconType::Spider; id = current_user_score->m_playerSpider; break;
        case 8: icon_type = IconType::Swing; id = current_user_score->m_playerSwing; break;
        case 9: icon_type = IconType::Jetpack; id = current_user_score->m_playerJetpack; break;
        default: icon_type = IconType::Cube; id = 0; break;
    }

    data.emplace_back(
        current_user_score->m_userName,
        current_user_score->m_accountID,
        current_user_score->m_userID,
        current_user_score->m_color1,
        current_user_score->m_color2,
        current_user_score->m_color3,
        id,
        icon_type,
        current_user_score->m_glowEnabled
    );
    geode::Mod::get()->setSavedValue("users", data);

    this->onClose(nullptr);
}