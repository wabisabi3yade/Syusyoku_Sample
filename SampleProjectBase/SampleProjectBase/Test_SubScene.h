#pragma once
#include "SubScene_Base.h"

class GameObject;

class Test_SubScene :
    public SubScene_Base
{
    GameObject* go {nullptr};
public:
    Test_SubScene(SceneMoveInfo* _pSceneMoveInfo);
    ~Test_SubScene() override;

    void Update() override;
    void LateUpdate() override;
    void Draw() override;

    void Release() override;
};

