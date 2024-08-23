#pragma once

// シーン内システムクラス
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// カメラ
class CP_Camera;

// シーン内部のシステムのインスタンスを持つクラス
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	/// @brief メインカメラ
	CP_Camera* pMainCamera;
	
	/// @brief シーンオブジェクト
	std::unique_ptr<SceneObjects> pSceneObjects;

	/// @brief シーンライティング
	std::unique_ptr<SceneLights> pSceneLights;

	/// @brief 衝突判定
	std::unique_ptr<CollisionChecker> pCollisionChecker;

	InSceneSystemManager() : pMainCamera(nullptr) {}
	~InSceneSystemManager();

public:
	void Init();

	/// @brief  メインカメラを取得
	/// @return 
	CP_Camera& GetMainCamera();

	/// @brief シーンのオブジェクト配列を取得
	/// @return オブジェクト配列の参照
	SceneObjects& GetSceneObjects();

	/// @brief シーンのライティング配列を取得
	/// @return ライティング配列の参照
	SceneLights& GetSceneLights();

	/// @brief シーンの衝突判定システムを取得
	/// @return 衝突判定システムの参照
	CollisionChecker& GetCollisonChecker() { return *pCollisionChecker; }

	// カメラをセットする
	void SetCamera(CP_Camera& _camera);
};

