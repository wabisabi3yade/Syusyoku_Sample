#include "pch.h"
#include "ModelLoader.h"

#define MODEL_NC_NUM (2)	// ���f���̖����K���Ŏg���镶����

bool ModelLoader::Load(const ModelSettings& _modelData)
{
	// ���\�[�X�Ǘ��Ɋ��Ƀ��[�h����Ă��邩�m�F����
	// ���f������M_�����Ă��Ȃ����
	std::string setName = _modelData.modelName;
	
	// ���f���̓�����2������ M_ �łȂ��Ȃ�
	if (setName.size() >= MODEL_NC_NUM && setName.substr(0, MODEL_NC_NUM) != "M_")
	{
		setName = "M_" + setName;	// ���O�̍ŏ��ɖ����K��������
	}

	// ���\�[�X�Ǘ��Ɋ��ɂ��邩�m�F����
	ResourceCollection* pReCollecter = ResourceCollection::GetInstance();
	if (pReCollecter->GetImpotred(setName))
	{
		return true;	// ���Ƀ��[�h����Ă���Ȃ珈���𔲂���
	}

	// ���f�������[�h����
	std::unique_ptr<Model> model = std::make_unique<Model>();
	bool isSuccess = model->Load(_modelData);
	if (!isSuccess)
		return false;	// ���s������I��

	// ���\�[�X�Ǘ��ɃZ�b�g����
	pReCollecter->SetResource<Model>(setName, std::move(model));
	return true;
}
