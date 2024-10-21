#pragma once

#include "BulletDebugDraw.h"
#include "BulletContactCallBack.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class CP_RigidBody;

/// @brief DX11でBulletPhisicsを使用時、変数と関数を管理するクラス
class DX11BulletPhisics : public Singleton_Base<DX11BulletPhisics>
{
	friend class Singleton_Base<DX11BulletPhisics>;

	/// @brief シーン内のRigidBodyオブジェクト(剛体のみ)
	std::list<CP_RigidBody*> sceneRigidbodys;

	/// @brief シーン内のGhostObject
	std::list<CP_RigidBody*> sceneGhostObjects;

	/// @brief 衝突検出の動作を設定
	std::unique_ptr<btDefaultCollisionConfiguration> pCollisionConfiguration;

	/// @brief 個々の衝突ペアに対して衝突検出
	std::unique_ptr<btCollisionDispatcher> pDispatcher;

	/// @brief 物理シミュレーション内で衝突検出
	std::unique_ptr<btBroadphaseInterface> pBroadphase;

	/// @brief 剛体の力学計算
	std::unique_ptr<btSequentialImpulseConstraintSolver> pSolver;

	/// @brief 物理シミュレーション全体を管理する
	std::unique_ptr<btDiscreteDynamicsWorld> pDynamicsWorld; 

	/// @brief GhostObjectのラップ検知を効率化するクラス
	std::unique_ptr<btGhostPairCallback> pGhostPairCallback;

	/// @brief デバッグ描画
	std::unique_ptr<BulletDebugDraw> pDebugDraw;

	/// @brief コールバックを呼び出すクラス
	std::unique_ptr<BulletContactCallBack> pContactCallBack;

	/// @brief 最大サブステップ数
	u_int maxSubSteps;

	/// @brief 重力値
	DirectX::SimpleMath::Vector3 gravityValue;
public:
	/// @brief Bullet初期化
	void Init();

	/// @brief 更新処理
	void Update();

	/// @brief 当たっているオブジェクトのコールバックを呼び出す
	void CollisionCallBack();

	/// @brief 描画処理
	void Draw();

	/// @brief 物理空間に衝突オブジェクトを追加する
	/// @param _rigidBodyCp 追加するRigidBodyコンポーネント
	/// @param _groupId 追加する当たり判定のグループ
	void AddCollObj(CP_RigidBody& _rigidBodyCp, int _groupId);

	/// @brief 物理空間に衝突オブジェクトを削除する
	/// @param _rigidBodyCp 削除するRigidBodyコンポーネント
	void RemoveCollObj(CP_RigidBody& _rigidBodyCp);

	/// @brief シーン内衝突オブジェクトの座標をDxに更新させる(Bt→Dx)
	void UpdateTransformBtToDx();

	/// @brief シーン内衝突オブジェクトの座標を更新する(Dx→Bt)
	void UpdateTransformDxToBt();

	/// @brief 表示させるかセットする
	/// @param _setBool 表示させるかフラグ
	void SetDisplay(bool _setBool);

	// 衝突オブジェクトをチェック
	u_int GetCollObjCnt() const;

	/// @brief 表示させるか取得
	/// @return 表示させるか？ 
	bool GetDisplay() const;

	// 重力値を取得する
	DirectX::SimpleMath::Vector3 GetGravityValue() const;
private:
	DX11BulletPhisics();
	~DX11BulletPhisics() {}

	/// @brief ワールド空間内に衝突オブジェクトがあるか確認
	/// @param _checkCollObj チェックする衝突オブジェクト
	/// @return ワールド空間内にあるか？
	bool IsExistCollObjInWorld(btCollisionObject& _checkCollObj);

	/// @brief 衝突終了があれば終了更新処理を行う
	void UpdateColExit();
};

