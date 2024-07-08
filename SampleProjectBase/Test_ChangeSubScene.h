#pragma once
#include "ChangeSubScene.h"


class Test_ChangeSubScene
    : public ChangeSubScene
{
public:
    enum Scene
    {
        Test1,
        Test2,
        Test3,
        Num
    };

    Test_ChangeSubScene(SceneMoveInfo* _moveInfo);
    ~Test_ChangeSubScene();

    SubScene_Base* ChangeScene() override;
};

