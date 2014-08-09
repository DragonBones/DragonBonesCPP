#ifndef __DBCC_ARMATURE_NODE_H__
#define __DBCC_ARMATURE_NODE_H__

#include "DragonBonesHeaders.h"
#include "cocos2d.h"
#include "DBCCSlot.h"
#include "DBCCArmature.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmatureNode : public cocos2d::Node
{
public:
    virtual cocos2d::Node *getCCDisplay() const { return _armature->getCCDisplay(); };
    virtual cocos2d::EventDispatcher *getCCEventDispatcher() const { return _armature->getCCEventDispatcher(); };
    virtual cocos2d::Rect getBoundingBox() const override;
    
public:

    static DBCCArmatureNode *create(DBCCArmature *armature);
    virtual bool initWithDBCCArmature(DBCCArmature *armature);
    
    DBCCArmatureNode();
    virtual ~DBCCArmatureNode();
    DBCCArmature *getArmature() const { return _armature; };
    Animation *getAnimation() const { return _armature->getAnimation(); };
    
protected:
    DBCCArmature *_armature;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCArmatureNode);
};
NAME_SPACE_DRAGON_BONES_END
#endif  // __DBCC_ARMATURE_NODE_H__
