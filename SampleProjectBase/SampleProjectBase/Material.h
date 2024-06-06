#pragma once
#include "Texture.h"

// ���f���̕\�ʃf�[�^
class Material
{
public:
	Material();
	~Material();

	DirectX::SimpleMath::Vector4 diffuse;
	DirectX::SimpleMath::Vector4 ambient;
	DirectX::SimpleMath::Vector4 specular;
	const Texture* texture;	// �e�N�X�`��
};

