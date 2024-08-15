#pragma once
#include "Singleton_Base.h"

// ゲーム内入力クラス
class GameInput : public Singleton_Base<GameInput>
{
public:
	// bool型で扱う種類
	enum class ButtonType
	{
		Player_Attack,
		Player_Jump,
		Max_BoolNum
	};

	// 数値で扱う種類
	enum class ValueType
	{
		Player_Move,
		Camera_Move,
		Max_ValueNum
	};

private:

	friend class Singleton_Base<GameInput>;

	// 対応ボタン
	struct ButtonLink
	{
		GamePad::Button padType;
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
	ButtonLink buttonLinks[static_cast<u_int>(ButtonType::Max_BoolNum)];
	ValueLink valueLinks[static_cast<u_int>(ValueType::Max_ValueNum)];

	// 状態
	bool c_buttonState[static_cast<u_int>(ButtonType::Max_BoolNum)];
	bool o_buttonState[static_cast<u_int>(ButtonType::Max_BoolNum)];

	DirectX::SimpleMath::Vector2 c_valueState[static_cast<u_int>(ButtonType::Max_BoolNum)];


	GameInput();
	~GameInput() {}
public:
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

	/// @brief 入力スティックの数値を取得する
	/// @param _valueType ゲーム内スティックの種類
	/// @return 入力数値
	DirectX::SimpleMath::Vector2 GetValue(ValueType _valueType);

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
};

