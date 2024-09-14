#pragma once

#include "BulletDebugDraw.h"

/// @brief DX11でBulletPhisicsを使用時、変数と関数を管理するクラス
class DX11BulletPhisics : public Singleton_Base<DX11BulletPhisics>
{
	friend class Singleton_Base<DX11BulletPhisics>;

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

	/// @brief デバッグ描画
	std::unique_ptr<BulletDebugDraw> pDebugDraw;

	/// @brief 最大サブステップ数
	u_int maxSubSteps;

	/// @brief 重力値
	DirectX::SimpleMath::Vector3 gravityValue;
public:
	/// @brief Bullet初期化
	void Init();

	/// @brief 更新処理
	void Update();

	/// @brief 描画処理
	void Draw();

	/// @brief 物理空間に衝突オブジェクトを追加する
	/// @param _collObj 追加する衝突オブジェクト
	/// @param _groupId 追加する当たり判定のグループ
	void AddCollObj(btCollisionObject& _collObj, int _groupId);

	/// @brief 物理空間に衝突オブジェクトを削除する
	/// @param _collObj 削除する衝突オブジェクト
	void RemoveCollObj(btCollisionObject& _collObj);

	// 衝突オブジェクトをチェック
	u_int GetCollObjCnt() const;

	// 重力値を取得する
	DirectX::SimpleMath::Vector3 GetGravityValue() const;
private:
	DX11BulletPhisics();
	~DX11BulletPhisics() {}

	/// @brief ワールド空間内に衝突オブジェクトがあるか確認
	/// @param _checkCollObj チェックする衝突オブジェクト
	/// @return ワールド空間内にあるか？
	bool IsExistCollObjInWorld(btCollisionObject& _checkCollObj);
};

