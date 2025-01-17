#pragma once
#include "CP_ButtonGroup.h"

namespace HashiTaku
{
	class CP_Fade;

	// バトル中のボタン(ポーズ、ゲームオーバー時に)
	class CP_BattleButtonGroup : public CP_ButtonGroup
	{
		enum class ButtonType
		{
			Retry,
			Title,
			Back,
			MaxNum
		};

	public:
		CP_BattleButtonGroup();
		~CP_BattleButtonGroup() {}

		/// @brief フェードオブジェクト名
		std::string fadeObjName;
	
		/// @brief 背景オブジェクト名
		std::string frontFadeName;

		/// @brief フェードオブジェクト
		CP_Fade* pFade; 

		/// @brief 手前のフェードオブジェクト
		CP_Fade* pFrontFade;

		/// @brief ボタンを押した後のフェードを閉じる時間
		float closeFadeTime;

		/// @brief ポーズ時のフェードにα値
		float poseFadeAlpha;

		/// @brief ボタンを表示する
		void OpenDisplay() override;

		/// @brief ボタンを閉じる
		void CloseDisplay() override;

		json Save() override;
		void Load(const json& _data) override;

	private:
		void Start() override;

		/// @brief オブジェクトを探す
		void FindObject();

		/// @brief ボタンの初期化
		void ButtonSetup();

		/// @brief リトライボタンを押したときの処理
		void OnPushRetry();

		/// @brief メニューボタンを押したときの処理
		void OnPushTitle();

		void ImGuiDebug() override;
	};
}


