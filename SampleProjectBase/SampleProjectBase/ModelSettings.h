#pragma once

// ���f�����C���|�[�g������Ƃ��̐ݒ�
class ModelSettings
{
public:
	bool isRighthand = false;	// �E��n���W���ǂ���

	const char* modelPath{ nullptr };	// ���f���t�@�C���̃p�X

	ModelSettings();
	~ModelSettings();
};