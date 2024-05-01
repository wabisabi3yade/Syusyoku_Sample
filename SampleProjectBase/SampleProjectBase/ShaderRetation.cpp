#include "ShaderRetation.h"

ShaderRetation::ShaderRetation()
{
}

ShaderRetation::~ShaderRetation()
{
	// �������
	SAFE_RELEASE(pVertexShader);
	SAFE_RELEASE(pPixelShader);
	SAFE_RELEASE(pInputLayout);
}

void ShaderRetation::SetVertexShader(ID3D11VertexShader* _pSetVertex)
{
	SAFE_RELEASE(pVertexShader);	// ��ɉ������

	// �A�h���X����
	pVertexShader = _pSetVertex;
}

void ShaderRetation::SetPixelShader(ID3D11PixelShader* _pSetPixel)
{
	SAFE_RELEASE(pPixelShader);

	// �A�h���X����
	pPixelShader = _pSetPixel;
}

void ShaderRetation::SetInputLayout(ID3D11InputLayout* _pSetInput)
{
	SAFE_RELEASE(pInputLayout);	// ��ɉ������

	// �A�h���X����
	pInputLayout = _pSetInput;
}


