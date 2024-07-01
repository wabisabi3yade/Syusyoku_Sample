#pragma once
#include "Component.h"

class Tag;
class Layer;

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

   // 今フレーム・前フレームで当たった判定
    std::list<Collider*> hitColliders;
    std::list<Collider*> o_hitColliders; 

    // 当たってない・当たってるときの当たり判定の色
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;

    Collider() = delete;    
    virtual ~Collider();
public:
    using Component::Component;
    Collider& operator=(const Collider& _other);

    void Init()override;   
    // 判定に当たったコライダー追加
    void SetHitCollider(Collider& _hitCollider) { hitColliders.push_back(&_hitCollider); }
    // 判定をリセット
    void ResetColliders() { hitColliders.clear(); } 

    Type GetType()const { return type; }    // 種類を取得

private:
#ifdef EDIT
    // これに当たると色を変える
    std::list<Tag> colorTags;   
    std::list<Layer> colorLayers;
#endif // EDIT

public:
    // 色を変えるタグ・レイヤーを設定
    void SetTagColor(const Tag& _tag); 
    void SetLayerColor(const Layer& _layer);
};

