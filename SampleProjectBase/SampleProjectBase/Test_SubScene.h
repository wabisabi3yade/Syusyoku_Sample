#pragma once
#include "SubScene_Base.h"
#include "Cube.h"

class Test_SubScene :
    public SubScene_Base
{
    std::unique_ptr<Cube> cube;
public:
    Test_SubScene(SceneMoveInfo* _pSceneMoveInfo);
    ~Test_SubScene() override;

    void Update() override;
    void LateUpdate() override;
    void Draw() override;

    void Release() override;
};

