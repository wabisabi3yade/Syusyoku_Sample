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
    enum ShapeType   // 形状の種類
    {
        Box,    // ボックス
        Sphere, // 球
        Capsule,    // カプセル
        Num
    };

private:
    /// @brief 既に形状は作成されたか？
    bool isCreateCompound;

protected:
    ShapeType type;  // タイプ

    /// @brief 中心座標と角度のオフセットを設定するための親コリジョン
    std::unique_ptr<btCompoundShape> pCompound;

    /// @brief コリジョンの形状
    std::unique_ptr<btCollisionShape> pCollisionShape;

    /// @brief 中心座標オフセット
    DirectX::SimpleMath::Vector3 centerOffset;

    /// @brief 角度オフセット
    DirectX::SimpleMath::Vector3 angleOffset;
    
public:
    CP_Collider() : type(ShapeType::Num), isCreateCompound(false) {}
    CP_Collider(ShapeType _type);
    virtual ~CP_Collider() {}
    CP_Collider(const CP_Collider& _other);

    CP_Collider& operator=(const CP_Collider& _other);

    // コンポーネント共通関数
    void Init() override;
    void OnDestroy() override;

    /// @brief コリジョンの中心座標オフセットをセット
    /// @param _offset オフセット座標
    void SetCenterOffset(const DirectX::SimpleMath::Vector3& _offset);

    /// @brief コリジョンの角度オフセットをセット
    /// @param _offset オフセット角度
    void SetAngleOffset(const DirectX::SimpleMath::Vector3& _offset);

    /// @brief 衝突形状取得
    /// @return 衝突形状
    btCollisionShape& GetColliderShape();

    // 種類を取得
    ShapeType GetType()const;

    bool GetIsCreateCompound() const;

    void ImGuiSetting() override;

    virtual nlohmann::json Save() override;
    virtual void Load(const nlohmann::json& _data) override;
private:
    void Copy(const CP_Collider& _other);

    /// @brief RigidBodyの形状をとる
    void RemoveShapeFromRb();

protected:
    // コンポーネント共通関数
    void OnEnableTrue() override;
    void OnEnableFalse() override;
    void OnChangeScale() override;

    /// @brief 形状を作成し直し、コンパウンドに追加
    void RecreateShape();

    /// @brief 各コリジョン形状を作成する
    virtual void CreateShape() = 0;

    /// @brief RigidBpdyコンポーネントに形状を送る
    void SendShapeToRb();

    /// @brief コンパウンドから形状を削除
    void RemoveFromCompound();

    /// @brief コンパウンドに形状を追加
    void AddToCompound();
};

