#include "DBCCFactory.h"
#include "DBCCTextureAtlas.h"
#include "DBCCSlot.h"
#include "DBCCEventDispatcher.h"
#include "DBCCArmature.h"
NAME_SPACE_DRAGON_BONES_BEGIN
DBCCFactory* DBCCFactory::_instance = nullptr;

DBCCFactory* DBCCFactory::getInstance()
{
    if (!_instance)
    {
        _instance = new DBCCFactory();
    }
    return _instance;
}

void DBCCFactory::destroyInstance()
{
    if (_instance)
    {
       CC_SAFE_DELETE(_instance);
    }
}

DBCCFactory::DBCCFactory() 
	:_dataAsyncStructQueue(nullptr)
	, _atlasAsyncStructQueue(nullptr)
	, _dataAsycCompleteQueue(nullptr)
	, _atlasAsyncCompleteQueue(nullptr)
	, _loadingThread(nullptr)
	, _needQuit(false)
	, _asyncRefCount(0)
{
}
DBCCFactory::~DBCCFactory() {}

DBCCArmature* DBCCFactory::buildArmature(const std::string &armatureName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName);
}

DBCCArmature* DBCCFactory::buildArmature(const std::string &armatureName, const std::string &dragonBonesName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName, dragonBonesName);
}

DBCCArmature* DBCCFactory::buildArmature(const std::string &armatureName, const std::string &skinName, const std::string &animationName,
                                         const std::string &dragonBonesName, const std::string &textureAtlasName) const
{
    return (DBCCArmature*) BaseFactory::buildArmature(armatureName, skinName, animationName, dragonBonesName, textureAtlasName);
}

DBCCArmatureNode* DBCCFactory::buildArmatureNode(const std::string &armatureName) const
{
    auto armature = buildArmature(armatureName);
    return DBCCArmatureNode::create(armature);
}

DBCCArmatureNode* DBCCFactory::buildArmatureNode(const std::string &armatureName, const std::string &dragonBonesName) const
{
    auto armature = buildArmature(armatureName, dragonBonesName);
    return DBCCArmatureNode::create(armature);
}

DBCCArmatureNode* DBCCFactory::buildArmatureNode(const std::string &armatureName, const std::string &skinName, const std::string &animationName,
    const std::string &dragonBonesName, const std::string &textureAtlasName) const
{
    auto armature = buildArmature(armatureName, skinName, animationName, dragonBonesName, textureAtlasName);
    return DBCCArmatureNode::create(armature);
}

DragonBonesData* DBCCFactory::loadDragonBonesData(const std::string &dragonBonesFilePath, const std::string &name)
{
    DragonBonesData *existDragonBonesData = getDragonBonesData(name);

    if (existDragonBonesData)
    {
        return existDragonBonesData;
    }

    const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(dragonBonesFilePath);
    if (data.getSize() == 0)
    {
        return nullptr;
    }

    // armature scale
    float scale = cocos2d::Director::getInstance()->getContentScaleFactor();

    // load skeleton.xml using XML parser.
    XMLDocument doc;
    doc.Parse(reinterpret_cast<char*>(data.getBytes()), data.getSize());
    // paser dragonbones skeleton data.
    XMLDataParser parser;
    DragonBonesData *dragonBonesData = parser.parseDragonBonesData(doc.RootElement(), scale);
    addDragonBonesData(dragonBonesData, name);
    return dragonBonesData;
}

ITextureAtlas* DBCCFactory::loadTextureAtlas(const std::string &textureAtlasFile, const std::string &name)
{
    ITextureAtlas *existTextureAtlas = getTextureAtlas(name);

    if (existTextureAtlas)
    {
        refreshTextureAtlasTexture(name.empty() ? existTextureAtlas->textureAtlasData->name : name);
        return existTextureAtlas;
    }

    const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(textureAtlasFile);
    if (data.getSize() == 0)
    {
        return nullptr;
    }

    // textureAtlas scale
    float scale = cocos2d::Director::getInstance()->getContentScaleFactor();

    XMLDocument doc;
    doc.Parse(reinterpret_cast<char*>(data.getBytes()), data.getSize());
    XMLDataParser parser;
    DBCCTextureAtlas *textureAtlas = new DBCCTextureAtlas();
    textureAtlas->textureAtlasData = parser.parseTextureAtlasData(doc.RootElement(), scale);

    int pos = textureAtlasFile.find_last_of("/");

    if (std::string::npos != pos)
    {
        std::string base_path = textureAtlasFile.substr(0, pos + 1);
        textureAtlas->textureAtlasData->imagePath = base_path + textureAtlas->textureAtlasData->imagePath;
    }

    //
    addTextureAtlas(textureAtlas, name);
    refreshTextureAtlasTexture(name.empty() ? textureAtlas->textureAtlasData->name : name);
    return textureAtlas;
}


void DBCCFactory::loadDragonBonesDataAsync(const std::string &dragonBonesFile, const std::string &name, const std::function<void(DragonBonesData*)>& callback)
{
	DragonBonesData *existDragonBonesData = getDragonBonesData(name);

	if (existDragonBonesData)
	{
		callback(existDragonBonesData);
		return;
	}


	// lazy init queue and thread
	if (_dataAsyncStructQueue == nullptr)
	{
		_dataAsyncStructQueue = new std::queue<DBCCAsyncDataStruct*>();
		_dataAsycCompleteQueue = new std::deque<DBCCAsyncDataStruct*>();
		
	}
	if (_loadingThread == nullptr)
	{
		// create a new thread to load dragonBonesData
		_loadingThread = new std::thread(&DBCCFactory::loadOnSubThread, this);
		_needQuit = false;
	}
	
	if (0 == _asyncRefCount)
	{
		cocos2d::Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(DBCCFactory::loadAsyncCallBack, this), this, 0.0f, false, "DBCCFactory::loadAsyncCallBack");
	}

	++_asyncRefCount;

	//make sure the path is absoluted.or else may cause sub thread unsafty when call FileUtils::getInstance()->getDataFromFile
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(dragonBonesFile);
	// generate async struct
	DBCCAsyncDataStruct *dataStruct = new (std::nothrow) DBCCAsyncDataStruct(fullPath, name, callback);

	// add async struct into queue
	_dataAsyncStructQueueMutex.lock();
	_dataAsyncStructQueue->push(dataStruct);
	_dataAsyncStructQueueMutex.unlock();

	_sleepCondition.notify_one();

}

void DBCCFactory::loadTextureAtlasAsync(const std::string &textureAtlasFile, const std::string &name, const std::function<void(ITextureAtlas*)>& callback){
	ITextureAtlas *existTextureAtlas = getTextureAtlas(name);
	if (existTextureAtlas)
	{
		//TODO: refreshTextureAtlasTexture in here should be asynchronous too 
		refreshTextureAtlasTexture(name.empty() ? existTextureAtlas->textureAtlasData->name : name);
		callback(existTextureAtlas);
		return;
	}


	// lazy init queue and thread
	if (_atlasAsyncStructQueue == nullptr)
	{
		_atlasAsyncStructQueue = new std::queue<DBCCAsyncAtlasStruct*>();
		_atlasAsyncCompleteQueue = new std::deque<DBCCAsyncAtlasStruct*>();
	}
	if (_loadingThread == nullptr)
	{
		// create a new thread to load dragonBonesData
		_loadingThread = new std::thread(&DBCCFactory::loadOnSubThread, this);
		_needQuit = false;
	}

	if (0 == _asyncRefCount)
	{
		cocos2d::Director::getInstance()->getScheduler()->schedule(CC_CALLBACK_1(DBCCFactory::loadAsyncCallBack, this), this, 0.0f, false, "DBCCFactory::loadAsyncCallBack");
	}

	++_asyncRefCount;

	//make sure the path is absoluted.or else may cause sub thread unsafty when call FileUtils::getInstance()->getDataFromFile
	std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(textureAtlasFile);
	// generate async struct
	DBCCAsyncAtlasStruct *atlasStruct = new (std::nothrow) DBCCAsyncAtlasStruct(fullPath, name, callback);

	// add async struct into queue
	_atlasAsyncStructQueueMutex.lock();
	_atlasAsyncStructQueue->push(atlasStruct);
	_atlasAsyncStructQueueMutex.unlock();

	_sleepCondition.notify_one();

}

void DBCCFactory::loadAsyncCallBack(float dt){
	//check dragonBonesData complete queue
	if (_dataAsycCompleteQueue != nullptr)
	{
		_dataAsyncCompleteQueueMutex.lock();
		if (_dataAsycCompleteQueue->empty())
		{
			_dataAsyncCompleteQueueMutex.unlock();
		}
		else
		{
			DBCCAsyncDataStruct *asyncStruct = _dataAsycCompleteQueue->front();
			_dataAsycCompleteQueue->pop_front();
			_dataAsyncCompleteQueueMutex.unlock();

			addDragonBonesData(asyncStruct->data, asyncStruct->name);

			if (asyncStruct->callback)
			{
				asyncStruct->callback(asyncStruct->data);
			}
			delete asyncStruct;

			--_asyncRefCount;
		}
	}
	

	//check atlas complete queue
	if (_atlasAsyncCompleteQueue != nullptr)
	{
		_atlasAsyncCompleteQueueMutex.lock();
		if (_atlasAsyncCompleteQueue->empty())
		{
			_atlasAsyncCompleteQueueMutex.unlock();
		}
		else
		{
			DBCCAsyncAtlasStruct *asyncStruct = _atlasAsyncCompleteQueue->front();
			_atlasAsyncCompleteQueue->pop_front();
			_atlasAsyncCompleteQueueMutex.unlock();

			addTextureAtlas(asyncStruct->data, asyncStruct->name);
			//TODO: refreshTextureAtlasTexture in here should be asynchronous too 
			refreshTextureAtlasTexture(asyncStruct->name.empty() ? asyncStruct->data->textureAtlasData->name : asyncStruct->name);

			if (asyncStruct->callback)
			{
				asyncStruct->callback(asyncStruct->data);
			}
			delete asyncStruct;

			--_asyncRefCount;
		}
	}
	


	if (0 == _asyncRefCount)
	{
		cocos2d::Director::getInstance()->getScheduler()->unschedule("DBCCFactory::loadAsyncCallBack", this);
	}
}

void DBCCFactory::loadOnSubThread()
{
	while (true)
	{
		DBCCAsyncDataStruct *dataStruct = nullptr;
		DBCCAsyncAtlasStruct *atlasStruct = nullptr;
		std::queue<DBCCAsyncDataStruct*> *pDataQueue = _dataAsyncStructQueue;
		std::queue<DBCCAsyncAtlasStruct*> *pAtlasQueue = _atlasAsyncStructQueue;

		bool dataStuctEmpty = false;
		bool atlasStuctEmpty = false;
		//------------check data queue---------------------
		if (pDataQueue != nullptr)
		{
			_dataAsyncStructQueueMutex.lock();
			if (pDataQueue->empty())
			{
				_dataAsyncStructQueueMutex.unlock();
				dataStuctEmpty = true;
			}
			else
			{
				dataStruct = pDataQueue->front();
				pDataQueue->pop();
				_dataAsyncStructQueueMutex.unlock();
				dataStuctEmpty = false;
			}
		}
		

		//parse dragonBonesData
		if (dataStruct != nullptr)
		{
			const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(dataStruct->file);
			if (data.getSize() == 0)
			{
				//callback(nullptr);
				dataStruct->data = nullptr;
			}
			else{
				// armature scale
				float scale = cocos2d::Director::getInstance()->getContentScaleFactor();

				// load skeleton.xml using XML parser.
				XMLDocument doc;
				doc.Parse(reinterpret_cast<char*>(data.getBytes()), data.getSize());
				// paser dragonbones skeleton data.
				XMLDataParser parser;
				DragonBonesData *dragonBonesData = parser.parseDragonBonesData(doc.RootElement(), scale);
				dataStruct->data = dragonBonesData;
			}

			_dataAsyncCompleteQueueMutex.lock();
			_dataAsycCompleteQueue->push_back(dataStruct);
			_dataAsyncCompleteQueueMutex.unlock();
		}

		//------------check atlas queue---------------------
		if (pAtlasQueue != nullptr)
		{
			_atlasAsyncStructQueueMutex.lock();
			if (pAtlasQueue->empty())
			{
				_atlasAsyncStructQueueMutex.unlock();
				atlasStuctEmpty = true;
			}
			else
			{
				atlasStruct = pAtlasQueue->front();
				pAtlasQueue->pop();
				_atlasAsyncStructQueueMutex.unlock();
				atlasStuctEmpty = false;
			}
		}
		

		if (atlasStruct != nullptr)
		{
			const auto &data = cocos2d::FileUtils::getInstance()->getDataFromFile(atlasStruct->file);
			if (data.getSize() == 0)
			{
				atlasStruct->data = nullptr;
			}

			// textureAtlas scale
			float scale = cocos2d::Director::getInstance()->getContentScaleFactor();

			XMLDocument doc;
			doc.Parse(reinterpret_cast<char*>(data.getBytes()), data.getSize());
			XMLDataParser parser;
			DBCCTextureAtlas *textureAtlas = new DBCCTextureAtlas();
			textureAtlas->textureAtlasData = parser.parseTextureAtlasData(doc.RootElement(), scale);

			int pos = atlasStruct->file.find_last_of("/");

			if (std::string::npos != pos)
			{
				std::string base_path = atlasStruct->file.substr(0, pos + 1);
				textureAtlas->textureAtlasData->imagePath = base_path + textureAtlas->textureAtlasData->imagePath;
				atlasStruct->data = textureAtlas;
			}
			_atlasAsyncCompleteQueueMutex.lock();
			_atlasAsyncCompleteQueue->push_back(atlasStruct);
			_atlasAsyncCompleteQueueMutex.unlock();
		}

		//check need quit
		if (dataStuctEmpty && atlasStuctEmpty)
		{
			if (_needQuit) {
				break;
			}
			else {
				std::unique_lock<std::mutex> lk(_sleepMutex);
				_sleepCondition.wait(lk);
				continue;
			}
		}
		
	}

	//clean up before quit thread
	if (_dataAsyncStructQueue != nullptr){delete _dataAsyncStructQueue;_dataAsyncStructQueue = nullptr;}
	if (_dataAsycCompleteQueue != nullptr){delete _dataAsycCompleteQueue;_dataAsycCompleteQueue = nullptr;}
	if (_atlasAsyncStructQueue != nullptr){delete _atlasAsyncStructQueue;_atlasAsyncStructQueue = nullptr;}
	if (_atlasAsyncCompleteQueue != nullptr){delete _atlasAsyncCompleteQueue;_atlasAsyncCompleteQueue = nullptr;}
	
}

void DBCCFactory::refreshTextureAtlasTexture(const std::string &name)
{
    for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
    {
        DBCCTextureAtlas *textureAtlas = static_cast<DBCCTextureAtlas*>(iterator->second);
        const TextureAtlasData *textureAtlasData = textureAtlas->textureAtlasData;

        if (iterator->first == name)
        {
            textureAtlas->reloadTexture();
        }
    }
}

void DBCCFactory::refreshAllTextureAtlasTexture()
{
    for (auto iterator = _textureAtlasMap.begin(); iterator != _textureAtlasMap.end(); ++iterator)
    {
        DBCCTextureAtlas *textureAtlas = static_cast<DBCCTextureAtlas*>(iterator->second);
        const TextureAtlasData *textureAtlasData = textureAtlas->textureAtlasData;
        textureAtlas->reloadTexture();
    }
}

bool DBCCFactory::hasDragonBones(const std::string &skeletonName, const std::string &armatureName, const std::string &animationName)
{
    auto dragonbonesData = getDragonBonesData(skeletonName);

    if (!dragonbonesData) { return false; }

    if (!armatureName.empty())
    {
        auto armatureData = dragonbonesData->getArmatureData(armatureName);

        if (!armatureData) { return false; }

        if (!animationName.empty())
        {
            auto animationData = armatureData->getAnimationData(animationName);
            return animationData != nullptr;
        }
    }

    return true;
}

DBCCArmature* DBCCFactory::generateArmature(const ArmatureData *armatureData) const
{
    Animation *animation = new Animation();
    // sprite
    auto display = cocos2d::Node::create();
    display->setCascadeColorEnabled(true);
    display->setCascadeOpacityEnabled(true);
    display->retain();
    // eventDispatcher
    DBCCEventDispatcher *eventDispatcher = new DBCCEventDispatcher();
    eventDispatcher->eventDispatcher = new cocos2d::EventDispatcher();
    eventDispatcher->eventDispatcher->setEnabled(true);
    // armature
    return new DBCCArmature((ArmatureData*)(armatureData), animation, eventDispatcher, display);
}

DBCCSlot* DBCCFactory::generateSlot(const SlotData *slotData) const
{
    return new DBCCSlot((SlotData*)(slotData));
}

void* DBCCFactory::generateDisplay(const ITextureAtlas *textureAtlas, const TextureData *textureData, const DisplayData *displayData) const
{
    DBCCTextureAtlas *dbccTextureAtlas = (DBCCTextureAtlas*)(textureAtlas);

    if (!dbccTextureAtlas || !textureData) return nullptr;

    auto texture = dbccTextureAtlas->getTexture();
    assert(texture);

    const float x = textureData->region.x;
    const float y = textureData->region.y;
    const bool rotated = textureData->rotated;
    const float width = rotated ? textureData->region.height : textureData->region.width;
    const float height = rotated ? textureData->region.width : textureData->region.height;
    cocos2d::Rect rect(x, y, width, height);
    cocos2d::Vec2 offset;
    cocos2d::Size originSize(width, height);

    if (textureData->frame)
    {
        float px = -textureData->frame->x;
        float py = -textureData->frame->y;
        originSize.width = textureData->frame->width;
        originSize.height = textureData->frame->height;
        // offset = sprite center - trimed texture center
        float cx1 = px + rect.size.width / 2;
        float cy1 = originSize.height - py - rect.size.height / 2;
        float cx2 = originSize.width / 2;
        float cy2 = originSize.height / 2;
        offset.x = cx2 - cx1;
        offset.y = cy2 - cy1;
    }
    // sprite
    auto spriteFrame = cocos2d::SpriteFrame::createWithTexture(texture, rect,
        textureData->rotated, offset, originSize);
    cocos2d::Node *display = cocos2d::Sprite::createWithSpriteFrame(spriteFrame);
    display->setCascadeColorEnabled(true);
    display->setCascadeOpacityEnabled(true);
    display->retain();
    float pivotX = 0;
    float pivotY = 0;

    if (displayData)
    {
        pivotX = displayData->pivot.x;
        pivotY = displayData->pivot.y;
    }

    display->setAnchorPoint(cocos2d::Vec2(pivotX / originSize.width, 1.f - pivotY / originSize.height));
    display->setContentSize(originSize);
    return display;
}
NAME_SPACE_DRAGON_BONES_END
