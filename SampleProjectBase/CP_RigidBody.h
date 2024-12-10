#pragma once
#include "Component.h"
#include "DX11BulletPhisics.h"
#include "CollisionTypeJudge.h"

class CP_Collider;

class CP_RigidBody : public Component
{
	friend class CollisionTypeJudge;

	/// @brief 当たり判定形状タイプ
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
		// 衝突のタイミングを判断する関数
		std::unique_ptr<CollisionTypeJudge> pColTypeJudge;
		// 衝突形状
		CP_Collider* pColliderComp{ nullptr };
		/// @brief 慣性モーメント
		btVector3 inertia;
	};

	/// @brief 衝突パラメータ(nullptrなら判定がない)
	std::unique_ptr<CollPair> collider;

	/// @brief 質量
	float mass;

	/// @brief 摩擦係数(0で完全に滑る)
	float friction;

	/// @brief 常に計算させる
	bool isAwaking;

	/// @brief すり抜けるか？
	bool isTrigger;

	/// @brief キネマティック
	bool isKinematic;

	/// @brief 軸の数
	static constexpr short AXIS_CNT = 3;

	/// @brief 固定したい回転軸フラグ
	std::array<bool, AXIS_CNT> isFreezeRotation;
public:
	CP_RigidBody();
	~CP_RigidBody() {}

	// コンポーネント共通関数
	void Init() override;
	void OnDestroy() override;
	void OnEnableTrue() override;
	void OnEnableFalse() override;

	/// @brief 衝撃を加える
	/// @param _power パワー
	void AddImpulse(const DirectX::SimpleMath::Vector3& _power);

	/// @brief 力を加える
	/// @param _power パワー
	void AddForce(const DirectX::SimpleMath::Vector3& _power);

	/// @brief コライダーをセットする
	/// @param _setCollider セットするコライダーコンポーネント
	void SetColliderShape(CP_Collider& _setCollider);
	
	/// @brief コライダーを削除する
	/// @param _removeCollider 
	void RemoveColliderShape(CP_Collider& _removeCollider);

	/// @brief 質量をセットする
	/// @param _mass 質量
	void SetMass(float _mass);

	/// @brief 質量をセットする
	/// @param _friction セットする摩擦係数
	void SetFriction(float _friction);

	/// @brief 静的オブジェクトに遷移しないようにするかセットする
	/// @param _isAwake 静的オブジェクトに遷移しないようにするか？
	void SetIsAwake(bool _isAwake);

	/// @brief 実態を持たないように（ghostObject）にするかセットする
	/// @param _isTrigger  実態を持たないようにするか？
	void SetIsTrigger(bool _isTrigger);

	/// @brief キネマティックをセット
	/// @param _isKinematic キネマティック
	void SetIsKinematic(bool _isKinematic);

	/// @brief DXからBulletにトランスフォームを代入する
	void SetToBtTransform();

	/// @brief BulletからDXにトランスフォームを代入する
	void SetToDXTransform();

	/// @brief 速度をセット
	/// @param _velocity 速度
	void SetVelocity(const DirectX::SimpleMath::Vector3& _velocity);

	/// @brief 速度をクリアする
	void ClearVelocity();

	/// @brief 衝突オブジェクトを取得する
	/// @param _outObject 結果オブジェクト
	btCollisionObject& GetCollisionObject();

	/// @brief 衝突タイプ判定を取得
	/// @return 衝突タイプ判定クラス変数
	CollisionTypeJudge& GetColTypeJudge() const;

	/// @brief 剛体の速度を取得する
	/// @return 速度
	DirectX::SimpleMath::Vector3 GetVelocity() const;

	void ImGuiDebug() override;

	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	void UpdateFreezeRotation();

	/// @brief ゲームオブジェクトからコライダーを探してセットする
	void FindSetCollider();

	/// @brief 剛体を作成する
	void CreateRigidBody();

	/// @brief GohstObjectを作成する
	void CreateGhost();

	/// @brief 剛体でもGhostでも共通に起きる処理
	void CommonCreateColObj();

	/// @brief btRigidBody型に変換する
	/// @return btRigidBpdy参照
	btRigidBody& CastRigidBody();

	/// @brief DxからBulletのトランスフォームを取得する
	/// @param _outBtTransform DxからBulletのTransformを取得
	void CastDxToBtTransform(btTransform& _outBtTransform);

	/// @brief 再度コライダーを作り直す（RigidBodyからGhpstObjectにしたいときとか）
	void ReCreateCollider();

	/// @brief 衝突中のRbリストに相手方から追加する
	/// @param _addRb 追加したRb
	void AddCurrentCollision(CP_RigidBody& _addRb);

	/// @brief BulletのTransformを取得する
	/// @param _btTrans 結果
	void GetBtTransform(btTransform& _btTrans);

	// ImGuiで回転固定に関する編集
	void ImGuiFreezeRot();
};

