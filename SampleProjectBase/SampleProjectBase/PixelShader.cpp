#include "pch.h"
#include "PixelShader.h"
#include "Direct3D11.h"

void PixelShader::MakeBuffer()
{
	//HRESULT hr;
	//// �s�N�Z���V�F�[�_�[�쐬
	//ID3DBlob* pPsBlob = nullptr;
	//hr = D3DCompileFromFile(
	//	_pPsPath,
	//	nullptr,
	//	D3D_COMPILE_STANDARD_FILE_INCLUDE,
	//	"main",
	//	"ps_4_0",
	//	D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
	//	0,
	//	&pPsBlob,
	//	&errBlob
	//);

	//// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g���쐬
	//ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	//ID3D11PixelShader* pPixelShader = nullptr;
	//hr = pDevice->CreatePixelShader(
	//	pPsBlob->GetBufferPointer(),
	//	pPsBlob->GetBufferSize(),
	//	nullptr,
	//	&pPixelShader
	//);
	//if (FAILED(hr))
	//	MessageError("�s�N�Z���V�F�[�_�[�쐬���s");
}

PixelShader::PixelShader()
{
}

PixelShader::~PixelShader()
{
}
