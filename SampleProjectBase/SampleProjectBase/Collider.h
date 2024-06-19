#pragma once
#include "Component.h"

// 当たり判定基礎クラス
class Collider :
    public Component
{
public :
    enum Type   // 当たり判定の種類
    {
        Box,    // ボックス
        Sphere, // 球
        Num,
        None
    };

protected:
    Type type;  // タイプ
    bool isHit; // 当たってるかフラグ
    // 当たってないときの色
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
public:
    using Component::Component;

    Type GetType()const { return type; }    // 種類を取得
};

