#pragma once
#include "AnimationNotify_Base.h"

/// @brief �C���X�^���X�𐶐�������N���X
class AnimNotifyRespawner_Base
{
public:
	AnimNotifyRespawner_Base() {}
	virtual ~AnimNotifyRespawner_Base() {}

	virtual std::unique_ptr<AnimationNotify_Base> Create() = 0;
};

/// @brief �C���X�^���X�𐶐�����N���X
/// @tparam T �����������N���X
template <HashiTaku::AnimNotifyConcept T>
class AnimNotifyRespawner : public AnimNotifyRespawner_Base
{
public:
	AnimNotifyRespawner() {}
	~AnimNotifyRespawner() {}

	/// @brief �w�肵���N���X�̃C���X�^���X�𐶐�����
	/// @return�C���X�^���X�̃��j�[�N�|�C���^
	std::unique_ptr<AnimationNotify_Base> Create() override;
};

template<HashiTaku::AnimNotifyConcept T>
inline std::unique_ptr<AnimationNotify_Base> AnimNotifyRespawner<T>::Create()
{
	return std::make_unique<T>();
}
