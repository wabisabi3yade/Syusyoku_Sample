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
	/// @brief �C�x���g�J�n����
	void Begin() override;

	/// @brief �C�x���g�Ԃ̍X�V����
	void Tick() override;

	/// @brief �C�x���g�I������
	void End() override;

	void ImGuiSetting() override;
};

