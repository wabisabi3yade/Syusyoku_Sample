#pragma once
#include "Singleton_Base.h"

namespace HashiTaku
{
	// ゲーム内入力クラス
	class GameInput
	{
	public:
		// bool型で扱う種類
		enum class ButtonType
		{
			Player_Attack,	// 攻撃
			Player_ChargeAttack,	// 攻撃
			Player_Jump, // ジャンプ
			Player_Rolling,	// ローリング
			Player_RockOn,	// ロックオン
			Player_Guard,	// ガード
			Max_BoolNum
		};
		/// @brief ボタンの種類数
		static constexpr u_int BUTTON_TYPE_CNT{ static_cast<u_int>(ButtonType::Max_BoolNum) };

		// 数値で扱う種類
		enum class ValueType
		{
			Player_Move,
			Camera_Move,
			Max_ValueNum
		};

	private:
		// 対応ボタン
		struct ButtonLink
		{
			GamePad::PadFlag padType;
			int keyType;
		};

		// 対応スティック
		struct ValueLink
		{
			GamePad::Value padValue_x;	// X
			GamePad::Value padValue_y;	// Y
			int keyType_x_positive;	// 正
			int keyType_x_negative;	// 負
			int keyType_y_positive;
			int keyType_y_negative;
		};

		/// @brief 対応する入力
		std::array<ButtonLink, BUTTON_TYPE_CNT> buttonLinks;
		std::array<ValueLink, BUTTON_TYPE_CNT> valueLinks;

		/// @brief ボタン入力の履歴時間(トリガー入力のみ対応)
		std::array<float, BUTTON_TYPE_CNT> buttonTimeLine;

		// 状態
		bool c_buttonState[BUTTON_TYPE_CNT];
		bool p_buttonState[BUTTON_TYPE_CNT];

		DirectX::SimpleMath::Vector2 c_valueState[static_cast<u_int>(ValueType::Max_ValueNum)];

	public:
		GameInput();
		~GameInput() {}

		/// @brief 更新処理
		void Update();

		/// @brief 押されているか取得
		/// @param _buttonType ゲーム内ボタンの種類
		/// @return 押されているか？
		bool GetButton(ButtonType _buttonType);

		/// @brief 押した瞬間か取得
		/// @param _buttonType ゲーム内ボタンの種類
		/// @return 押した瞬間か？
		bool GetButtonDown(ButtonType _buttonType);

		/// @brief 離された瞬間か取得
		/// @param _buttonType ゲーム内ボタンの種類
		/// @return 離された瞬間か？
		bool GetButtonUp(ButtonType _buttonType);

		/// @brief 指定秒以内にボタンが押されたか取得
		/// @param _buttonType ゲーム内ボタンの種類
		/// @param _withinTime 指定秒以内に押されている
		/// @return 指定秒以内にボタンが押されたか？
		bool GetButtonDown(ButtonType _buttonType, float _withinTime);

		/// @brief 入力スティックの数値を取得する
		/// @param _valueType ゲーム内スティックの種類
		/// @return 入力数値
		const DirectX::SimpleMath::Vector2& GetValue(ValueType _valueType);

	private:
		/// @brief デフォルトの設定にする
		void DefaultSetting();

		/// @brief スティック入力初期設定
		void InitValueSettings();

		// ゲームパッドがつながっているか取得
		bool IsGamePadConnect();

		/// @brief ボタン入力更新
		void ButtonUpdate();

		/// @brief 数値入力更新
		void ValueUpdate();

		/// @brief 履歴の更新処理
		void TimeLineUpdate();
	};
}