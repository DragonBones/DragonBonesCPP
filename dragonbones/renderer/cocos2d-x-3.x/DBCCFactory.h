#ifndef DBCC_FACTORY_H
#define DBCC_FACTORY_H

#include "DragonBonesHeaders.h"
#include "DBCCRenderHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCFactory : public BaseFactory
{
private:
    static DBCCFactory *_instance;
    
public:

    static DBCCFactory* getInstance();
    static void destroyInstance();
    
    DBCCFactory();
    virtual ~DBCCFactory();

    virtual DBCCArmature* buildArmature(const std::string &armatureName) const override;
    virtual DBCCArmature* buildArmature(const std::string &armatureName, const std::string &dragonBonesName) const override;
    virtual DBCCArmature* buildArmature(const std::string &armatureName, const std::string &skinName, const std::string &animationName, const std::string &dragonBonesName, const std::string &textureAtlasName) const override;
    virtual DBCCArmatureNode* buildArmatureNode(const std::string &armatureName) const;
    virtual DBCCArmatureNode* buildArmatureNode(const std::string &armatureName, const std::string &dragonBonesName) const;
    virtual DBCCArmatureNode* buildArmatureNode(const std::string &armatureName, const std::string &skinName, const std::string &animationName, const std::string &dragonBonesName, const std::string &textureAtlasName) const;
    
    virtual DragonBonesData* loadDragonBonesData(const std::string &dragonBonesFile, const std::string &name = "");
    virtual ITextureAtlas* loadTextureAtlas(const std::string &textureAtlasFile, const std::string &name = "");
	virtual void loadDragonBonesDataAsync(const std::string &dragonBonesFile, const std::string &name,const std::function<void(DragonBonesData*)>& callback);
	virtual void loadTextureAtlasAsync(const std::string &textureAtlasFile, const std::string &name, const std::function<void(ITextureAtlas*)>& callback);

    virtual void refreshTextureAtlasTexture(const std::string &name);
    virtual void refreshAllTextureAtlasTexture();
    virtual bool hasDragonBones(const std::string &skeletonName, const std::string &armatureName = "", const std::string &animationName = "");
    
protected:
    virtual DBCCArmature* generateArmature(const ArmatureData *armatureData) const override;
    virtual DBCCSlot* generateSlot(const SlotData *slotData) const override;
    virtual void* generateDisplay(const ITextureAtlas *textureAtlas, const TextureData *textureData, const DisplayData *displayData) const override;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCFactory);

public:
	struct DBCCAsyncDataStruct
	{
	public:
		DBCCAsyncDataStruct(const std::string& dragonBonesFile, const std::string& name, std::function<void(DragonBonesData*)> f) : 
			file(dragonBonesFile),name(name), callback(f) {}

		std::string file;
		std::string name;
		DragonBonesData* data;
		std::function<void(DragonBonesData*)> callback;
	};

	struct DBCCAsyncAtlasStruct
	{
	public:
		DBCCAsyncAtlasStruct(const std::string& textureAtlasFile,const std::string &name, std::function<void(ITextureAtlas*)> f) : 
			file(textureAtlasFile),name(name), callback(f) {}

		std::string file;
		std::string name;
		ITextureAtlas* data;
		std::function<void(ITextureAtlas*)> callback;
	};

protected:
	std::thread* _loadingThread;

	std::queue<DBCCAsyncDataStruct*>* _dataAsyncStructQueue;
	std::queue<DBCCAsyncAtlasStruct*>* _atlasAsyncStructQueue;
	std::deque<DBCCAsyncDataStruct*>* _dataAsycCompleteQueue;
	std::deque<DBCCAsyncAtlasStruct*>* _atlasAsyncCompleteQueue;


	std::mutex _dataAsyncStructQueueMutex;
	std::mutex _atlasAsyncStructQueueMutex;
	std::mutex _dataAsyncCompleteQueueMutex;
	std::mutex _atlasAsyncCompleteQueueMutex;


	std::mutex _sleepMutex;
	std::condition_variable _sleepCondition;

	bool _needQuit;
	int _asyncRefCount;
private:
	void loadAsyncCallBack(float dt);
	void loadOnSubThread();

};
NAME_SPACE_DRAGON_BONES_END

#endif  // DBCC_FACTORY_H