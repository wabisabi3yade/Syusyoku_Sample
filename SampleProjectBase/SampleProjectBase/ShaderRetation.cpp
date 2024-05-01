#include "ShaderRetation.h"

ShaderRetation::ShaderRetation()
{
}

ShaderRetation::~ShaderRetation()
{
	// 解放する
	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(pInputLayout);
}

void ShaderRetation::SetVertexShader(ID3D11VertexShader* _pSetVertex)
{
	SAFE_RELEASE(pVertexShader);	// 先に解放する

	// アドレスを代入
	pVertexShader = _pSetVertex;
}

void ShaderRetation::SetPixelShader(ID3D11PixelShader* _pSetPixel)
{
	SAFE_RELEASE(pPixelShader);

	// アドレスを代入
	pPixelShader = _pSetPixel;
}

void ShaderRetation::SetInputLayout(ID3D11InputLayout* _pSetInput)
{
	SAFE_RELEASE(pInputLayout);	// 先に解放する

	// アドレスを代入
	pInputLayout = _pSetInput;
}


