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

private:
	static void Display(const std::unordered_map<std::string, std::unique_ptr<Asset_Base>>& _assets);
	static void DisplayTexture();
	static void DisplayModel();
	static void DisplayMaterial();
	static void DisplayAnimation();
	static void DisplayBoneList();
};
