#pragma once

#include "Component.h"

namespace HashiTaku
{
	// �R���Z�v�g�Ŏ擾�ł��郊�\�[�X�̌^���w��
	template<typename T>
	concept ComponentConcept = std::is_base_of_v<Component, T>;
}
