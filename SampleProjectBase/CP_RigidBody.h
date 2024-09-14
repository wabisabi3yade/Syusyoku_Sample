#pragma once
#include "CloneComponent.h"

#include "DX11BulletPhisics.h"

/// @brief 重さや剛体かなどのパラメータ保持コンポーネント
class CP_RigidBody : public Component, public CloneComponent<CP_RigidBody>
{
	/// @brief 衝突オブジェクト(RigidBody, Ghost)
	std::unique_ptr<btCollisionObject> pCollisionObject;

	/// @brief 剛体操作
	std::unique_ptr<btDefaultMotionState> pMotionState;

	/// @brief 当たり判定の形状
	btCollisionShape* pShape;

	/// @brief 慣性モーメント
	DirectX::SimpleMath::Vector3 inertia;

	/// @brief 質量(0.0なら静的オブジェクト)
	float mass;

	/// @brief 剛体か？
	bool isRigidbody;

	/// @brief 重力を受けるか？
	bool isGravity;

	/// @brief 物理計算を行うか？
	bool isKinematic;

	/// @brief 形状がセットされているか？
	bool isSetShape;
public:
	CP_RigidBody();
	CP_RigidBody(const CP_RigidBody& _other);

	CP_RigidBody& operator=(const CP_RigidBody& _other);

	void Init() override;
	void Update() override;
	void ImGuiSetting() override;
	void OnDestroy() override;

	/// @brief 質量をセットし、再計算
	/// @param _mass 質量
	void SetMass(float _mass);

	/// @brief 当たり判定の形状をセット
	/// @param _shape 形状
	void SetShape(btCollisionShape& _shape);

	/// @brief 形状を削除する
	void RemoveShape();

	/// @brief DxからBtのトランスフォームに合わせる
	/// @param _dxTransform dxのトランスフォーム
	void SetTransformDxToBt();

	/// @brief  BtからDxのトランスフォームに合わせる
	void SetTransformBtToDx();
	
	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	void Copy(const CP_RigidBody& _other);

	void Start() override;

	void OnEnableTrue() override;
	void OnEnableFalse() override;

	/// @brief BulletのTransformに変換する
	/// @param _btTransform 格納するBullet Transform
	void ToBtTransform(btTransform& _btTransform);

	/// @brief 剛体かどうセット
	/// @param _isRB 剛体か？
	void SetIsRigidbody(bool _isRB);

	/// @brief 重力を受けるかセット
	/// @param _isGravity 重力を受けるか？
	void SetIsGravity(bool _isGravity);

	/// @brief 衝突オブジェクトを解放し、物理空間から外す
	void RemoveCollObject();

	/// @brief 衝突オブジェクト作成
	void CreateCollObject();

	/// @brief 剛体を作成
	void CreateRB();

	/// @brief Ghostを作成
	void CreateGhost();

	/// @brief 形状と剛体の座標を合わせる
	void CollisionSetTransform();

	/// @brief 衝突オブジェクトをbtRigidBodyに変換
	/// @return cast変換したポインタ
	btRigidBody* CastRigidBody();

	/// @brief 物理シミュレーションに衝突オブジェクトを追加する
	void AddCollisionToWorld();
};

