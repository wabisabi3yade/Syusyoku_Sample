#pragma once
#include "AnimationNotify_Base.h"

namespace HashiTaku
{
	// �R���Z�v�g�Ŏ擾�ł��郊�\�[�X�̌^���w��
	template<typename T>
	concept NotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;
}