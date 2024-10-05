#pragma once
#include "AnimationParameters.h"

/// @brief �e�I�u�W�F�N�g�֓n�����߂̃R�s�[���ƂȂ�A�j���[�V�����p�����[�^
class OriginAnimationParameters : public AnimationParameters
{
	/// @brief �p�����[�^���X�g
	std::unordered_map<std::string, HashiTaku::AnimParam::conditionValType> animParameters;

	/// @brief �p�����[�^�폜���̒ʒm�T�u�W�F�N�g
	std::unique_ptr<HashiTaku::Subject<HashiTaku::AnimParam::NotificationData>> pAnimParamSubject;
public:
	OriginAnimationParameters();
	~OriginAnimationParameters() {}


};

