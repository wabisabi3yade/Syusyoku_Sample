#pragma once

// ���f�����C���|�[�g������Ƃ��̐ݒ�
class ModelSettings
{
public:
	bool isRighthand;	// �E��n���W���ǂ���
	bool isPermanent;	// �ړ������Ƃ��ɉ������Ȃ��i�����Ǝc��j
	float scaleBase;	// �C���|�[�g����Ƃ��̃��f���̑傫��

	const char* modelPath;	// ���f���t�@�C���̃p�X
	std::string modelName;	// ���f���̖��O

	ModelSettings(): isRighthand(false), isPermanent(false), scaleBase(1.0f), modelPath(nullptr), modelName("") {};
	ModelSettings(const char* _modelPath, std::string _modelName, float _scaleBase,
		bool _isRighthand = false, bool _isPermanent = false);
	~ModelSettings();
};