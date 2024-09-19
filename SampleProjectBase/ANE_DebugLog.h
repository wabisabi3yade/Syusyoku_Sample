#pragma once
#include "AnimationNotifyEvent.h"

class ANE_DebugLog : public AnimationNotifyEvent
{
	/// @brief 表示メッセージ
	std::string displayMessage;

public:
	ANE_DebugLog();
	~ANE_DebugLog() {}

	// メッセージをセット
	void SetMessage(const std::string& _message);

	std::string GetTypeName() const override;

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void OnEvent() override;
};

