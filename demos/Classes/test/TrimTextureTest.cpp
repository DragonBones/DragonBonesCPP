#include "TrimTextureTest.h"

USING_NS_DB;

TrimTextureTest::TrimTextureTest()
{}

std::string TrimTextureTest::title()
{
    return "TrimTextureTest";
}

std::string TrimTextureTest::subtitle()
{
    return "";
}

void TrimTextureTest::demoInit()
{

    auto size = VisibleRect::getWinSize();

    // factory
    auto factory = DBCCFactory::getInstance();
    factory->loadDragonBonesData("armatures/TrimTest/skeleton.xml", "TrimBody");
    factory->loadTextureAtlas("armatures/TrimTest/texture.xml", "originTexture");
    factory->loadTextureAtlas("armatures/TrimTest/texture1.xml", "trimTexture");

    auto originNode = factory->buildArmatureNode("aaa", "", "A", "TrimBody", "originTexture");
    originNode->setPosition(Vec2(size.width / 2 - 100, size.height/2));
    addChild(originNode);
    auto originBox = originNode->getBoundingBox();
    auto originInnerBox = originNode->getInnerBoundingBox();
    auto originBoxNode = LayerColor::create(Color4B(0,0,0,60));
    originBoxNode->setContentSize(originBox.size);
    originBoxNode->setPosition(originBox.origin);
    addChild(originBoxNode);

    auto trimNode = factory->buildArmatureNode("aaa", "", "A", "TrimBody", "trimTexture");
    trimNode->setPosition(Vec2(size.width / 2 + 100, size.height/2));
    addChild(trimNode);
    auto trimBox = trimNode->getBoundingBox();
    auto trimInnerBox = trimNode->getInnerBoundingBox();
    auto trimBoxNode = LayerColor::create(Color4B(0,0,0,60));
    trimBoxNode->setContentSize(trimBox.size);
    trimBoxNode->setPosition(trimBox.origin);
    addChild(trimBoxNode);
    auto a = 1;

}

