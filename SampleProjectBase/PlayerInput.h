#pragma once
#include "Component.h"

class PlayerMove;
class PlayerShot;

class PlayerInput :
    public Component
{
    InputClass* input{nullptr};  // ����

    // ���쏈���Ɋ֌W�̂���R���|�[�l���g
    PlayerMove* move{nullptr};   // �ړ�
    PlayerShot* shot{nullptr};   // �e����

public:
    using Component::Component;

    void Init() override;
    void Update()override;
};

