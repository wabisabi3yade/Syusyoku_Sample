#pragma once
#include "Component.h"

class CP_PlayerMove;

class PlayerInput :
    public Component
{
    InputClass* input{nullptr};  // 入力

    // 操作処理に関係のあるコンポーネント
    CP_PlayerMove* move{nullptr};   // 移動

public:
    using Component::Component;

    void Init() override;
    void Update()override;
};

