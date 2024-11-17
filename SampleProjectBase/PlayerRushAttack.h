#pragma once
#include "PlayerGroundAttack.h"

class ITargetAccepter;

/// @brief �O�ːi�U��
class PlayerRushAttack : public PlayerGroundAttack
{
	/// @brief ���ړ��J�[�u
	AnimationCurve distanceHoriCurve;

	/// @brief �^�[�Q�b�g��̃I�u�W�F�N�g
	ITargetAccepter* pTargetObj;

	/// @brief ���ړ����鋗���̒���
	float progressLengthHori;

	/// @brief �O��t���[���܂ł̉��ړ�����
	float prevDistanceHori;

	/// @brief �a��n�߂�G�Ƃ̋���
	float slashBeginDistance;

	/// @brief �O�ɐi�ނ�
	bool isProgressForward;
public:
	PlayerRushAttack();
	~PlayerRushAttack() {}

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	// State���ʏ���
	void OnStartBehavior() override;
	void UpdateBehavior() override;
	void TransitionCheckUpdate();

	/// @brief �O�����ɓːi
	void MoveForward();

	/// @brief �G�Ƃ̋�������苗�������ɂȂ�����a��I����
	bool CheckDistanceToEnemy(float _currentPlayRatio);

	/// @brief �^�[�Q�b�g�悪���݂��邩�擾
	/// @return �^�[�Q�b�g�悪���݂��邩�H
	bool isExistTrgeter() const;

	void ImGuiDebug() override;
};

