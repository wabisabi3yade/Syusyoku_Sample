#pragma once
#include "BroadScene_Base.h"

class Tank_BroadScene : public BroadScene_Base
{
    // ƒV[ƒ“‘JˆÚ‚Ì‰Šú‰»
    void InitChangeScene(SceneMoveInfo* _moveInfo) override;

public:
    Tank_BroadScene(SceneMoveInfo* _pMoveInfo);
    ~Tank_BroadScene();
};

