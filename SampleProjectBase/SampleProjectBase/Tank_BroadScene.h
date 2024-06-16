#pragma once
#include "BroadScene_Base.h"

class Tank_BroadScene : public BroadScene_Base
{
    // シーン遷移の初期化
    void InitChangeScene(SceneMoveInfo* _moveInfo) override;

public:
    Tank_BroadScene(SceneMoveInfo* _pMoveInfo);
    ~Tank_BroadScene();
};

