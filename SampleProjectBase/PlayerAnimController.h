#pragma once
#include "AnimationController.h"
#include "IAnimationObserver.h"

// �v���C���[�̃A�j���[�V�����R���g���[���[
class PlayerAnimController : public AnimationController, public HashiTaku::AnimationSubject
{
public:
	enum class AnimType
	{
		Move,
		Attack,
		Jump,
		MaxNum
	};

private:
	/// @brief ���݂̃A�j���[�V�������
	AnimType nowState;

	/// @brief ���x����
	float speedRatio;

public:
	PlayerAnimController();
	~PlayerAnimController() {}

	/// @brief �A�j���[�V�����ύX
	/// @param _type �A�j���[�V�����̃^�C�v
	/// @param _type ��ԏ������s����?
	void ChangeAnimationByState(AnimType _type, bool _isInterp = true);

	/// @brief �ړ����x�������Z�b�g
	/// @param _speedRatio �ړ����x����
	void SetMoveSpeedRatio(float _speedRatio);

private:
	/// @brief ��������
	void Init();

	/// @brief �A�j���[�V�����I������
	void OnAnimationFinish() override;
};

