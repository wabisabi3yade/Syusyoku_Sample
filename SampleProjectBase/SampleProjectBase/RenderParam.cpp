#include "pch.h"

#include "D3D11_Renderer.h"
#include "RenderParam.h"


RenderParam::~RenderParam()
{
}

bool RenderParam::InitConstBuffer(ID3D11Device* _device)
{
	// �萔�o�b�t�@�̒�`
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// �萔�o�b�t�@�@�̍쐬
	// GPU�ɓn���f�[�^�T�C�Y���擾
	cbDesc.ByteWidth = sizeof(CbTransform);
	auto hr = _device->CreateBuffer(
		&cbDesc, 
		nullptr, 
		&cbTransformSet.pBuffer);
	if (FAILED(hr))
	{
		ImGuiDebugLog::AddDebugLog("���W�萔�o�b�t�@�̏������ŃG���[");
		return false;
	}

	// �r���[�ϊ��o�b�t�@���m�ۂ���
	cbDesc.ByteWidth = sizeof(CbView);
	hr = _device->CreateBuffer(
		&cbDesc,
		nullptr,
		&cbViewSet.pBuffer);
	if (FAILED(hr))
	{
		ImGuiDebugLog::AddDebugLog("�r���[�ϊ��萔�o�b�t�@�̏������ŃG���[");
		return false;
	}

	// �v���W�F�N�V�����̃o�b�t�@�m��
	cbDesc.ByteWidth = sizeof(CbProjection);
	hr = _device->CreateBuffer(
		&cbDesc,
		nullptr,
		&cbProjectionSet.pBuffer);

	if (FAILED(hr)) 
	{
		ImGuiDebugLog::AddDebugLog("�v���W�F�N�V�����萔�o�b�t�@�̏������ŃG���[");
		return false;
	}

	return true;
}

bool RenderParam::Init(ID3D11Device* _device)
{
	bool result = InitConstBuffer(_device);

	if (!result)
	{
		ImGuiDebugLog::AddDebugLog("RenderParam�N���X�̏������ŃG���[");
		return false;
	}

	return true;
}

