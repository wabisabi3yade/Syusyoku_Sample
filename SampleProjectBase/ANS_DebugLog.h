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

	// �^�����擾����
	std::string GetTypeName() const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �C�x���g�J�n����
	void Begin() override;

	/// @brief �C�x���g�Ԃ̍X�V����
	void Tick() override;

	/// @brief �C�x���g�I������
	void End() override;

	void ImGuiSetting() override;
};

