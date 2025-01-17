#pragma once

// シーン内システムクラス
#include "SceneObjects.h"
#include "SceneLights.h"
#include "ShadowDrawer.h"

namespace HashiTaku
{
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

		/// @brief 影描画機能クラス
		std::unique_ptr<ShadowDrawer> pShadowDrawer;

		/// @brief シーン内のΔtの速度倍率
		float timeScale;

		/// @brief カメラがNullObjectかどうか
		bool isNullCamera;

		InSceneSystemManager();
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

		/// @brief ビュー変換行列を更新
		void UpdateViewMatrix();

		/// @brief シーンのオブジェクト配列を取得
		/// @return オブジェクト配列の参照
		SceneObjects& GetSceneObjects();

		/// @brief シーンのライティング配列を取得
		/// @return ライティング配列の参照
		SceneLights& GetSceneLights();

		/// @brief 入力クラスを取得
		/// @return 入力クラス
		GameInput& GetInput();

		/// @brief 影描画クラスを取得
		/// @return 影描画クラス
		ShadowDrawer& GetShadowDrawer();

		/// @brief Δtの速度倍率を取得
		/// @return Δtの速度倍率
		float GetTimeScale() const;

		/// @brief カメラがセットされていないか？
		/// @return カメラがセットされていないか？
		bool GetIsNullCamera() const;

		// カメラをセットする
		void SetCamera(CP_Camera& _camera);

		/// @brief Δtの速度倍率をセット
		/// @param _timeScale 速度倍率(1.0が等速)
		void SetTimeScale(float _timeScale);

		/// @brief カメラを削除する
		/// @param _camera 削除するカメラ
		void DeleteCamera(CP_Camera& _camera);
	};
}