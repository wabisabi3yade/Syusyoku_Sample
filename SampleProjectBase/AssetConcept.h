#pragma once

// �A�Z�b�g
#include "Asset_Base.h"

// ���{����̖��O���
namespace HashiTaku
{
	// �R���Z�v�g�Ŏ擾�ł��郊�\�[�X�̌^���w��
	template<typename T>
	concept AssetConcept = std::is_base_of_v<Asset_Base, T>;
}