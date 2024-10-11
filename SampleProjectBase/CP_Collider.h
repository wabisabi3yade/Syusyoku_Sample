#pragma once
#include "Component.h"

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

    /// @brief 中心座標と角度のオフセットを設定するための親コリジョン
    std::unique_ptr<btCompoundShape> pCompound;

    /// @brief コリジョンの形状
    std::unique_ptr<btCollisionShape> pCollisionShape;

    /// @brief 中心座標オフセット
    DirectX::SimpleMath::Vector3 centerOffset;

    /// @brief 角度オフセット
    DirectX::SimpleMath::Vector3 angleOffset;
    
public:
    CP_Collider() : type(Type::Num) {}
    CP_Collider(Type _type);
    virtual ~CP_Collider() {}
    CP_Collider(const CP_Collider& _other);

    CP_Collider& operator=(const CP_Collider& _other);

    void Init() override;
    void OnDestroy() override;

    void SetCenterOffset(const DirectX::SimpleMath::Vector3& _offset);

    void SetAngleOffset(const DirectX::SimpleMath::Vector3& _offset);

    /// @brief 衝突形状取得
    /// @return 衝突形状
    btCollisionShape& GetColliderShape();

    // 種類を取得
    Type GetType()const { return type; }   

    void ImGuiSetting() override;

    /// @brief セーブする
     /// @param _data セーブシーンデータ
    virtual nlohmann::json Save() override;
    /// @brief ロードする
    /// @param _data ロードするシーンデータ 
    virtual void Load(const nlohmann::json& _data) override;

private:
    void Copy(const CP_Collider& _other);

    /// @brief RigidBodyの形状をとる
    void RemoveShapeFromRb();

protected:
    void OnEnableTrue() override;
    void OnEnableFalse() override;

    void OnChangeTransform() override;

    void SettingShape();
    virtual void CreateShape() = 0;

    /// @brief RigidBpdyコンポーネントに形状を送る
    void SendShapeToRb();

    void RemoveFromCompound();

    void AddToCompound();
};

