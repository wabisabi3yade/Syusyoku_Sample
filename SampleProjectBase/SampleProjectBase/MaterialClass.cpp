#include "pch.h"
#include "MaterialClass.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;
MaterialClass::MaterialClass()
{
	// �f�t�H���g�̃V�F�[�_�[�������ݒ肵�Ă���
	ShaderCollection* shCol =  ShaderCollection::GetInstance();
	pVertexShader = shCol->GetVertexShader(shCol->defaultVS);
	pPixelShader = shCol->GetPixelShader(shCol->defaultPS);

	// ������
	parameter.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.emissive = Color(1.0f, 1.0f, 1.0f, 1.0f);
	parameter.shininess = 0.0f;
	parameter.isTextureEnable = false;	
}

MaterialClass::~MaterialClass()
{
}

