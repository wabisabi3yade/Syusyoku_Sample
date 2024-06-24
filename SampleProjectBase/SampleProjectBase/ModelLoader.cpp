#include "pch.h"
#include "ModelLoader.h"

#include <filesystem>

#define MODEL_NC_NUM (3)	// ���f���̖����K���Ŏg���镶����
#define TEX_NC_NUM (2)	// �e�N�X�`���̖����K���Ŏg���镶����

namespace fs = std::filesystem;

Model* ModelLoader::Load(const ModelSettings& _modelData)
{
	// ���\�[�X�Ǘ��Ɋ��Ƀ��[�h����Ă��邩�m�F����
	// ���f������M_�����Ă��Ȃ����
	std::string setName = _modelData.modelName;
	
	// ���O�̓�����3������ Md_ �łȂ��Ȃ�
	if (setName.size() >= MODEL_NC_NUM && setName.substr(0, MODEL_NC_NUM) != "Md_")
	{
		setName = "Md_" + setName;	// ���O�̍ŏ��ɖ����K��������
	}

	// ���\�[�X�Ǘ��Ɋ��ɂ��邩�m�F����
	ResourceCollection* pReCollecter = ResourceCollection::GetInstance();
	if (pReCollecter->GetImpotred(setName))
	{
		// ���Ƀ��[�h���ꂽ���f����Ԃ�
		return pReCollecter->GetResource<Model>(setName);	
	}

	// ���f�������[�h����
	std::unique_ptr<Model> model = std::make_unique<Model>();
	bool isSuccess = model->Load(_modelData);
	if (!isSuccess)
		return nullptr;	// ���s������I��

	Model* retModel = model.get();	// �Ԃ����f�����擾
	// ���\�[�X�Ǘ��ɃZ�b�g����
	pReCollecter->SetResource<Model>(setName, std::move(model));

	return retModel;
}

Texture* TextureLoader::Load(TextureSetting _texSetting)
{
	// ���\�[�X�Ǘ��Ɋ��Ƀ��[�h����Ă��邩�m�F����
	std::string& setName = _texSetting.setName;

	// ���\�[�X�Ǘ��Ɋ��ɂ��邩�m�F����
	ResourceCollection* pReCollecter = ResourceCollection::GetInstance();
	if (pReCollecter->GetImpotred(setName))
	{
		// ���Ƀ��[�h���Ă���e�N�X�`����Ԃ�
		return pReCollecter->GetResource<Texture>(setName);	
	}

	// �e�N�X�`�������[�h����
	std::unique_ptr<Texture> pTex = std::make_unique<Texture>();
	bool isSuccess = pTex->Load(_texSetting.pathName);
	if (!isSuccess)
		return nullptr;	// ���s������I��

	Texture* retTex = pTex.get();
	// ���\�[�X�Ǘ��ɃZ�b�g����
	pReCollecter->SetResource<Texture>(setName, std::move(pTex));

	return retTex;
}