#pragma once
#include "Component.h"

class CameraMove;
// �R���g���[������
class CameraInput :
    public Component
{
    CameraMove* camMove{nullptr};

public:
    using Component::Component;
    void Init()override;
    void Update() override;
};

