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

	/// @brief 当たり判定のオフセット座標
	DirectX::SimpleMath::Vector3 collOffsetPos;

	/// @brief 当たり判定のオフセット角度
	DirectX::SimpleMath::Vector3 collOffsetAngle;

	/// @brief 質量(0.0なら静的オブジェクト)
	float mass;

	/// @brief 剛体か？
	bool isRigidBody;

	/// @brief 形状がセットされているか？
	bool isSetShape;
public:
	CP_RigidBody();
	CP_RigidBody(const CP_RigidBody& _other);
	~CP_RigidBody();

	CP_RigidBody& operator=(const CP_RigidBody& _other);

	void Awake() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief 質量をセットし、再計算
	/// @param _mass 質量
	void SetMass(float _mass);

	/// @brief 当たり判定の形状をセット
	/// @param _shape 形状
	void SetShape(btCollisionShape& _shape);

	/// @brief オブジェクトからのオフセット座標をセット
	/// @param _originPos オフセット座標
	void SetCollOffsetPos(const DirectX::SimpleMath::Vector3& _offsetPos);

	/// @brief オブジェクトからのオフセット角度をセット
	/// @param _offsetAngles オフセット角度
	void SetCollAngles(const DirectX::SimpleMath::Vector3& _offsetAngles);

	/// @brief 形状を削除する
	void RemoveShape();
private:
	void Copy(const CP_RigidBody& _other);

	void Start() override;

	/// @brief BulletのTransformに変換する
	/// @param _btTransform 格納するBullet Transform
	void ToBtTransform(btTransform& _btTransform);

	/// @brief 剛体かどうセット
	/// @param _isRB 剛体か？
	void SetRigidBody(bool _isRB);

	/// @brief 衝突オブジェクトを解放し、物理空間から外す
	void RemoveCollObject();

	/// @brief 衝突オブジェクト作成
	void CreateCollObject();

	/// @brief 剛体を作成
	void CreateRB();

	/// @brief Ghostを作成
	void CreateGhost();

	/// @brief Transformを更新する
	void UpdateBtTransform();
};

