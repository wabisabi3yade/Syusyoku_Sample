#pragma once
#include "AnimationNotifyState.h"

class ANS_DebugLog : public AnimationNotifyState
{
	std::string message[3];

	u_int selectId;

	char inputString[IM_INPUT_BUF];

	bool isEditing[3] = { false };
public:
	ANS_DebugLog();
	~ANS_DebugLog() {};

	// 型名を取得する
	std::string GetTypeName() const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief イベント開始処理
	void Begin() override;

	/// @brief イベント間の更新処理
	void Tick() override;

	/// @brief イベント終了処理
	void End() override;

	void ImGuiSetting() override;
};

