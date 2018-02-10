#include "BaseDemo.h"

class ReplaceSkin : BaseDemo
{
public:
    CREATE_FUNC(ReplaceSkin);

    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = ReplaceSkin::create();

        scene->addChild(layer);
        return scene;
    }

private:
    unsigned _replaceSuitIndex;
    std::vector<std::vector<std::string>> _suitConfigs;
    std::vector<std::string> _replaceSuitParts;
    dragonBones::CCFactory* _factory;
    dragonBones::CCArmatureDisplay* _armatureDisplay;

protected:
    virtual void _onStart()
    {
        _replaceSuitIndex = 0;
        _suitConfigs.push_back({
            "2010600a",
            "2010600a_1",
            "20208003",
            "20208003_1",
            "20208003_2",
            "20208003_3",
            "20405006",
            "20509005",
            "20703016",
            "20703016_1",
            "2080100c",
            "2080100e",
            "2080100e_1",
            "20803005",
            "2080500b",
            "2080500b_1"
        });

        _suitConfigs.push_back({
            "20106010",
            "20106010_1",
            "20208006",
            "20208006_1",
            "20208006_2",
            "20208006_3",
            "2040600b",
            "2040600b_1",
            "20509007",
            "20703020",
            "20703020_1",
            "2080b003",
            "20801015"
        });

        _factory = dragonBones::CCFactory::getFactory();
        _factory->loadDragonBonesData("you_xin/body/body_ske.json");
        _factory->loadTextureAtlasData("you_xin/body/body_tex.json");

        for (size_t i = 0, l = _suitConfigs.size(); i < l; ++i) 
        {
            for (const auto& partArmatureName : _suitConfigs[i]) 
            {
                // you_xin/suit1/2010600a/xxxxxx
                const auto& path = "you_xin/suit" + dragonBones::to_string(i + 1) + "/" + partArmatureName + "/" + partArmatureName;
                const auto& dragonBonesJSONPath = path + "_ske.json";
                const auto& textureAtlasJSONPath = path + "_tex.json";
                //
                _factory->loadDragonBonesData(dragonBonesJSONPath);
                _factory->loadTextureAtlasData(textureAtlasJSONPath);
            }
        }
        //
        _armatureDisplay = _factory->buildArmatureDisplay("body");
        _armatureDisplay->getEventDispatcher()->setEnabled(true);
        _armatureDisplay->getEventDispatcher()->addCustomEventListener(dragonBones::EventObject::LOOP_COMPLETE, std::bind(&ReplaceSkin::_animationEventHandler, this, std::placeholders::_1));
        _armatureDisplay->getAnimation()->play("idle", 0);
        //
        _armatureDisplay->setPosition(0.0f, -200.0f);
        _armatureDisplay->setScale(0.25f);
        addChild(_armatureDisplay);
        // Init the first suit.
        for (const auto& part : _suitConfigs[0]) {
            const auto partArmatureData = _factory->getArmatureData(part);
            _factory->replaceSkin(_armatureDisplay->getArmature(), partArmatureData->defaultSkin);
        }
        //
        const auto listener = cocos2d::EventListenerMouse::create();
        listener->onMouseDown = CC_CALLBACK_1(ReplaceSkin::_mouseDownHandler, this);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
        //
        createText("Touch to replace armature skin.");
    }

private:
    void _mouseDownHandler(cocos2d::EventMouse* event)
    {
        _randomReplaceSkin();
    }

    void _animationEventHandler(cocos2d::EventCustom* event) const
    {
        // Random animation index.
        const auto& animationNames = _armatureDisplay->getAnimation()->getAnimationNames();
        const auto animationIndex = cocos2d::random((size_t)0, animationNames.size() - 1);
        const auto& animationName = animationNames[animationIndex];
        // Play animation.
        _armatureDisplay->getAnimation()->fadeIn(animationName, 0.3f, 0);
    }

    void _randomReplaceSkin()
    {
        // This suit has been replaced, next suit.
        if (_replaceSuitParts.empty()) {
            _replaceSuitIndex++;

            if (_replaceSuitIndex >= _suitConfigs.size()) 
            {
                _replaceSuitIndex = 0;
            }

            // Refill the unset parits.
            for (const auto& partArmatureName : _suitConfigs[_replaceSuitIndex]) 
            {
                _replaceSuitParts.push_back(partArmatureName);
            }
        }

        // Random one part in this suit.
        const auto partIndex = cocos2d::random((size_t)0, _replaceSuitParts.size() - 1);
        const auto& partArmatureName = _replaceSuitParts[partIndex];
        const auto partArmatureData = _factory->getArmatureData(partArmatureName);
        // Replace skin.
        _factory->replaceSkin(_armatureDisplay->getArmature(), partArmatureData->defaultSkin);
        // Remove has been replaced
        _replaceSuitParts.erase(_replaceSuitParts.begin() + partIndex);
    }
};