#include "AppDelegate.h"

#include "HelloDragonBones.h"
#include "AnimationBase.h"
#include "DragonBonesEvent.h"
#include "AnimationLayer.h"
#include "BoneOffset.h"
#include "InverseKinematics.h"
#include "BoundingBox.h"
#include "ReplaceSlotDisplay.h"
#include "ReplaceSkin.h"
#include "ReplaceAnimation.h"
#include "CoreElement.h"
#include "PerformanceTest.h"
#include "DragonBonesTest.h"

USING_NS_CC;

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("DragonBones", cocos2d::Rect(0, 0, 1136, 640));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(1136, 640, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.f);
    FileUtils::getInstance()->addSearchPath("assets");

    // create a scene. it's an autorelease object
    // auto scene = HelloDragonBones::createScene();
    // auto scene = AnimationBase::createScene();
    // auto scene = DragonBonesEvent::createScene();
    // auto scene = AnimationLayer::createScene();
    // auto scene = BoneOffset::createScene();
    // auto scene = InverseKinematics::createScene();
    // auto scene = BoundingBox::createScene();
    // auto scene = ReplaceSlotDisplay::createScene();
    // auto scene = ReplaceSkin::createScene();
    // auto scene = ReplaceAnimation::createScene();
    // auto scene = CoreElementGame::createScene();
    // auto scene = PerformanceTest::createScene();

    auto scene = DragonBonesTest::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
