#pragma once
#include "Component.h"

class CP_PlayerMove;

class PlayerInput :
    public Component
{
    InputClass* input{nullptr};  // ����

    // ���쏈���Ɋ֌W�̂���R���|�[�l���g
    CP_PlayerMove* move{nullptr};   // �ړ�

public:
    using Component::Component;

    void Init() override;
    void Update()override;
};

