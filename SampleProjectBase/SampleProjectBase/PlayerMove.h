#pragma once
#include "Component.h"
class PlayerMove :
    public Component
{
    float moveSpeed;
    DirectX::SimpleMath::Vector3 moveVec;   // ˆÚ“®•ûŒü

    void Input();   // “ü—Í
public:
    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Draw() override;
};

