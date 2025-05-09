#pragma once
#include <xinput.h>
#pragma comment (lib, "xinput.lib")


namespace HashiTaku
{
	/// @brief ゲームパッドクラス
	class GamePad
	{
	public:
		enum PadFlag	// パッドの入力フラグ
		{
			// 十字
			Arrow_Up = 0,
			Arrow_Down,
			Arrow_Right,
			Arrow_Left,

			// 記号
			Sankaku,
			Batsu,
			Maru,
			Shikaku,

			// RL
			R1,
			L1,
			R2,
			L2,
			R3,
			L3,

			// スティック
			StickR_Up,
			StickR_Down,
			StickR_Right,
			StickR_Left,

			StickL_Up,
			StickL_Down,
			StickL_Right,
			StickL_Left,


			// オプション
			Option,
			Share,

			Button_Num
		};

		enum Value	// 傾きをとる
		{
			// スティック
			StickR_X = 0,
			StickR_Y,
			StickL_X,
			StickL_Y,

			// トリガー
			Trigger_R2,
			Trigger_L2,

			Value_Num
		};

	private:
		/// @brief スティックなどの入力値
		float padValue[static_cast<int>(Value::Value_Num)];

		/// @brief 振動時間
		float vibeTime;

		/// @brief 振動の経過時間
		float elapsedViveTime;

		/// @brief コントローラーがつながっているか状態
		DWORD isConnectResult;

		/// @brief 現在のボタン状態
		bool buttonState[static_cast<int>(PadFlag::Button_Num)];

		/// @brief 1フレーム前のボタン状態
		bool o_buttonState[static_cast<int>(PadFlag::Button_Num)];

		/// @brief コントローラー振動中か？
		bool isVibration;
	public:
		GamePad();
		~GamePad();

		void InputUpdate();	// 入力の更新

		/// @brief コントローラー振動の更新処理
		void VibrationUpdate();

		bool Input(const PadFlag& _getButton)const;	// 押されているか(継続)
		bool InputTrigger(const PadFlag& _getButton)const;	// 押した瞬間(瞬間)
		bool InputRelease(const PadFlag& _getButton)const;	// 離れた瞬間(瞬間)]

		/// @brief コントローラー振動
		/// @param _power 震えるパワー(0.0～1.0)
		/// @param _time 振動時間
		void SetVibration(float _power, float _time);

		/// @brief コントローラー振動を終了する
		void EndVibration();

		/// @brief スティックやLRトリガーの入力量を取得する
		/// @param _getValue 取得する場所
		/// @return 入力量
		float GetValue(const Value& _getValue) const;

		bool GetConnecting() const { return (isConnectResult == ERROR_SUCCESS); }	// 繋がってるか確認する
	};


}
