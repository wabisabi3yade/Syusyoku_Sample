#pragma once
#include "CloneComponent.h"

#include <btBulletDynamicsCommon.h>
#include "DXToBullet.h"

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

    /// @brief コリジョンの形状
    std::unique_ptr<btCollisionShape> pCollisionShape;

    // 当たってない・当たってるときの当たり判定の色
    static const DirectX::SimpleMath::Color normalColor;
    static const DirectX::SimpleMath::Color hitColor;
    
public:
    CP_Collider() : type(Type::Num) {}
    CP_Collider(Type _type);
    CP_Collider(const CP_Collider& _other);
    virtual ~CP_Collider();

    CP_Collider& operator=(const CP_Collider& _other);

    // 種類を取得
    Type GetType()const { return type; }   

private:
    void Copy(const CP_Collider& _other);

    /// @brief RigidBodyコンポーネントを作成する
    void CreateRigidBody();

protected:
    void Start() override;

    /// @brief RigidBpdyコンポーネントに形状を送る
    /// @param _shape 形状
    void SendShapeToRb(btCollisionShape& _shape);
};

