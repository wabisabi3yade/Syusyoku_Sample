#pragma once

/// @brief �A�j���[�V�����Ԃ̕�ԏ���
class IAnimInterpolate
{
public:
	IAnimInterpolate() {}
	~IAnimInterpolate() {}

	// �o�ߎ��Ԃ��擾����
	virtual float GetElapsedTime() const = 0;

	// �J�ڎ��Ԃ��擾����
	virtual float GetTransitionTime() const = 0;
};