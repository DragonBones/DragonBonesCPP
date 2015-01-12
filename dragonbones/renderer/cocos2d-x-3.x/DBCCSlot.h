#ifndef DBCC_SLOT_H
#define DBCC_SLOT_H

#include "DragonBonesHeaders.h"
#include "cocos2d.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class DBCCArmature;
class DBCCSlot : public Slot
{
protected:
    cocos2d::Node *_nodeDisplay;
    
public:
    virtual cocos2d::Node* getCCDisplay() const;
    virtual DBCCArmature* getCCChildArmature() const;

    virtual cocos2d::Vec2 getGlobalPosition() const;

    virtual void setDisplayImage(cocos2d::Node *display, bool disposeExisting = true);
    
public:
    DBCCSlot(SlotData *slotData);
    virtual ~DBCCSlot();
    virtual void dispose() override;

    virtual Rectangle getBoundingBox() override;
    
protected:
    virtual void updateDisplayColor(int aOffset, int rOffset, int gOffset, int bOffset, float aMultiplier, float rMultiplier, float gMultiplier, float bMultiplier) override;

    virtual int getDisplayZIndex() const override;
    virtual void addDisplayToContainer(void *container, int zIndex) override;
    virtual void removeDisplayFromContainer() override;
    virtual void disposeDisplay() override;
    virtual void disposeDisplayList() override;
    virtual void updateDisplay(void *display) override;
    virtual void updateDisplayBlendMode(BlendMode blendMode) override;
    virtual void updateDisplayVisible(bool visible) override;
    virtual void updateDisplayTransform() override;
    
private:
    DRAGON_BONES_DISALLOW_COPY_AND_ASSIGN(DBCCSlot);
};
NAME_SPACE_DRAGON_BONES_END

#endif  // DBCC_SLOT_H
