#include "Cocos2dxFactory.h"
#include "Cocos2dxDisplayBridge.h"
#include "CCNode.h"
#include "CCLayer.h"
#include "Cocos2dxTextureAtlas.h"
#include "Cocos2dxAtlasNode.h"
#include "CCGeometry.h"
#include "XMLDataParser.h"
#include "platform/CCFileUtils.h"
#include "CCNode.h"
#include "ConstValues.h"

namespace dragonBones
{

        /**
         * Creates a new StarlingFactory instance.
         */
    Cocos2dxFactory::Cocos2dxFactory()
        {
        }
        
        /** @private */
        ITextureAtlas* Cocos2dxFactory::generateTextureAtlas(Object *content, TextureAtlasData *textureAtlasRawData)
        {
            return 0;
            /*var texture:Texture;
            var bitmapData:BitmapData;
            if (content is BitmapData)
            {
                bitmapData = content as BitmapData;
                texture = Texture.fromBitmapData(bitmapData, generateMipMaps, optimizeForRenderToTexture);
            }
            else if (content is MovieClip)
            {
                var width:int = getNearest2N(content.width) * scaleForTexture;
                var height:int = getNearest2N(content.height) * scaleForTexture;
                
                _helpMatrix.a = 1;
                _helpMatrix.b = 0;
                _helpMatrix.c = 0;
                _helpMatrix.d = 1;
                _helpMatrix.scale(scaleForTexture, scaleForTexture);
                _helpMatrix.tx = 0;
                _helpMatrix.ty = 0;                
                var movieClip:MovieClip = content as MovieClip;
                movieClip.gotoAndStop(1);
                bitmapData = new BitmapData(width, height, true, 0xFF00FF);
                bitmapData.draw(movieClip, _helpMatrix);
                movieClip.gotoAndStop(movieClip.totalFrames);
                texture = Texture.fromBitmapData(bitmapData, generateMipMaps, optimizeForRenderToTexture, scaleForTexture);
            }
            else
            {
                throw new Error();
            }            
            var textureAtlas:StarlingTextureAtlas = new StarlingTextureAtlas(texture, textureAtlasRawData, false);            
            if (Starling.handleLostContext)
            {
                textureAtlas._bitmapData = bitmapData;
            }
            else
            {
                bitmapData.dispose();
            }
            return textureAtlas;*/
        }
        
        /** @private */
        Armature* Cocos2dxFactory::generateArmature()
        {
            cocos2d::Node *node = cocos2d::Node::create();
            node->setCascadeOpacityEnabled(true);
            return new Armature(new CocosNode(node));
        }
        
        /** @private */
        Slot* Cocos2dxFactory::generateSlot()
        {
            return new Slot(new Cocos2dxDisplayBridge());
        }
        
        /** @private */
        Object* Cocos2dxFactory::generateDisplay(ITextureAtlas *textureAtlas, const String &fullName, Number pivotX, Number pivotY)
        {
            Cocos2dxTextureAtlas *ccTextureAtlas = dynamic_cast<Cocos2dxTextureAtlas*>(textureAtlas);
            cocos2d::Rect rect;
            rect.origin.x = 0;
            rect.origin.y = 0;
            Rectangle region = ccTextureAtlas->getRegion(fullName);
            rect.size.width = region.width;
            rect.size.height = region.height;

            cocos2d::Cocos2dxAtlasNode *atlasNode = cocos2d::Cocos2dxAtlasNode::create(ccTextureAtlas->getTextureAtlas() , ccTextureAtlas->getQuadIndex(fullName) , rect);
            cocos2d::BlendFunc func;
            func.src = GL_SRC_ALPHA;
            func.dst = GL_ONE_MINUS_SRC_ALPHA;
            atlasNode->setBlendFunc(func);
            atlasNode->setCascadeOpacityEnabled(true);
            atlasNode->setAnchorPoint(cocos2d::Point(pivotX / (Number)region.width , (region.height-pivotY) / (Number)region.height));
            atlasNode->setContentSize(cocos2d::Size(region.width , region.height));
            return new CocosNode(atlasNode);
            //static_cast<cocos2d::LayerColor*>(node->node)->initWithColor(cocos2d::ccc4(255,0,0,255) , 100 , 100);
            //return node;
            //DisplayObject *obj = new DisplayObject();
            //obj->textureAtlas = dynamic_cast<Cocos2dxTextureAtlas*>(textureAtlas);
            //obj->atlasNode = cocos2d::Cocos2dxAtlasNode::create(obj->textureAtlas->getTextureAtlas() , obj->textureAtlas->getQuadIndex(fullName));
            //obj->atlasNode->retain();
            //obj->fullName = fullName;
            //obj->pivotX = pivotX;
            //obj->pivotY = pivotY;


            //return obj;
        }

		void Cocos2dxFactory::loadSkeletonFile(const String &skeletonFile , const String &name)
		{
			dragonBones::XMLDataParser parser;
			// 使用XML解析器载入skeleton的xml
			dragonBones::XMLDocument doc;
			auto date = cocos2d::FileUtils::getInstance()->getDataFromFile(skeletonFile);
			doc.Parse(reinterpret_cast<char*>(date.getBytes()), date.getSize());

			// 解析骨骼动画数据
			SkeletonData *skeleton = parser.parseSkeletonData(doc.RootElement());
			
			addSkeletonData(skeleton , name);
		}

		void Cocos2dxFactory::loadTextureAtlasFile(const String &textureAtlasFile , const String &name)
		{
			dragonBones::XMLDataParser parser;
			// 载入皮肤数据
			dragonBones::XMLDocument doc;
			auto date = cocos2d::FileUtils::getInstance()->getDataFromFile(textureAtlasFile);
			doc.Parse(reinterpret_cast<char*>(date.getBytes()), date.getSize());

			int pos = textureAtlasFile.find_last_of("/");
			if (std::string::npos != pos){
				std::string base_path = textureAtlasFile.substr(0, pos + 1);
				
				std::string img_path = doc.RootElement()->Attribute(ConstValues::A_IMAGE_PATH.c_str());
				std::string new_img_path = base_path + img_path;

				doc.RootElement()->SetAttribute(ConstValues::A_IMAGE_PATH.c_str(), new_img_path.c_str());
			}

			// 解析皮肤数据
			TextureAtlasData *textureAtlasData = parser.parseTextureAtlasData(doc.RootElement());
			addTextureAtlas(new dragonBones::Cocos2dxTextureAtlas(textureAtlasData));

		}


		CocosNode::CocosNode(cocos2d::Node *n) : node(n)
		{
			if(node)
			{
				node->retain();
			}
		}

		CocosNode::~CocosNode()
		{
			if(node)
			{
				node->release();
			}
		}

		Cocos2dxFactory * Cocos2dxFactory::msCocos2dxFactory = nullptr;

		Cocos2dxFactory * Cocos2dxFactory::getInstance()
		{
			if (msCocos2dxFactory == nullptr)
			{
				msCocos2dxFactory = new Cocos2dxFactory();
			}
			return msCocos2dxFactory;
		}

		void Cocos2dxFactory::destroyInstance()
		{
			if (msCocos2dxFactory)
			{
				msCocos2dxFactory->dispose();
				CC_SAFE_DELETE(msCocos2dxFactory);
			}
		}

};
