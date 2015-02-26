#ifndef ASYNC_TEST_H
#define ASYNC_TEST_H

#include "Demo.h"

USING_NS_DB;

class AsyncTest : public DemoBase
{
public:
	CREATE_FUNC(AsyncTest);

	AsyncTest();

	virtual std::string title();
	virtual std::string subtitle();

	
	//virtual void update(float dt) override;

protected:
	virtual void demoInit() override;
	virtual void addInteraction() override{};

	virtual void addNode(Ref* node);

	virtual void loadModule(Ref* node);

	virtual void loadSingle();

	virtual void onLoadModuleComplete(DragonBonesData *dragonBonesData, ITextureAtlas *textureAtlas);
private:
	Vector<DBCCArmatureNode*> list;
	Label* numLabel;
	MenuItemLabel* addLabel;
	MenuItemLabel* subLabel;



	int loadedPos;
	int addedPos;
	bool inLoading;
};


class ArmatureMaterials
{
public:
	ArmatureMaterials(const std::string &dragonBonesFile, const std::string &textureAtlasFile,const std::string &name);
	~ArmatureMaterials();
	virtual void load(const std::function<void(DragonBonesData*, ITextureAtlas*)>& callback);
private:
	std::string dragonBonesFile;
	std::string textureAtlasFile;
	std::string name;

	bool dataLoaded;
	bool atlasLoaded;

	ITextureAtlas *textrueAtlas;
	DragonBonesData *dragonBonesData;

	std::function<void(DragonBonesData*, ITextureAtlas*)> completeCallBack;
	virtual void onAtlasAsyncComplete(ITextureAtlas *data);
	virtual void onDataAsyncComplete(DragonBonesData *data);
	virtual void checkComplete();
};

#endif  // AsyncTest_H