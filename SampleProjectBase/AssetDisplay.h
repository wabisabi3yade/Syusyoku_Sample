#pragma once
#include "AssetContacter.h"

// �A�Z�b�g�̃C���|�[�g���ꂽ���̂�\������
class AssetDisplay : private AssetContacter
{
	AssetDisplay() {}
	~AssetDisplay() {}
public:

	/// @brief �A�Z�b�g�̒��g��`��
	static void Draw();
};

