#pragma once

class AnimationParameters;

/// @brief �A�j���[�V�����p�����[�^�Z�b�g�C���^�[�t�F�[�X
class IAnimParametersSetter
{
public:
	IAnimParametersSetter() {}
	virtual ~IAnimParametersSetter() {}

	/// @brief �A�j���[�V�����p�����[�^�Z�b�g
	/// @param _animatiionParameters �A�j���[�V�����p�����[�^
	virtual void SetAnimationParameters(AnimationParameters& _animatiionParameters) = 0;
};

