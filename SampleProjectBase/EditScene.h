#pragma once
#include "Scene.h"

namespace HashiTaku
{
	/// @brief 編集をするシーンクラス
	class EditScene : public Scene, IImGuiUser
	{
		/// @brief シーン再生中か？
		bool isPlaying;

		/// @brief  ポーズ中か？
		bool isPause;

		/// @brief 1フレームだけ再生中か？
		bool isOneFrameProgressing;
	public:
		/// @brief コンストラクタ
		/// @param _sceneName シーン名
		/// @param _frameRateClass 可変フレームレートクラス
		EditScene(const std::string& _name);
		~EditScene() {}

		/// @brief 実行関数
		void Exec() override;

	private:
		bool CanSceneUpdate();

		/// @brief セーブ機能
		void SceneSave();

		// セーブファイルのパス名
		std::string SaveFilePath();

		/// @brief ImGuiによるシーン内のパラメータ編集などの開発関数
		void ImGuiDebug() override;

		/// @brief 再生中の表示
		void ImGuiPlaying();

		/// @brief 停止中の表示
		void ImGuiStop();

		/// @brief 再生を始める
		void PlayStart();

		/// @brief 再生を止める
		void PlayEnd();

		/// @brief 再生をポーズしたり、開始したり
		void PlayPauseAndBegin();

		/// @brief 開始する
		void StartProgressOneFrame();
	};
}