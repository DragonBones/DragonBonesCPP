#ifndef TRIM_TEXTURE_TEST_H
#define TRIM_TEXTURE_TEST_H

#include "Demo.h"

USING_NS_DB;

class TrimTextureTest : public DemoBase
{
public:
    CREATE_FUNC(TrimTextureTest);

    TrimTextureTest();

    virtual std::string title();
    virtual std::string subtitle();
    //virtual void update(float dt) override;

protected:
    virtual void demoInit() override;
    virtual void addInteraction() override{};

};
#endif  // TRIM_TEXTURE_TEST_H
