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
namespace dragonBones {
    class Armature;
    class CCDragonBones:public cocos2d::CCNode {
    public:
        static Armature*                       buildArmature(const char* skeletonXMLFile,
                                                             const char* textureXMLFile,
                                                             const char* dragonBonesName,
                                                             const char* armatureName,
                                                             const char* animationName = "");
        static CCDragonBones*                  create(Armature*arm);

        static CCDragonBones*                  create(
															const char* skeletonXMLFile,
															const char* dragonBonesName,
															const char* armatureName); 
															 
        static CCDragonBones*                  create(
															const char* skeletonXMLFile,
															const char* textureXMLFile,
															const char* dragonBonesName,
                                                            const char* armatureName,
                                                            const char* animationName = "");

        CCNode*                                getDisplayNode();
        Armature*                              getArmature();
        virtual void                           onExit(); 
        void                                   gotoAndPlay(
                                                             const String &animationName,
                                                             Number fadeInTime = -1,
                                                             Number duration = -1,
                                                             Number loop = NaN,
                                                             uint layer = 0,
                                                             const String &group = "",
                                                             const String &fadeOutMode = Animation::SAME_LAYER_AND_GROUP,
                                                             bool displayControl = true,
                                                             bool pauseFadeOut = true,
                                                             bool pauseFadeIn = true
                                                             );
        void                                   addEventListener(
                                                                const String &type,
                                                                EventDispatcher::Function listener,
                                                                const String &key);
        
    private:
        void                                   initWithArmature(Armature*arm);
        void                                   update(float dt);
        Armature*                              m_Armature;
        
    };
    
}
#endif /* defined(__quickcocos2dx__CCDragonBones__) */
