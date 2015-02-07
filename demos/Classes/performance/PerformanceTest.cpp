#include "PerformanceTest.h"

USING_NS_DB;

PerformanceTest::PerformanceTest()
    :list(0)
    ,numLabel(nullptr)
    ,addLabel(nullptr)
    ,subLabel(nullptr)
{}

std::string PerformanceTest::title()
{
    return "PerformanceTest";
}

std::string PerformanceTest::subtitle()
{
    return "";
}

void PerformanceTest::demoInit()
{

    auto size = VisibleRect::getWinSize();

    // ui
    auto menu = Menu::create();
    auto label = Label::createWithSystemFont("-10", "Î¢ÈíÑÅºÚ", 80);
    subLabel = MenuItemLabel::create(label, this, menu_selector(PerformanceTest::subNode));
    subLabel->setPosition(60, size.height / 2);
    label = Label::createWithSystemFont("+10", "Î¢ÈíÑÅºÚ", 80);
    addLabel = MenuItemLabel::create(label, this, menu_selector(PerformanceTest::addNode));
    addLabel->setPosition(size.width - 60, size.height / 2);
    menu->addChild(addLabel);
    menu->addChild(subLabel);
    menu->setPosition(0, 0);
    addChild(menu);

    numLabel = Label::createWithSystemFont("", "Î¢ÈíÑÅºÚ", 26);
    numLabel->setPosition(size.width / 2, size.height - 90);
    addChild(numLabel);
    
    // factory
    DBCCFactory::getInstance()->loadDragonBonesData("armatures/DragonBoy/skeleton.xml", "DragonBoy");
    DBCCFactory::getInstance()->loadTextureAtlas("armatures/DragonBoy/texture.xml", "DragonBoy");

    addNode(nullptr);
}

void PerformanceTest::addNode(Ref* node)
{
    for (int i = 0; i < 10; i++)
    {
        auto node = DBCCFactory::getInstance()->buildArmatureNode("dragonBoy");
        node->setPosition(480.f, 200.f);
        addChild(node);
        list.pushBack(node);
    }
    auto str = CCString::createWithFormat("now count: %d", list.size());
    numLabel->setString(str->getCString());
}

void PerformanceTest::subNode(Ref* node)
{
    if (list.size() >= 10)
    {
        for (int i = 0; i < 10; i++)
        {
            list.back()->removeFromParent();
            list.popBack();
        }
        auto str = CCString::createWithFormat("now count: %d", list.size());
        numLabel->setString(str->getCString());
    }
}
