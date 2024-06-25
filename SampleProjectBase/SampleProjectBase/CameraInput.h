#pragma once
#include "Component.h"

class CameraMove;
// ƒRƒ“ƒgƒ[ƒ‰“ü—Í
class CameraInput :
    public Component
{
    CameraMove* camMove{nullptr};

public:
    using Component::Component;
    void Init()override;
    void Update() override;
};

