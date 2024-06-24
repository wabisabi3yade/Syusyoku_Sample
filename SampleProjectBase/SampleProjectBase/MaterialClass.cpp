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

	parameter.isTextureEnable = false;	// ������
}

MaterialClass::~MaterialClass()
{
}

