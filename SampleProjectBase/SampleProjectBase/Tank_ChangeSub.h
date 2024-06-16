#pragma once
#include "ChangeSubScene.h"

class Tank_ChangeSub
	 : public ChangeSubScene
{
public:
    enum Scene
    {
        Title,
        InGame,
        Result,
        Num
    };

    Tank_ChangeSub(SceneMoveInfo* _moveInfo);
    ~Tank_ChangeSub();

    SubScene_Base* ChangeScene() override;
};

