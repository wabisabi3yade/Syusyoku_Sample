#pragma once
#include "AnimNotifyRespawner.h"
#include "AnimNotifyConcept.h"

class AnimationParameters;

class AnimationNotifyFactory
{
	/// @brief �S�ẴA�j���[�V�����ʒm�N���X���X�g
	std::unordered_map<AnimationNotify_Base::NotifyType, std::unique_ptr<AnimNotifyRespawner_Base>> animNotifyList;

	/// @brief �������ɕK�v�ɂȂ�A�j���[�V�����p�����[�^�A�h���X
	AnimationParameters* pAnimationParameters;
public:
	AnimationNotifyFactory(AnimationParameters& _animationParameters);
	~AnimationNotifyFactory() {}

	/// @brief �ʒm�N���X�𐶐����A�Ԃ�
	/// @param _notifyName �ʒm�C�x���g�N���X
	/// @return ���������C���X�^���X
	std::unique_ptr<AnimationNotify_Base> Create(AnimationNotify_Base::NotifyType _notifyType);

	/// @brief �R���{�{�b�N�X�ō쐬����
	/// @brief Factory�ō쐬�����ʒm
	/// @return �쐬����{�^���������ꂽ��Ԃ�
	bool ImGuiCombo(std::unique_ptr<AnimationNotify_Base>& _pCreateNotify);
private:
	void Init();

	/// @brief �C�x���g�ʒm��ǉ����āA���X�|�[�i�[���쐬����
	/// @tparam T �쐬����ʒm�C�x���g
	/// @param _createNotifyType ���
	template<HashiTaku::AnimNotifyConcept T> void ResisterNotify(AnimationNotify_Base::NotifyType _createNotifyType);
};

template<HashiTaku::AnimNotifyConcept T>
inline void AnimationNotifyFactory::ResisterNotify(AnimationNotify_Base::NotifyType _createNotifyType)
{
	std::unique_ptr<AnimNotifyRespawner_Base> pRespawner = std::make_unique<AnimNotifyRespawner<T>>();

	animNotifyList[_createNotifyType] = std::move(pRespawner);
}
