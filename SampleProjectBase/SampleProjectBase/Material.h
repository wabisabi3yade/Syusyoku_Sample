#pragma once
#include "Texture.h"

class VertexShader;
class PixelShader;
// ���f���̕\�ʃf�[�^
class Material
{
public:
	Material();
	~Material();

	DirectX::SimpleMath::Color diffuse;	// �F
	DirectX::SimpleMath::Vector4 ambient;	// ����
	DirectX::SimpleMath::Vector4 specular;	// ���ʔ���
	DirectX::SimpleMath::Vector4 emissive;	// ������

	const Texture* texture;	// �e�N�X�`��
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;
};

