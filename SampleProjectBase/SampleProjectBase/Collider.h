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
        Num
    };

protected:
    Type type;  // タイプ

    std::vector<Collider*> hitColliders;    // 今フレームで当たった判定
    std::vector<Collider*> o_hitColliders;  // 前フレームで当たった判定

    //bool isHit{false}; // 当たってるかフラグ

    // 当たってない・当たってるときの当たり判定の色
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;

    Collider() = delete;
    virtual ~Collider();
public:
    using Component::Component;
    void Init()override;    // 初期処理をする(Collider継承したクラスは絶対に呼ぶようにする)

    /* void SetIsHit(bool _isHit) { isHit = _isHit; }*/

    // この当たり判定に当たったコライダーを配列に追加する
    void SetHitCollider(Collider& _hitCollider) { hitColliders.push_back(&_hitCollider); }
    void ResetColliders() { hitColliders.clear(); } // 当たってる判定をリセットする

    Type GetType()const { return type; }    // 種類を取得
};

