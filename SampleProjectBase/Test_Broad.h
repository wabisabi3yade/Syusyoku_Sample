#pragma once
#include "BroadScene_Base.h"

// テスト用大局クラスのサブシーンまとめ
class Test_Broad :
    public BroadScene_Base
{
    // シーン遷移の初期化
    void InitChangeScene(SceneMoveInfo* _moveInfo) override;

public:
    Test_Broad(SceneMoveInfo* _pMoveInfo);
    ~Test_Broad();

    void Release() override;
};

