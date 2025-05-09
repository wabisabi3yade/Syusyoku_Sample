#pragma once
#include "CP_Button.h"
#include "CP_UIRenderer.h"

namespace HashiTaku
{
	/// @brief ボタンをグループ化して、管理する
	class CP_ButtonGroup : public Component
	{
		/// @brief ボタンのパラメータ
		struct ButtonParameter
		{
			/// @brief ボタンコンポーネント
			CP_Button* pButton{ nullptr };

			/// @brief 表示するときのボタンの座標
			DXSimp::Vector3 buttonPos;
		};

		/// @brief セレクトイメージの座標先オフセット（ボタンの座標から）
		std::vector<DXSimp::Vector3> selectOffsetPosFromButton;

		/// @brief デフォルトのボタンのID
		u_int defaultSelectButtonId;
	protected:
		/// @brief ボタンパラメータのグループ
		std::vector<ButtonParameter> buttonParameterGroup;

		/// @brief ボタンのコンポーネントを持つオブジェクト
		std::vector<std::string> buttonObjNames;

		/// @brief セレクトしてる時の画像
		CP_UIRenderer* pSelectBackImage;

		/// @brief セレクトしてる時の画像オブジェクト名
		std::string backImageObjName;

		/// @brief 現在選んでいるボタンのID
		int curSelectButtonId;

		/// @brief ボタンの数
		int maxButtonCnt;

		/// @brief 縦入力で移動するボタンの数
		int vertMoveSpeed;

		/// @brief 横入力で移動するボタンの数
		int horiMoveSpeed;

		/// @brief 移動することができるか
		bool canInput;
	public:
		CP_ButtonGroup();
		virtual ~CP_ButtonGroup() {}

		/// @brief 移動できるかセットする
		/// @param _canMove 移動できるかフラグ
		void SetCanMove(bool _canMove);

		/// @brief ボタンを表示する
		virtual void OpenDisplay();

		/// @brief ボタンを閉じる
		virtual void CloseDisplay();

		json Save() override;
		void Load(const json& _data) override;
	protected:
		void Start() override;

		void Update() override;

		/// @brief 入力で選択ボタンを移動する
		void MoveButton();

		/// @brief ボタンを押す
		void DecideButton();

		/// @brief セレクトの背景イメージを選択中のボタンへ移動
		void MoveSelectBackImage();

		/// @brief 設定しているボタンの座標へボタン画像を移動させる
		void SettingButtonPosition();

		/// @brief ボタンを表示させるか？
		/// @param _isDisplay 表示させるか？
		virtual void SetDisplayButtons(bool _isDisplay);

		/// @brief 最大数をセット
		/// @param _max 最大数
		void SetMaxButtonCnt(int _max);

		void ImGuiDebug() override;
	};
}