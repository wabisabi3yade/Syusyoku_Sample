#include "pch.h"
#include "TextureCollect.h"
#include "Texture.h"

bool TextureCollect::Load(const char* _pathName, std::string _texName)
{
	// ���Ƀe�N�X�`��������Ȃ�ǂݍ��܂Ȃ�
	if (GetIsImported(_texName))
	{
		return true;
	}
	
	// �e�N�X�`���N���X���쐬���āA���[�h���s��
	std::unique_ptr<Texture> tex = std::make_unique<Texture>();
	bool isSucess = tex->Load(_pathName);
	if (!isSucess)
		return false;

	// ����������A�z��ɓ����
	resourceList.emplace(_texName, std::move(tex));

	return true;
}
