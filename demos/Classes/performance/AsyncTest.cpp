#include "AsyncTest.h"

USING_NS_DB;
const std::string waitingLoadList[] = {
	"armatures/DragonBoy/skeleton.xml", "armatures/DragonBoy/texture.xml", "DragonBoy",
	"armatures/dragon/skeleton.xml", "armatures/dragon/texture.xml", "Dragon",
	"armatures/Knight/skeleton.xml", "armatures/Knight/texture.xml", "Knight",
	"armatures/cyborg/skeleton.xml", "armatures/cyborg/texture.xml", "Cyborg",
	"armatures/cyborg_mix/skeleton.xml", "armatures/cyborg_mix/texture.xml", "Cyborg(AnimationMixing)",
};

const std::string armatureNameList[] = {
	"dragonBoy", 
	"Dragon", 
	"knight",
	"cyborg",
	"cyborg",
};

AsyncTest::AsyncTest()
	:list(0)
	, numLabel(nullptr)
	, addLabel(nullptr)
	, subLabel(nullptr)
	, loadedPos(0)
	, addedPos(0)
	, inLoading(false)
{}

std::string AsyncTest::title()
{
	return "AsyncTest";
}

std::string AsyncTest::subtitle()
{
	return "";
}

void AsyncTest::demoInit()
{
	auto size = VisibleRect::getWinSize();

	// ui
	auto menu = Menu::create();
	auto label = Label::createWithSystemFont("begin load", "Î¢ÈíÑÅºÚ", 80);
	addLabel = MenuItemLabel::create(label, this, menu_selector(AsyncTest::loadModule));
	addLabel->setPosition(size.width / 2, size.height - 500);
	menu->addChild(addLabel);
	menu->setPosition(0, 0);
	addChild(menu);
}


void AsyncTest::onLoadModuleComplete(DragonBonesData *dragonBonesData, ITextureAtlas *textureAtlas)
{
	addNode(nullptr);
	int arrLen = (sizeof(waitingLoadList) / sizeof(waitingLoadList[0]));
	if(loadedPos < arrLen)
	{
		loadSingle();
	}
	else{
		loadedPos = 0;
		addedPos = 0;
		inLoading = false;

	}
}

void AsyncTest::loadSingle()
{
	ArmatureMaterials *mat = new ArmatureMaterials(waitingLoadList[loadedPos], waitingLoadList[loadedPos + 1], waitingLoadList[loadedPos + 2]);
	loadedPos += 3;

	mat->load(CC_CALLBACK_2(AsyncTest::onLoadModuleComplete, this));
}

void AsyncTest::loadModule(Ref* node)
{
	
	if (inLoading == false)
	{
		inLoading = true;
		loadSingle();
	}
}


void AsyncTest::addNode(Ref* node)
{
	auto armatureNode = DBCCFactory::getInstance()->buildArmatureNode(armatureNameList[addedPos]);
	armatureNode->setPosition(480.f + random(-100.f, 100.f), 200.f);
	addChild(armatureNode);
	list.pushBack(armatureNode);

	addedPos++;
}



ArmatureMaterials::ArmatureMaterials(const std::string &dragonBonesFile, const std::string &textureAtlasFile, const std::string &name)
	: dragonBonesFile(dragonBonesFile)
	, textureAtlasFile(textureAtlasFile)
	, name(name)
	, textrueAtlas(nullptr)
	, dragonBonesData(nullptr)
	, dataLoaded(false)
	, atlasLoaded(false)
{
}

ArmatureMaterials::~ArmatureMaterials()
{
	textrueAtlas = nullptr;
	dragonBonesData = nullptr;

}

void ArmatureMaterials::load(const std::function<void(DragonBonesData*, ITextureAtlas*)>& callback)
{
	completeCallBack = callback;
	DBCCFactory::getInstance()->loadDragonBonesDataAsync(dragonBonesFile, name, CC_CALLBACK_1(ArmatureMaterials::onDataAsyncComplete, this));
	DBCCFactory::getInstance()->loadTextureAtlasAsync(textureAtlasFile, name, CC_CALLBACK_1(ArmatureMaterials::onAtlasAsyncComplete, this));

}

void ArmatureMaterials::checkComplete()
{
	if (dataLoaded && atlasLoaded)
	{
		if (completeCallBack != nullptr)
		{
			completeCallBack(dragonBonesData, textrueAtlas);
		}
	}
}

void ArmatureMaterials::onAtlasAsyncComplete(ITextureAtlas *data)
{
	if (data != nullptr)
	{
		textrueAtlas = data;
		atlasLoaded = true;
		checkComplete();
	}
}

void ArmatureMaterials::onDataAsyncComplete(DragonBonesData *data)
{
	if (data != nullptr)
	{
		dragonBonesData = data;
		dataLoaded = true;
		checkComplete();
	}
}