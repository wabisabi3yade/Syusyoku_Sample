#pragma once
#include "Component.h"

// 描画コンポーネントのベース
class CP_Renderer :
    public Component
{
    virtual void DrawSetup(){};
public:
    using Component::Component;

    virtual void Draw() = 0;
};

