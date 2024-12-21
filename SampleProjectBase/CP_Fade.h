#pragma once
#include "Component.h"

namespace HashiTaku
{
	class CP_UIRenderer;

	/// @brief フェードクラス
	class CP_Fade : public Component
	{
		// 状態
		enum class State
		{
			Wait, // 何もしない
			Open, // フェードを薄くする
			Close // フェードを濃くする
		};

		/// @brief フェード終了時に起こす関数
		std::function<void()> endFunction;

		/// @brief UI描画クラス
		CP_UIRenderer* pUIRenderer;

		/// @brief フェードの状態
		State state;

		/// @brief 呼び出されてフェードする時間
		float settingFadeTime;

		/// @brief フェードで変えるa値の速度
		float fadeSpeed;

		/// @brief 目的とするα値
		float targetAlpha;

		/// @brief フェードで閉める時間
		float fadeCloseTime;

		/// @brief フェードで開ける時間
		float fadeOpenTime;

		/// @brief スタート時にフェードを開けて開始するようにする
		bool isStartOpen;
	public:
		CP_Fade();
		~CP_Fade() {}

		/// @brief フェードを空ける時間
		void OpenFade();

		/// @brief フェードを空ける時間
		/// @param _openTime 開ける時間
		/// @param _targetAlpha 目標のα
		void OpenFade(float _openTime, float _targetAlpha = 0.0f);

		/// @brief フェードで閉める時間
		void CloseFade();

		/// @brief フェードを閉める時間
		/// @param _closeTime 閉める時間
		/// @param _targetAlpha 目標のα
		void CloseFade(float _closeTime, float _targetAlpha = 1.0f);

		/// @brief フェード終了時の関数をセットする
		/// @param _func 関数
		void SetOnEndFunction(const std::function<void()>& _func);

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 開始処理
		void Awake() override;

		/// @brief 更新処理
		void Update() override;

		/// @brief 閉める更新処理
		void CloseUpdate();

		/// @brief フェードの速度を求める
		void CalcFadeSpeed();

		/// @brief 開ける更新
		void OpenUpdate();

		/// @brief 更新できるか処理
		/// @return 更新できるか？
		bool GetCanUpdate() const;

		/// @brief 実行する
		void ExecEndFunction();

		void ImGuiDebug() override;
	};
}