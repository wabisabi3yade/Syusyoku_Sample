#include "pch.h"
#include "VertexShader.h"
#include "Direct3D11.h"

void VertexShader::MakeShader(const char* _pData, u_int _dataSize)
{
	// ���_�V�F�[�_�[���쐬
	ID3D11VertexShader* ptr = pVxShader.get();
	Direct3D11::GetInstance()->GetRenderer()->GetDevice()
		->CreateVertexShader(_pData, _dataSize, nullptr, &ptr);
}

VertexShader::VertexShader() : Shader(Shader::Type::Vertex)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// �f�o�C�X�R���e�L�X�g�ɒ��_�V�F�[�_�[���Z�b�g
	pContext->VSSetShader(pVxShader.get(), nullptr, 0);
	// �萔�o�b�t�@�𑗂�
	for (int i = 0; i < pBuffers.size(); i++)
		pContext->VSSetConstantBuffers(i, 1, &pBuffers[i]);
	// ���\�[�X�𑗂�
	for (int i = 0; i < pTextures.size(); i++)
		pContext->VSSetShaderResources(i, 1, &pTextures[i]);
}
