#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/TextInput.hpp>
using namespace geode::prelude;

struct IconSet {
    int cube;
    int ship;
    int ball;
    int ufo;
    int wave;
    int robot;
    int spider;
    int swing;
    int jetpack;
    int col1;
    int col2;
    int glowCol;
    int trail;
    int deathEffect;
    bool glowEnabled;
    std::string name;
    bool isEmpty = true;
};

class IconSetManager {
private:
    static inline std::vector<IconSet> savedSets;
    static inline int currentSlot = 0;
    static inline int maxSlots = 50;

public:
    static IconSet getCurrentIconSet() {
        auto gm = GameManager::sharedState();
        IconSet set;
        set.cube = gm->getPlayerFrame();
        set.ship = gm->getPlayerShip();
        set.ball = gm->getPlayerBall();
        set.ufo = gm->getPlayerBird();
        set.wave = gm->getPlayerDart();
        set.robot = gm->getPlayerRobot();
        set.spider = gm->getPlayerSpider();
        set.swing = gm->getPlayerSwing();
        set.jetpack = gm->getPlayerJetpack();
        set.col1 = gm->getPlayerColor();
        set.col2 = gm->getPlayerColor2();
        set.glowCol = gm->getPlayerGlowColor();
        set.trail = gm->getPlayerStreak();
        set.deathEffect = gm->getPlayerDeathEffect();
        set.glowEnabled = gm->getPlayerGlow();
        set.isEmpty = false;
        return set;
    }

    static void applyIconSet(const IconSet& set) {
        auto gm = GameManager::sharedState();
        gm->setPlayerFrame(set.cube);
        gm->setPlayerShip(set.ship);
        gm->setPlayerBall(set.ball);
        gm->setPlayerBird(set.ufo);
        gm->setPlayerDart(set.wave);
        gm->setPlayerRobot(set.robot);
        gm->setPlayerSpider(set.spider);
        gm->setPlayerSwing(set.swing);
        gm->setPlayerJetpack(set.jetpack);
        gm->setPlayerColor(set.col1);
        gm->setPlayerColor2(set.col2);
        gm->setPlayerStreak(set.trail);
        gm->setPlayerDeathEffect(set.deathEffect);
        gm->m_playerGlow = set.glowEnabled;
        gm->m_playerGlowColor = set.glowCol;
        gm->save();
    }

    static int findNextEmptySlot() {
        for (int i = 0; i < savedSets.size(); i++) {
            if (savedSets[i].isEmpty) return i;
        }
        if (savedSets.size() < maxSlots) {
            return savedSets.size();
        }
        return -1;
    }

    static void saveToSlot(int slot, const std::string& name = "") {
        IconSet set = getCurrentIconSet();
        set.name = name.empty() ? "Set " + std::to_string(slot + 1) : name;
        
        if (slot >= savedSets.size()) {
            savedSets.resize(slot + 1);
        }
        savedSets[slot] = set;

        matjson::Value obj = matjson::Value::object();
        obj["cube"] = set.cube;
        obj["ship"] = set.ship;
        obj["ball"] = set.ball;
        obj["ufo"] = set.ufo;
        obj["wave"] = set.wave;
        obj["robot"] = set.robot;
        obj["spider"] = set.spider;
        obj["swing"] = set.swing;
        obj["jetpack"] = set.jetpack;
        obj["col1"] = set.col1;
        obj["col2"] = set.col2;
        obj["glowCol"] = set.glowCol;
        obj["trail"] = set.trail;
        obj["deathEffect"] = set.deathEffect;
        obj["glowEnabled"] = set.glowEnabled;
        obj["name"] = set.name;
        obj["isEmpty"] = false;

        Mod::get()->setSavedValue("slot_" + std::to_string(slot), obj);
    }

    static void loadFromSlot(int slot) {
        if (slot >= savedSets.size() || savedSets[slot].isEmpty) return;
        auto set = savedSets[slot];
        applyIconSet(set);
        currentSlot = slot;
    }

    static void deleteSlot(int slot) {
        if (slot >= savedSets.size()) return;
        
        savedSets.erase(savedSets.begin() + slot);
        
        for (int i = slot; i < savedSets.size(); i++) {
            auto& set = savedSets[i];
            matjson::Value obj = matjson::Value::object();
            obj["cube"] = set.cube;
            obj["ship"] = set.ship;
            obj["ball"] = set.ball;
            obj["ufo"] = set.ufo;
            obj["wave"] = set.wave;
            obj["robot"] = set.robot;
            obj["spider"] = set.spider;
            obj["swing"] = set.swing;
            obj["jetpack"] = set.jetpack;
            obj["col1"] = set.col1;
            obj["col2"] = set.col2;
            obj["glowCol"] = set.glowCol;
            obj["trail"] = set.trail;
            obj["deathEffect"] = set.deathEffect;
            obj["glowEnabled"] = set.glowEnabled;
            obj["name"] = set.name;
            obj["isEmpty"] = false;
            Mod::get()->setSavedValue("slot_" + std::to_string(i), obj);
        }
        
        Mod::get()->setSavedValue("slot_" + std::to_string(savedSets.size()), matjson::Value::object());
    }

    static void renameSlot(int slot, const std::string& newName) {
        if (slot >= savedSets.size() || savedSets[slot].isEmpty) return;
        savedSets[slot].name = newName;
        
        auto& set = savedSets[slot];
        matjson::Value obj = matjson::Value::object();
        obj["cube"] = set.cube;
        obj["ship"] = set.ship;
        obj["ball"] = set.ball;
        obj["ufo"] = set.ufo;
        obj["wave"] = set.wave;
        obj["robot"] = set.robot;
        obj["spider"] = set.spider;
        obj["swing"] = set.swing;
        obj["jetpack"] = set.jetpack;
        obj["col1"] = set.col1;
        obj["col2"] = set.col2;
        obj["glowCol"] = set.glowCol;
        obj["trail"] = set.trail;
        obj["deathEffect"] = set.deathEffect;
        obj["glowEnabled"] = set.glowEnabled;
        obj["name"] = newName;
        obj["isEmpty"] = false;

        Mod::get()->setSavedValue("slot_" + std::to_string(slot), obj);
    }

    static void loadSavedSets() {
        savedSets.clear();
        
        for (int i = 0; i < maxSlots; i++) {
            auto saved = Mod::get()->getSavedValue<matjson::Value>("slot_" + std::to_string(i));
            IconSet set;
            
            if (saved.isObject() && !saved["isEmpty"].asBool().unwrapOr(true)) {
                set.cube = saved["cube"].asInt().unwrapOr(1);
                set.ship = saved["ship"].asInt().unwrapOr(1);
                set.ball = saved["ball"].asInt().unwrapOr(1);
                set.ufo = saved["ufo"].asInt().unwrapOr(1);
                set.wave = saved["wave"].asInt().unwrapOr(1);
                set.robot = saved["robot"].asInt().unwrapOr(1);
                set.spider = saved["spider"].asInt().unwrapOr(1);
                set.swing = saved["swing"].asInt().unwrapOr(1);
                set.jetpack = saved["jetpack"].asInt().unwrapOr(1);
                set.col1 = saved["col1"].asInt().unwrapOr(0);
                set.col2 = saved["col2"].asInt().unwrapOr(3);
                set.glowCol = saved["glowCol"].asInt().unwrapOr(0);
                set.trail = saved["trail"].asInt().unwrapOr(1);
                set.deathEffect = saved["deathEffect"].asInt().unwrapOr(1);
                set.glowEnabled = saved["glowEnabled"].asBool().unwrapOr(false);
                set.name = saved["name"].asString().unwrapOr("Set " + std::to_string(i + 1));
                set.isEmpty = false;
                savedSets.push_back(set);
            } else {
                break;
            }
        }
    }

    static int getSlotsCount() { return savedSets.size(); }
    static const IconSet& getSlot(int i) { return savedSets[i]; }
    static bool canAddMore() { return savedSets.size() < maxSlots; }
};

class RenamePopup : public geode::Popup<int, std::function<void()>> {
protected:
    int slotIndex;
    std::function<void()> callback;
    TextInput* input;

    bool setup(int index, std::function<void()> cb) override {
        slotIndex = index;
        callback = cb;
        auto winSize = this->m_mainLayer->getContentSize();

        this->setTitle("Rename Icon Set");

        input = TextInput::create(200.f, "Set Name");
        input->setString(IconSetManager::getSlot(slotIndex).name);
        input->setPosition({winSize.width / 2, winSize.height / 2 + 10});
        this->m_mainLayer->addChild(input);

        auto okBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("OK", "goldFont.fnt", "GJ_button_01.png", 0.8f),
            this,
            menu_selector(RenamePopup::onConfirm)
        );
        okBtn->setPosition({0, -30});

        auto menu = CCMenu::create();
        menu->addChild(okBtn);
        menu->setPosition(winSize / 2);
        this->m_mainLayer->addChild(menu);

        return true;
    }

    void onConfirm(CCObject*) {
        std::string newName = input->getString();
        if (!newName.empty()) {
            IconSetManager::renameSlot(slotIndex, newName);
            if (callback) callback();
        }
        this->onClose(nullptr);
    }

public:
    static RenamePopup* create(int index, std::function<void()> callback) {
        auto ret = new RenamePopup();
        if (ret && ret->initAnchored(260.f, 140.f, index, callback)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class IconPreview : public CCNode {
public:
    static IconPreview* create(const IconSet& set, float scale = 0.35f) {
        auto ret = new IconPreview();
        if (ret && ret->init(set, scale)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool init(const IconSet& set, float scale) {
        if (!CCNode::init()) return false;

        auto gm = GameManager::sharedState();
        float spacing = 19.0f;

        struct IconInfo {
            int type;
            int id;
        };

        std::vector<IconInfo> icons = {
            {0, set.cube},
            {1, set.ship},
            {2, set.ball},
            {3, set.ufo},
            {4, set.wave},
            {5, set.robot},
            {6, set.spider},
            {7, set.swing},
            {8, set.jetpack}
        };

        float totalWidth = spacing * (icons.size() - 1);
        
        for (int i = 0; i < icons.size(); i++) {
            auto icon = SimplePlayer::create(icons[i].id);
            icon->updatePlayerFrame(icons[i].id, (IconType)icons[i].type);
            icon->setColor(gm->colorForIdx(set.col1));
            icon->setSecondColor(gm->colorForIdx(set.col2));
            
            if (set.glowEnabled) {
                icon->setGlowOutline(gm->colorForIdx(set.glowCol));
                icon->enableCustomGlowColor(gm->colorForIdx(set.glowCol));
            }
            
            icon->setScale(scale);
            icon->setPosition({i * spacing - totalWidth / 2, 0});
            this->addChild(icon);
        }

        this->setContentSize({totalWidth, 40});
        return true;
    }
};

class IconSetLayer;

class IconSetCell : public CCNode {
private:
    int slotIndex;
    IconSet iconSet;
    IconSetLayer* parentLayer;

public:
    static IconSetCell* create(int index, const IconSet& set, IconSetLayer* parent);

    bool init(int index, const IconSet& set, IconSetLayer* parent) {
        if (!CCNode::init()) return false;

        slotIndex = index;
        iconSet = set;
        parentLayer = parent;

        this->setContentSize({360, 70});

        auto bg = CCScale9Sprite::create("GJ_square02.png");
        bg->setContentSize(this->getContentSize());
        bg->setPosition(this->getContentSize() / 2);
        bg->setOpacity(100);
        this->addChild(bg);

        auto menu = CCMenu::create();
        menu->setPosition({0, 0});
        this->addChild(menu);

        if (!iconSet.isEmpty) {
            auto loadBtn = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Load", "goldFont.fnt", "GJ_button_01.png", 0.55f),
                this,
                menu_selector(IconSetCell::onLoad)
            );
            loadBtn->setPosition({30, 50});
            menu->addChild(loadBtn);

            auto saveBtn = CCMenuItemSpriteExtra::create(
                ButtonSprite::create("Save", "goldFont.fnt", "GJ_button_04.png", 0.55f),
                this,
                menu_selector(IconSetCell::onSave)
            );
            saveBtn->setPosition({30, 20});
            menu->addChild(saveBtn);

            auto nameInput = TextInput::create(220.f, "Set Name");
            nameInput->setString(iconSet.name);
            nameInput->setPosition({200, 50});
            nameInput->setScale(0.7f);
            nameInput->setCallback([this](const std::string& text) {
                if (!text.empty()) {
                    IconSetManager::renameSlot(slotIndex, text);
                }
            });
            this->addChild(nameInput);

            auto preview = IconPreview::create(iconSet, 0.4f);
            preview->setPosition({200, 20});
            this->addChild(preview);

            auto deleteSprite = CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png");
            deleteSprite->setScale(0.65f);
            auto deleteBtn = CCMenuItemSpriteExtra::create(
                deleteSprite,
                this,
                menu_selector(IconSetCell::onDelete)
            );
            deleteBtn->setPosition({340, 50});
            menu->addChild(deleteBtn);
        }

        return true;
    }

    void onSave(CCObject*);
    void onLoad(CCObject*);
    void onRename(CCObject*);
    void onDelete(CCObject*);
};

class IconSetLayer : public geode::Popup<> {
protected:
    ScrollLayer* scrollLayer = nullptr;
    CCMenu* mainMenu = nullptr;

    bool setup() override {
        auto winSize = this->m_mainLayer->getContentSize();

        this->setTitle("Icon Set Manager");
        this->m_closeBtn->setPositionY(this->m_closeBtn->getPositionY() - 10.f);

        mainMenu = CCMenu::create();
        mainMenu->setPosition({0, 0});
        this->m_mainLayer->addChild(mainMenu);

        auto addBtn = CCMenuItemSpriteExtra::create(
            ButtonSprite::create("Add New Set", "goldFont.fnt", "GJ_button_01.png", 0.8f),
            this,
            menu_selector(IconSetLayer::onAddNew)
        );
        addBtn->setPosition({winSize.width / 2, winSize.height - 50});
        mainMenu->addChild(addBtn);

        refreshList();
        return true;
    }

public:
    void refreshList() {
        if (scrollLayer) {
            scrollLayer->removeFromParentAndCleanup(true);
            scrollLayer = nullptr;
        }

        auto winSize = this->m_mainLayer->getContentSize();
        auto scrollContent = CCLayer::create();
        int count = IconSetManager::getSlotsCount();

        if (count == 0) {
            auto noSetsLabel = CCLabelBMFont::create("No saved sets yet.\nClick 'Add New Set' to save\nyour current icons!", "bigFont.fnt");
            noSetsLabel->setPosition({180, 100});
            noSetsLabel->setScale(0.5f);
            noSetsLabel->setAlignment(kCCTextAlignmentCenter);
            scrollContent->addChild(noSetsLabel);
            scrollContent->setContentSize({360, 200});
        } else {
            float totalHeight = count * 75;
            
            for (int i = 0; i < count; i++) {
                auto cell = IconSetCell::create(i, IconSetManager::getSlot(i), this);
                cell->setPosition({0, totalHeight - (i * 75) - 75});
                scrollContent->addChild(cell);
            }
            
            scrollContent->setContentSize({360, totalHeight});
        }

        float contentHeight = count == 0 ? 200 : count * 75;

        scrollLayer = ScrollLayer::create({360, 180}, true, true);
        scrollLayer->m_contentLayer->addChild(scrollContent);
        scrollLayer->m_contentLayer->setContentSize({360, std::max(180.0f, contentHeight)});
        scrollLayer->setPosition({winSize.width / 2 - 180, 30});
        
        if (count > 0) {
            scrollLayer->scrollToTop();
        }
        
        this->m_mainLayer->addChild(scrollLayer);
    }

    void onAddNew(CCObject*) {
        if (!IconSetManager::canAddMore()) {
            FLAlertLayer::create("Limit Reached", "You have reached the maximum\nnumber of icon sets (50).", "OK")->show();
            return;
        }

        int newSlot = IconSetManager::findNextEmptySlot();
        if (newSlot == -1) {
            FLAlertLayer::create("Error", "No empty slots available.", "OK")->show();
            return;
        }

        IconSetManager::saveToSlot(newSlot, "Set " + std::to_string(newSlot + 1));
        refreshList();
        FLAlertLayer::create("Saved!", "New icon set created!", "OK")->show();
    }

    static IconSetLayer* create() {
        auto ret = new IconSetLayer();
        if (ret && ret->initAnchored(400.f, 300.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

IconSetCell* IconSetCell::create(int index, const IconSet& set, IconSetLayer* parent) {
    auto ret = new IconSetCell();
    if (ret && ret->init(index, set, parent)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void IconSetCell::onSave(CCObject*) {
    IconSetManager::saveToSlot(slotIndex);
    if (parentLayer) {
        parentLayer->refreshList();
    }
    FLAlertLayer::create("Saved!", "Icon set updated!", "OK")->show();
}

void IconSetCell::onLoad(CCObject*) {
    IconSetManager::loadFromSlot(slotIndex);
    if (parentLayer) {
        parentLayer->removeFromParent();
    }
    FLAlertLayer::create("Loaded!", "Icon set applied!", "OK")->show();
}

void IconSetCell::onRename(CCObject*) {
    auto popup = RenamePopup::create(slotIndex, [this]() {
        if (parentLayer) {
            parentLayer->refreshList();
        }
    });
    popup->show();
}

void IconSetCell::onDelete(CCObject*) {
    geode::createQuickPopup(
        "Delete Set",
        "Are you sure you want to\ndelete this icon set?",
        "Cancel", "Delete",
        [this](auto, bool btn2) {
            if (btn2) {
                IconSetManager::deleteSlot(slotIndex);
                if (parentLayer) {
                    parentLayer->refreshList();
                }
                FLAlertLayer::create("Deleted!", "Icon set removed!", "OK")->show();
            }
        }
    );
}

#include <Geode/modify/GJGarageLayer.hpp>
class $modify(IconSetGarageLayer, GJGarageLayer) {
    bool init() {
        if (!GJGarageLayer::init()) return false;

        IconSetManager::loadSavedSets();

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto iconPath = Mod::get()->getResourcesDir() / "icon.png";
        auto iconSprite = CCSprite::create(iconPath.string().c_str());
        
        if (!iconSprite) {
            iconSprite = CCSprite::createWithSpriteFrameName("GJ_sortIcon_001.png");
        }
        
        if (!iconSprite) {
            return true;
        }
        
        iconSprite->setScale(0.5f);

        auto btn = CCMenuItemSpriteExtra::create(
            iconSprite,
            this,
            menu_selector(IconSetGarageLayer::onIconSetsBtn)
        );

        auto menu = CCMenu::create();
        menu->addChild(btn);
        menu->setPosition({50, winSize.height - 50});
        menu->setID("icon-sets-menu"_spr);
        this->addChild(menu, 100);

        return true;
    }

    void onIconSetsBtn(CCObject*) {
        auto layer = IconSetLayer::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(layer, 1000);
    }
};