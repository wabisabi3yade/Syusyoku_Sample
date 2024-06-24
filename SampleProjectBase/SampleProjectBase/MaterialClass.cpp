#include "pch.h"
#include "MaterialClass.h"
#include "ShaderCollection.h"

using namespace DirectX::SimpleMath;
MaterialClass::MaterialClass()
{
	// デフォルトのシェーダーを初期設定しておく
	ShaderCollection* shCol =  ShaderCollection::GetInstance();
	pVertexShader = shCol->GetVertexShader(shCol->defaultVS);
	pPixelShader = shCol->GetPixelShader(shCol->defaultPS);

	parameter.isTextureEnable = false;	// 初期化
}

MaterialClass::~MaterialClass()
{
}

