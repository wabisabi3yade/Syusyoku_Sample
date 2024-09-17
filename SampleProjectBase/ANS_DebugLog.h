#pragma once
#include "AnimationNotifyState.h"

class ANS_DebugLog : public AnimationNotifyState
{
	std::string message[3];

	u_int selectId;

	char* inputString;
public:
	ANS_DebugLog();
	~ANS_DebugLog() {}

	std::string ClassNameToStr() override;
private:
	/// @brief イベント開始処理
	void Begin() override;

	/// @brief イベント間の更新処理
	void Tick() override;

	/// @brief イベント終了処理
	void End() override;

	void ImGuiSetting() override;
};

