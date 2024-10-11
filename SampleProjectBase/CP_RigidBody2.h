#pragma once
#include "Component.h"
#include "DX11BulletPhisics.h"

class CP_Collider;

class CP_RigidBody2 : public Component
{

	enum ShapeType
	{
		Box,
		Sphere,
		Num
	};

	/// @brief 衝突オブジェクトと、コリジョン形状のペア
	struct CollPair
	{
		/// @brief 衝突オブジェクト
		std::unique_ptr<btRigidBody> pCollisionObject;
		/// @brief 剛体操作
		std::unique_ptr<btDefaultMotionState> pMotionState;
		/// @brief コリジョン形状
		/*std::unique_ptr<btCollisionShape> pColliderShape;*/
		CP_Collider* pCollider{ nullptr };
		/// @brief 慣性モーメント
		DirectX::SimpleMath::Vector3 inertia;
	};
	//std::list<std::unique_ptr<CollPair>> colliders;	// 衝突形状リスト
	std::unique_ptr<CollPair> collider;

	/// @brief 質量
	float mass;

	/// @brief 常に計算させる
	bool isAwake;

	bool isTrigger;
public:
	CP_RigidBody2();
	~CP_RigidBody2() {}

	void Init() override;

	void Start() override;

	void OnDestroy() override;

	void OnChangeTransform() override;

	void OnEnableTrue() override;
	void OnEnableFalse() override;

	void SetColliderShape(CP_Collider& _setCollider);
	void SetColliderShape(ShapeType _shapeType);

	void RemoveColliderShape(CP_Collider& _removeCollider);

	void SetMass(float _mass);
	void SetIsAwake(bool _isAwake);

	void SetToBtTransform();
	void SetToDXTransform();

	void ImGuiSetting() override;

	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief BulletのTransformを取得する
	/// @param _btTrans 結果
	void GetBtTransform(btTransform& _btTrans);

	/// @brief ゲームオブジェクトからコライダーを探してセットする
	void FindSetCollider();
};

