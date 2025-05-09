#pragma once
#include "CP_ButtonGroup.h"

namespace HashiTaku
{
	class CP_Fade;

	/// @brief タイトルのボタン管理
	class CP_TitleButtonGroup : public CP_ButtonGroup
	{
		/// @brief タイトルで使用するボタンの種類
		enum class ButtonType
		{
			Start,
			End,
			Check,
			MaxNum
		};

		/// @brief フェードオブジェクト名
		std::string fadeObjName;

		/// @brief チェックマークオブジェクト名
		std::string checkImageObjName;

		/// @brief フェード
		CP_Fade* pFade;

		/// @brief チェックマークのイメージ
		CP_UIRenderer* pCheckImage;
	public:
		CP_TitleButtonGroup();
		~CP_TitleButtonGroup() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		void Start() override;

		/// @brief オブジェクトを探す
		void FindObject();

		/// @brief スタートボタンを押したときの処理
		void OnPushStart();

		/// @brief 終了ボタンを押したときの処理
		void OnPushEnd();

		/// @brief チェックマークを押したときの処理
		void OnPushCheck();

		/// @brief ボタンの初期化
		void ButtonSetup();

		void ImGuiDebug() override;
	};
}