#include "pch.h"
#include "ModelCollect.h"
#include "ModelSettings.h"
#include "Model.h"

bool ModelCollect::Load(const ModelSettings& _settings)
{
	// ���f���i�[�N���X�Ɋ��Ƀ��[�h����Ă���Ȃ烍�[�h�������Ȃ�
	bool isImported = ModelCollect::GetInstance()->GetIsImported(_settings.modelName);
	if (isImported)
		return true;

	// ���[�h����郂�f�����
	std::unique_ptr<Model> loadModel = std::make_unique<Model>();
	// ���f���̃��[�h�������s��
	bool isSucess  = loadModel->Load(_settings);
	if (!isSucess)
		return false;

	// ���f�����}�b�v�z��ɓ����
	resourceList.emplace(_settings.modelName, std::move(loadModel));

	return true;
}

std::unique_ptr<Model> ModelCollect::GetModel(std::string _modelName)
{
	// �f�[�^���̃��f�����擾����
	const Model* pOriginModel = GetConstResource(_modelName);

	// ���f�����Ȃ������Ȃ�
	if (pOriginModel == nullptr)
	{
		std::string message = "���f�����܂����[�h����Ă��܂���" + _modelName;
		ImGuiDebugLog::AddDebugLog(message);

		return nullptr;	// nullptr��Ԃ�
	}

	// ����V�����|�C���^���m�ۂ���
	std::unique_ptr<Model> pSetModel = std::make_unique<Model>();
	pSetModel->SetModel(*pOriginModel);	// ���̃��f�����烁�b�V���Ȃǂ̏����R�s�[����

	// �A�h���X���ړ�������
	return std::move(pSetModel);
}
