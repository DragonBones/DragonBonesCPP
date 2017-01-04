#include "ReplaceSkin.h"

USING_NS_CC;

Scene* ReplaceSkin::createScene()
{
    auto scene = Scene::create();
    auto layer = ReplaceSkin::create();

    scene->addChild(layer);
    return scene;
}

bool ReplaceSkin::init()
{
    if ( !LayerColor::initWithColor(cocos2d::Color4B(105, 105, 105, 255)))
    {
        return false;
    }

    _armature = nullptr;
    _armatureDisplay = nullptr;

    // Load DragonBones Data.
    _dragonBonesData = _factory.loadDragonBonesData("ReplaceSkin/man.json");
    _factory.loadTextureAtlasData("ReplaceSkin/texture.json");

    if (_dragonBonesData)
    {
        _initArmature();
        _initAnimation();
    }
    else
    {
        assert(false);
    }
    
    // load data
    _parseSkinData();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("ReplaceSkin/skin.plist");

    // init buttons
    auto menu = Menu::create();
    for (int i = 0; i < 4; i++) {
        char resName[256] = {0};
        sprintf(resName, "ReplaceSkin/all%d.png", i + 1);
        auto item = cocos2d::MenuItemImage::create(resName, resName, CC_CALLBACK_1(ReplaceSkin::itemClicked, this));
        item->setPosition(Vec2(100 + (i % 2) * 750, 550 - (i / 2) * 400));
        menu->addChild(item, i+1, i+1);
    }
    menu->setPosition(Vec2(0,0));
    this->addChild(menu);

    const auto text = cocos2d::Label::create();
    text->setPosition(cocos2d::Director::getInstance()->getVisibleSize().width * 0.5f, 60.f);
    text->setString("Click button to replace skin.");
    text->setAlignment(cocos2d::TextHAlignment::CENTER);
    this->addChild(text);

    return true;
}

void ReplaceSkin::_initArmature()
{
    // a. Build Armature Display. (buildArmatureDisplay will advanceTime animation by Armature Display)
    _armatureDisplay = _factory.buildArmatureDisplay("man");
    _armature = _armatureDisplay->getArmature();

    // Add Armature Display.
    const auto center = cocos2d::Director::getInstance()->getVisibleSize();
    _armatureDisplay->setPosition(center.width * 0.5f, center.height * 0.5f - 200);
    this->addChild(_armatureDisplay);
}

void ReplaceSkin::_initAnimation()
{
    // Play Animation.
    _armatureDisplay->getAnimation().play("runFront", 0);
}

void ReplaceSkin::_parseSkinData()
{
    auto rawData = FileUtils::getInstance()->getStringFromFile("ReplaceSkin/ele.json");
    _doc.Parse(rawData.c_str());

    char key[4] = {0};
    for (int i = 0; i < 4; i++)
    {
        sprintf(key, "all%d", i + 1);
        std::vector<std::vector<std::string>> ret = getResNamesDataByIDWithAll(key);
        _skinData[key] = ret;
    }
}

const std::vector<std::vector<std::string>> ReplaceSkin::getResNamesDataByIDWithAll(std::string key)
{
    std::vector<std::vector<std::string>> ret;
    
    const rapidjson::Value& _all = _doc["all"];
    for (rapidjson::Value::ConstValueIterator itr = _all.Begin(); itr != _all.End(); ++itr)
    {
        std::string checkId = (*itr)["id"].GetString();
        if (strcmp(checkId.c_str(), key.c_str()) == 0)
        {
            auto tempResNames = (*itr)["resNames"].GetArray();
            for (rapidjson::Value::ConstValueIterator itr2 = tempResNames.Begin(); itr2 != tempResNames.End(); ++itr2)
            {
                ret.push_back(getResNamesDataByID(itr2->GetString()));
            }
            break;
        }
    }

    return ret;
}

const std::vector<std::string> ReplaceSkin::getResNamesDataByID(std::string idStr)
{
    std::vector<std::string> ret;
    //    auto _eleData = ;
    const rapidjson::Value& _ele = _doc["eles"];
    for (rapidjson::Value::ConstValueIterator itr = _ele.Begin(); itr != _ele.End(); ++itr)
    {
        std::string checkId = (*itr)["id"].GetString();
        if (strcmp(checkId.c_str(), idStr.c_str()) == 0)
        {
            auto array = (*itr)["resNames"].GetArray();
            for (rapidjson::Value::ConstValueIterator itr2 = array.Begin(); itr2 != array.End(); ++itr2)
            {
                ret.push_back(itr2->GetString());
            }
            break;
        }
    }
    
    return ret;
}

void ReplaceSkin::setAll(const std::vector<std::vector<std::string>>& skin)
{
    setHead(skin.at(0));
    setCloth(skin.at(1));
    setPants(skin.at(2));
    setShoe(skin.at(3));
}

void ReplaceSkin::setHead(const std::vector<std::string>& skin)
{
    setNewSlot("Atoufa", skin.at(0));
}

void ReplaceSkin::setCloth(const std::vector<std::string>& skin)
{
    setNewSlot("Ayifu", skin.at(0));
    setNewSlot("Axiuzi11", skin.at(1));
    setNewSlot("Axiuzi21", skin.at(2));
    setNewSlot("Axiuzi12", skin.at(3));
    setNewSlot("Axiuzi22", skin.at(4));
}

void ReplaceSkin::setPants(const std::vector<std::string>& skin)
{
    setNewSlot("Akuzi", skin.at(0));
    setNewSlot("Akuzi12", skin.at(1));
    setNewSlot("Akuzi22", skin.at(2));
    setNewSlot("Akuzi11", skin.at(3));
    setNewSlot("Akuzi21", skin.at(4));
}

void ReplaceSkin::setShoe(const std::vector<std::string>& skin)
{
    setNewSlot("Axiezi1", skin.at(0));
    setNewSlot("Axiezi2", skin.at(1));
}

void ReplaceSkin::setNewSlot(const char* slotName, std::string textureName)
{
    CCLOG("slot : %s, texture : %s", slotName, textureName.c_str());
    auto slot = _armature->getSlot(slotName);
    auto display = dragonBones::DBCCSprite::create();
    if (textureName.length() > 0)
    {
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureName);
        display->setSpriteFrame(frame);
    }
    slot->setDisplay(display, dragonBones::DisplayType::Image);
}

void ReplaceSkin::itemClicked(cocos2d::Ref* sender)
{
    int tag = dynamic_cast<MenuItemImage*>(sender)->getTag();
    CCLOG("all %d clicked\n-----", tag);
    char key[4] = {0};
    sprintf(key, "all%d", tag);
    auto skin = _skinData[key];
    setAll(skin);
    CCLOG("-----");
}
