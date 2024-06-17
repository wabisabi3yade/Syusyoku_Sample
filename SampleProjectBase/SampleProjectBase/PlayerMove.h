#pragma once
#include "Component.h"
class PlayerMove :
    public Component
{
    float moveSpeed;
    DirectX::SimpleMath::Vector3 moveVec;   // �ړ�����

    void Input();   // ����
public:
    void Init() override;
    void Update() override;
    void LateUpdate() override;
    void Draw() override;
};

