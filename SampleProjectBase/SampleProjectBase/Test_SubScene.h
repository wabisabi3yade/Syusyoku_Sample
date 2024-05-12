#pragma once
#include "SubScene_Base.h"

class Triangle;
class Model;

class Test_SubScene :
    public SubScene_Base
{
    Model* cube{nullptr};

public:
    Test_SubScene(SceneMoveInfo* _pSceneMoveInfo);
    ~Test_SubScene() override;

    void Update() override;
    void LateUpdate() override;
    void Draw() override;

    void Release() override;
};

