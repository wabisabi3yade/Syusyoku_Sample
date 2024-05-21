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
	bool isSucess  = loadModel->SetSetting(_settings);
	if (!isSucess)
		return false;

	// ���f�����}�b�v�z��ɓ����
	resourceList.emplace(_settings.modelName, std::move(loadModel));

	return true;
}
