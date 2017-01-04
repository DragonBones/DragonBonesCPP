#ifndef __REPLACE_SKIN_H__
#define __REPLACE_SKIN_H__

#include "cocos2d.h"
#include "dragonBones/cocos2dx/CCDragonBonesHeaders.h"
#include "rapidjson/document.h"

class ReplaceSkin : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(ReplaceSkin);

private:
    rapidjson::Document _doc;
    std::map<std::string, std::vector<std::vector<std::string>>> _skinData;

    dragonBones::DragonBonesData* _dragonBonesData;
    dragonBones::Armature* _armature;
    dragonBones::CCArmatureDisplay* _armatureDisplay;
    dragonBones::CCFactory _factory;

    void _initArmature();
    void _initAnimation();

    void _parseSkinData();
    const std::vector<std::vector<std::string>> getResNamesDataByIDWithAll(std::string key);
    const std::vector<std::string> getResNamesDataByID(std::string idStr);
    void setAll(const std::vector<std::vector<std::string>>& skin);
    void setHead(const std::vector<std::string>& skin);
    void setCloth(const std::vector<std::string>& skin);
    void setPants(const std::vector<std::string>& skin);
    void setShoe(const std::vector<std::string>& skin);
    void setNewSlot(const char* slotName, std::string textureName);

    void itemClicked(cocos2d::Ref* sender);
};

#endif // __REPLACE_SKIN_H__
