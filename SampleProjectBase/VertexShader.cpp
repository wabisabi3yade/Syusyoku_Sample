#include "pch.h"
#include "VertexShader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

void VertexShader::MakeShader(const char* _pData, u_int _dataSize)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	ID3D11Device* pDevice = renderer->GetDevice();
	HRESULT hr;

	// ���_�V�F�[�_�[���쐬
	hr = pDevice->CreateVertexShader(_pData, _dataSize, nullptr, &pVxShader);
	if (FAILED(hr))
	{
		HASHI_DEBUG_LOG("���_�V�F�[�_�[�쐬���s");
		return;
	}

	MakeInputLayout(_pData, _dataSize);
}

void VertexShader::MakeInputLayout(const char* _pData, u_int _dataSize)
{
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	ID3D11Device* pDevice = renderer->GetDevice();

	// ���̓��C�A�E�g�I�u�W�F�N�g�쐬
	// ���@���_�V�F�[�_�[�ɂ͂ǂ��������f�[�^�\���̒��_�f�[�^���n���Ă��邩���`�������̓��C�A�E�g
	// Vertex�\���̂̃����o�ϐ��̃T�C�Y���w�肵�āA�V�F�[�_�[�̉��̏��Ȃ̂��𔻕ʂ���
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	HRESULT hr = pDevice->CreateInputLayout(
		layout,
		_countof(layout),
		_pData,
		_dataSize,
		&pInputLayout
	);

	if (FAILED(hr))
	{
		HASHI_DEBUG_LOG("���s");
		return;
	}
}

VertexShader::VertexShader() : Shader(Shader::Type::Vertex), pVxShader(nullptr)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// �f�o�C�X�R���e�L�X�g�ɒ��_�V�F�[�_�[���Z�b�g
	pContext->VSSetShader(pVxShader.Get(), nullptr, 0);

	// ���̓��C�A�E�g���Z�b�g
	pContext->IASetInputLayout(pInputLayout.Get());

	// �萔�o�b�t�@�𑗂�
	for (int i = 0; i < pBuffers.size(); i++)
		pContext->VSSetConstantBuffers(i, 1, &pBuffers[i]);

	// ���\�[�X�𑗂�
	for (int i = 0; i < pTextures.size(); i++)
		pContext->VSSetShaderResources(i, 1, &pTextures[i]);
}
