#pragma once
#include "ChangeSubScene.h"


class Test_ChangeSubScene
    : public ChangeSubScene
{
public:
    enum TEST_SCENE
    {
        TEST1,
        TEST2,
        TEST3,
        NUM
    };

    Test_ChangeSubScene(SceneMoveInfo* _moveInfo);
    ~Test_ChangeSubScene();

    SubScene_Base* ChangeScene() override;
};

