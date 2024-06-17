#pragma once
#include "Component.h"
class PlayerMove :
    public Component
{
    float moveSpeed;
    DirectX::SimpleMath::Vector3 moveVec;   // 移動方向

    void Input();   // 入力
public:
    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Draw() override;
};

