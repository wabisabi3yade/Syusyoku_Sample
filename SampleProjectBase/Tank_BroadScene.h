#pragma once
#include "BroadScene_Base.h"

class Tank_BroadScene : public BroadScene_Base
{
    // �V�[���J�ڂ̏�����
    void InitChangeScene(SceneMoveInfo* _moveInfo) override;

public:
    Tank_BroadScene(SceneMoveInfo* _pMoveInfo);
    ~Tank_BroadScene();
};

