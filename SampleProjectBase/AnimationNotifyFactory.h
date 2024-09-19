#pragma once
#include "AnimNotifyRespawner.h"

class AnimationNotifyFactory : public Singleton_Base<AnimationNotifyFactory>
{
	friend class Singleton_Base<AnimationNotifyFactory>;

	/// @brief �S�ẴA�j���[�V�����ʒm�N���X���X�g
	std::unordered_map<std::string, std::unique_ptr<AnimNotifyRespawner_Base>> animNotifyList;

#ifdef EDIT
	// �I�𒆂̖��O
	std::string selectName;	
#endif

public:

	/// @brief �ʒm�N���X�𐶐����A�Ԃ�
	/// @param _notifyName �ʒm�C�x���g�N���X��
	/// @return ���������C���X�^���X
	std::unique_ptr<AnimationNotify_Base> Create(const std::string& _notifyName);

	/// @brief �R���{�{�b�N�X�ō쐬����
	/// @brief Factory��
	/// @return �쐬����{�^���������ꂽ��Ԃ�
	bool ImGuiCombo(std::unique_ptr<AnimationNotify_Base>& _pCreateNotify);
private:
	AnimationNotifyFactory();
	~AnimationNotifyFactory() {}

	void Init();
	template<HashiTaku::AnimNotifyConcept T> void ResisterNotify();

	std::vector<const std::string*> GetNotifyNames() const;
};

template<HashiTaku::AnimNotifyConcept T>
inline void AnimationNotifyFactory::ResisterNotify()
{
	std::string notifyName = TYPENAME_ROUGH(T);
	std::unique_ptr<AnimNotifyRespawner_Base> pRespawner = std::make_unique<AnimNotifyRespawner<T>>();

	animNotifyList[notifyName] = std::move(pRespawner);
}
