#pragma once
#include "Texture.h"

// ���f���̕\�ʃf�[�^
class Material
{
public:
	Material();
	~Material();

	Float4 diffuse;
	Float4 ambient;
	Float4 specular;
	Texture* texture;	// �e�N�X�`��
};

