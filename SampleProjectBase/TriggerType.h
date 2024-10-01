#pragma once

/// @brief イベントを起こしたかどうかのTrigger型変数
class TriggerType
{
	/// @brief イベントを起こしたらtrueになる
	bool isTrigger;

public:
	TriggerType();
	~TriggerType() {}

	/// @brief トリガーをtrueにする
	void SetIsTrigger();

	/// @brief isTriggerをリセットする
	void ResetTrigger();

	/// @brief トリガーの状態を取得
	/// @return トリガーの状態
	bool GetIsTrigger() const;
};

