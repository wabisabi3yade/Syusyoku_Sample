#pragma once
#include "PlayerActState_Base.h"

class PlayerGuardState : public PlayerActState_Base
{
	/// @brief �p���B�ł����Ԃ̎�������
	u_int sustainParryFrame;

	/// @brief �o�ߎ���
	u_int parryElapsedFrame;

	/// @brief �p���B�ő�����Q�[�W
	float parryAddGuardGage;

	/// @brief �K�[�h���邱�Ƃ��ł��鐳�ʂ̊p�x
	float canParryForwardAngle;

	/// @brief �p���B�ł�����
	bool canParry;
public:
	PlayerGuardState();
	~PlayerGuardState() {}

	/// @brief �p���B�ł��邩�m�F
	/// @return �p���B�ł��邩�H
	bool GetCanParry(const DirectX::SimpleMath::Vector3& _enemyPos);

	/// @brief �p���B���̍s��
	void OnParry();

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �J�n
	void OnStartBehavior() override;

	/// @brief �X�V
	void UpdateBehavior() override;

	/// @brief �I��
	void OnEndBehavior() override;

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName,
		const std::string& _toAnimNodeName) override;

	/// @brief �p���B�ł��鎞�Ԃ̍X�V
	void ParryTimeUpdate();

	/// @brief �Q�[�W������ċ��͂Ȉꌂ
	void ReleaseAttack();

	/// @brief �p���B���̍s��
	void GuardParry();

	void ImGuiDebug() override;
private:
	// �K�[�h���̃p�����[�^��
	static constexpr auto GUARD_PARAMNAME{ "isGuard" };
};

