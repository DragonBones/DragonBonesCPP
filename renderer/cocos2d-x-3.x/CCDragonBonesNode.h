//
//  CCDragonBones.h
//
//
//  Created by Wayne Dimart on 14-4-18.
//  Copyright (c) 2014年 . All rights reserved.
//

#ifndef __quickcocos2dx__CCDragonBones__
#define __quickcocos2dx__CCDragonBones__

#include <iostream>
#include "cocos2d.h"
#include "preDB.h"
#include "Animation.h"
#include "Event.h"
namespace cocos2d 
{
    class Armature;
    class DragonBonesNode : public Node
	{
    public:
        static dragonBones::Armature*                       buildArmature(const char* skeletonXMLFile,
                                                             const char* textureXMLFile,
                                                             const char* dragonBonesName,
                                                             const char* armatureName,
                                                             const char* animationName = "");
        static DragonBonesNode*                  create(dragonBones::Armature*arm);

        static DragonBonesNode*                  create(
															const char* skeletonXMLFile,
															const char* dragonBonesName,
															const char* armatureName); 
															 
        static DragonBonesNode*                  create(
															const char* skeletonXMLFile,
															const char* textureXMLFile,
															const char* dragonBonesName,
                                                            const char* armatureName,
                                                            const char* animationName = "");

        Node*                                getDisplayNode();
        dragonBones::Armature*                              getArmature();
        virtual void                           onExit(); 
        void                                   gotoAndPlay(
                                                             const std::string &animationName,
                                                             float fadeInTime = -1,
                                                             float duration = -1,
                                                             float loop = dragonBones::NaN,
                                                             unsigned int layer = 0,
                                                             const std::string &group = "",
                                                             const std::string &fadeOutMode = dragonBones::Animation::SAME_LAYER_AND_GROUP,
                                                             bool displayControl = true,
                                                             bool pauseFadeOut = true,
                                                             bool pauseFadeIn = true
                                                             );
 
        void                                   addEventListener(
                                                                const std::string &type, 
                                                                const std::string &key,
																Ref*pObj,
																SEL_CallFuncND callback); 
		void addEventListener(const std::string &type, dragonBones::EventDispatcher::Function listener , const std::string &key = "");
		bool								   hasEventListener(const std::string &type);
		void								   removeEventListener(const std::string &type, const std::string &key);
		void								   dispatchEvent(dragonBones::Event *event);

        
    private:
        void                                   initWithArmature(dragonBones::Armature*arm);
        void                                   update(float dt);
        dragonBones::Armature*                              m_Armature; 
    }; 
}
#endif /* defined(__quickcocos2dx__CCDragonBones__) */
