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
		std::unique_ptr<btCollisionObject> pCollisionObject;
		/// @brief 剛体操作
		std::unique_ptr<btDefaultMotionState> pMotionState;
		/// @brief コリジョン形状
		/*std::unique_ptr<btCollisionShape> pColliderShape;*/
		CP_Collider* pColliderComp{ nullptr };
		/// @brief 慣性モーメント
		DirectX::SimpleMath::Vector3 inertia;
	};
	//std::list<std::unique_ptr<CollPair>> colliders;	// 衝突形状リスト
	std::unique_ptr<CollPair> collider;

	/// @brief 質量
	float mass;

	/// @brief 常に計算させる
	bool isAwake;

	/// @brief すり抜けるか？
	bool isTrigger;
public:
	CP_RigidBody2();
	~CP_RigidBody2() {}

	// コンポーネント共通関数
	void Init() override;
	void Start() override;
	void OnDestroy() override;
	void OnChangeTransform() override;
	void OnEnableTrue() override;
	void OnEnableFalse() override;

	/// @brief コライダーをセットする
	/// @param _setCollider セットするコライダーコンポーネント
	void SetColliderShape(CP_Collider& _setCollider);
	
	/// @brief コライダーを削除する
	/// @param _removeCollider 
	void RemoveColliderShape(CP_Collider& _removeCollider);

	/// @brief 質量をセットする
	/// @param _mass 
	void SetMass(float _mass);

	/// @brief 静的オブジェクトに遷移しないようにするかセットする
	/// @param _isAwake 静的オブジェクトに遷移しないようにするか？
	void SetIsAwake(bool _isAwake);

	/// @brief 実態を持たないように（ghostObject）にするかセットする
	/// @param _isTrigger  実態を持たないようにするか？
	void SetIsTrigger(bool _isTrigger);

	/// @brief DXからBulletにトランスフォームを代入する
	void SetToBtTransform();

	/// @brief BulletからDXにトランスフォームを代入する
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

	/// @brief btRigidBody型に変換する
	/// @return btRigidBpdy参照
	btRigidBody& CastRigidBody();

	/// @brief 再度コライダーを作り直す（RigidBodyからGhpstObjectにしたいときとか）
	void ReCreateCollider();
};

