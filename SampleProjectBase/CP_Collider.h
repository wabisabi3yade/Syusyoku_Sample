#pragma once
#include "CloneComponent.h"

class Tag;
class Layer;

// 当たり判定基礎クラス
class CP_Collider : public Component
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

    // 今フレーム・前フレームで当たった判定
    std::list<CP_Collider*> hitColliders;
    std::list<CP_Collider*> o_hitColliders; 

    // 当たってない・当たってるときの当たり判定の色
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
    
public:
    CP_Collider() : type(Type::Num) {}
    CP_Collider(Type _type);
    virtual ~CP_Collider();

    void Start() override;   

    // 判定に当たったコライダー追加
    void SetHitCollider(CP_Collider& _hitCollider) { hitColliders.push_back(&_hitCollider); }

    // 判定をリセット
    void ResetColliders() { hitColliders.clear(); } 

    Type GetType()const { return type; }    // 種類を取得
public:
    // 色を変えるタグ・レイヤーを設定
    void SetTagColor(const Tag& _tag); 
    void SetLayerColor(const Layer& _layer);
};

