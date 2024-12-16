#pragma once
#include "BulletTest.h"
#include "InSceneSystemManager.h"

namespace HashiTaku
{
	// 1シーンクラス
	class Scene
	{
	protected:
		/// @brief シーン名
		std::string sceneName;

		/// @brief シーン内システムマネジャー
		InSceneSystemManager* pInSceneSystem;

		/// @brief エフェクトマネジャー
		DX11EffecseerManager* pEffectManager;
	public:
		Scene(const std::string& _name);
		virtual ~Scene();

		/// @brief 実行関数
		virtual void Exec();

		// シーン名設定
		void SetName(const std::string& _name);

		// 名前を取得
		std::string GetName() const;

	protected:
		/// @brief シーンない更新
		void SceneUpdate();

		/// @brief シーン内描画
		void SceneDraw();

		/// @brief jsonファイルからシーンをロード
		void SceneLoad();
	private:
		/// @brief 描画準備
		void DrawSetup();

		// セーブファイルのパス名
		std::string SaveFilePath();
	};
}