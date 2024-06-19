#pragma once

// ���f�����C���|�[�g������Ƃ��̐ݒ�
class ModelSettings
{
public:
	bool isRighthand = false;	// �E��n���W���ǂ���
	bool isPermanent = false;	// �ړ������Ƃ��ɉ������Ȃ��i�����Ǝc��j

	const char* modelPath{ nullptr };	// ���f���t�@�C���̃p�X
	std::string modelName;	// ���f���̖��O

	ModelSettings();
	ModelSettings(const char* _modelPath, std::string _modelName,
		bool _isRighthand = false, bool _isPermanent = false);
	~ModelSettings();

	// ���f�������[�h����O�̏���
	void Setup(const char* _modelPath, std::string _modelName, 
		bool _isRighthand = false, bool _isPermanent = false);
};