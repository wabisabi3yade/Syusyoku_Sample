#pragma once
#include "AnimationNotifyEvent.h"

class ANE_DebugLog : public AnimationNotifyEvent
{
	/// @brief 表示メッセージ
	std::string displayMessage;

#ifdef EDIT
	/// @brief 編集中かどうか？
	bool isEditing{ false };
#endif // EDIT

public:
	ANE_DebugLog();
	~ANE_DebugLog() {}

	// メッセージをセット
	void SetMessage(const std::string& _message);

	void ImGuiSetting() override;

	/// @brief クローン関数
	/// @return 生成した通知イベント
	std::unique_ptr<AnimationNotify_Base> Clone() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void OnEvent() override;
};

