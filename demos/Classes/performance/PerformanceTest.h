#ifndef PERFORMANCE_TEST_H
#define PERFORMANCE_TEST_H

#include "Demo.h"

USING_NS_DB;

class PerformanceTest : public DemoBase
{
public:
    CREATE_FUNC(PerformanceTest);

    PerformanceTest();

    virtual std::string title();
    virtual std::string subtitle();
    //virtual void update(float dt) override;

protected:
    virtual void demoInit() override;
    virtual void addInteraction() override{};

    virtual void addNode(Ref* node);
    virtual void subNode(Ref* node);

private:
    Vector<DBCCArmatureNode*> list;
    Label* numLabel;
    MenuItemLabel* addLabel;
    MenuItemLabel* subLabel;
};
#endif  // PerformanceTest_H
