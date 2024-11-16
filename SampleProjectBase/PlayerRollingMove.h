#pragma once
#include "PlayerActState_Base.h"
#include "AnimationCurve.h"

/// @brief �v���C���[�̃��[�����O����
class PlayerRollingMove : public PlayerActState_Base
{
	/// @brief �ړ������̕ψʃA�j���[�V�����J�[�u
	std::unique_ptr<AnimationCurve> pDistanceCurve;

	/// @brief ���[�����O����
	float rollingDistance;

	/// @brief ���G����
	float invicibleTime;

	/// @brief �o�ߎ���
	float elapsedTime;

	/// @brief �O�t���[���܂łɐi�񂾋���
	float prevProgressDistance;
public:
	PlayerRollingMove();
	~PlayerRollingMove() {}

private:
	/// @brief �e��Ԃ̊J�n����
	void OnStartBehavior() override;

	/// @brief �X�V����
	void UpdateBehavior() override;

	/// @brief �I������
	void OnEndBehavior() override;

	/// @brief �A�j���[�V�����I�����̍s��
	/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
	/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
	void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName) override;

	/// @brief ���͕����ɑ����Ɍ���
	void TurnInputVec();

	/// @brief �J�n���̃p�����[�^����
	void BeginParametar();

	/// @brief �O�i�����Ɉړ�����
	void Move();

	/// @brief ���G���Ԃ̍X�V
	void UpdateInvicible();

	void ImGuiDebug() override;

	/// @brief �Z�[�u����
	/// @return �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
public:
	/// @brief ���[�����O�g���K�[�̃A�j���[�V�����p�����[�^��
	static constexpr auto ROLLING_ANIMPARAM{ "rollingTrigger" };
};

