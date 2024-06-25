#pragma once
#include "Component.h"

class CameraMove;
// コントローラ入力
class CameraInput :
    public Component
{
    CameraMove* camMove{nullptr};

public:
    using Component::Component;
    void Init()override;
    void Update() override;
};

