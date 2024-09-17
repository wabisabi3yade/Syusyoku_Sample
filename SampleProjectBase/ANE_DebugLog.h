#pragma once
#include "AnimationNotifyEvent.h"

class ANE_DebugLog : public AnimationNotifyEvent
{
	/// @brief �\�����b�Z�[�W
	std::string displayMessage;

public:
	ANE_DebugLog();
	~ANE_DebugLog() {}

	// ���b�Z�[�W���Z�b�g
	void SetMessage(const std::string& _message);

	std::string ClassNameToStr() override;
private:
	void OnEvent() override;
};

