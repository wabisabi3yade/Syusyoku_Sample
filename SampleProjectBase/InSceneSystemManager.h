#pragma once

// シーン内システムクラス
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"

// シーン内部のシステムのインスタンスを持つクラス
class InSceneSystemManager : public Singleton_Base<InSceneSystemManager>
{
	friend class Singleton_Base<InSceneSystemManager>;

	/// @brief メインカメラ
	CP_Camera* pMainCamera;

	/// @brief ゲーム内入力クラス
	std::unique_ptr<GameInput> pGameInput;
	
	/// @brief シーンオブジェクト
	std::unique_ptr<SceneObjects> pSceneObjects;

	/// @brief シーンライティング
	std::unique_ptr<SceneLights> pSceneLights;

	/// @brief カメラがNullObjectかどうか
	bool isNullCamera;

	InSceneSystemManager() : pMainCamera(nullptr), isNullCamera(true) {}
	~InSceneSystemManager();

public:
	/// @brief 初期化処理
	void Init();

	/// @brief ゲーム内入力クラスを更新する
	void InputUpdate();

	/// @brief 変数を解放して、新しく生成する
	void Reset();

	/// @brief  メインカメラを取得
	/// @return カメラの取得
	CP_Camera& GetMainCamera();

	/// @brief シーンのオブジェクト配列を取得
	/// @return オブジェクト配列の参照
	SceneObjects& GetSceneObjects();

	/// @brief シーンのライティング配列を取得
	/// @return ライティング配列の参照
	SceneLights& GetSceneLights();

	/// @brief 入力クラスを取得
	/// @return 入力クラス
	GameInput& GetInput();

	// カメラをセットする
	void SetCamera(CP_Camera& _camera);

	/// @brief カメラを削除する
	/// @param _camera 削除するカメラ
	void DeleteCamera(CP_Camera& _camera);
};

