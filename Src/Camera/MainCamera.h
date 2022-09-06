#pragma once
#include "BaseCamera.h"
class MainCamera :
    public BaseCamera
{
public:
    MainCamera();
    ~MainCamera();

    void Initialize() override;
    void Update() override;

public:


private:


};

