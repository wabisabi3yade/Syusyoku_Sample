#pragma once
#include "Component.h"

// �`��R���|�[�l���g�̃x�[�X
class CP_Renderer :
    public Component
{
    virtual void DrawSetup(){};
public:
    using Component::Component;

    virtual void Draw() = 0;
};

