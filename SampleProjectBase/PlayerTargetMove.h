#pragma once
#include "PlayerMoveState.h"

class PlayerTargetMove : public PlayerMoveState
{
public:
	PlayerTargetMove();
	~PlayerTargetMove() {}

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void OnEndBehavior() override;
	void TransitionCheckUpdate() override;

	/// @brief �u�����h�������Z�b�g
	void ApplyBlendAnim();

	/// @brief ���[�g���[�V�������Z�b�g
	void ApplyRootMotion();

	void ImGuiSetting() override;
};

