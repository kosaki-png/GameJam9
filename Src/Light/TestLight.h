#pragma once
#include "BaseLight.h"

class TestLight :
    public BaseLight
{
public:
    TestLight();
    ~TestLight();

    void Initialize() override;
    void Update() override;

private:

};

