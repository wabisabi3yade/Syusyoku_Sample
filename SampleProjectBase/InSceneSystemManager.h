#pragma once

// シーン内システムクラス
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// シーン内部のシステムのインスタンスを持つクラス
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	/// @brief シーン内のRigidBody
	std::list<CP_RigidBody*> pSceneRbs;

	/// @brief メインカメラ
	CP_Camera* pMainCamera;
	
	/// @brief シーンオブジェクト
	std::unique_ptr<SceneObjects> pSceneObjects;

	/// @brief シーンライティング
	std::unique_ptr<SceneLights> pSceneLights;

	InSceneSystemManager() : pMainCamera(nullptr) {}
	~InSceneSystemManager();

public:
	void Init();

	/// @brief 変数を解放して、新しく生成する
	void Reset();

	/// @brief  メインカメラを取得
	/// @return 
	CP_Camera& GetMainCamera();

	/// @brief シーンのオブジェクト配列を取得
	/// @return オブジェクト配列の参照
	SceneObjects& GetSceneObjects();

	/// @brief シーンのライティング配列を取得
	/// @return ライティング配列の参照
	SceneLights& GetSceneLights();

	// カメラをセットする
	void SetCamera(CP_Camera& _camera);

	/// @brief RigidBodyを追加する 
	/// @param _setRb 追加するRigidBody
	void AddRigidBody(CP_RigidBody& _setRb);

	/// @brief RigidBodyを除外する 
	/// @param _removetRb 除外するRigidBody
	void RemoveRigidBody(CP_RigidBody& _removetRb);

	/// @brief BtTransformをDxに更新する
	void UpdateTransformBtToDx();
};

