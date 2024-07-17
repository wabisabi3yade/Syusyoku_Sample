#pragma once
#include "Component.h"

class PlayerMove;

class PlayerInput :
    public Component
{
    InputClass* input{nullptr};  // 入力

    // 操作処理に関係のあるコンポーネント
    PlayerMove* move{nullptr};   // 移動

public:
    using Component::Component;

    void Init() override;
    void Update()override;
};

