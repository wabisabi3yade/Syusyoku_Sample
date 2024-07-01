#include "pch.h"
#include "Shader.h"
#include <d3dcompiler.h>
#include "Texture.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

void Shader::MakeBuffer(const char* _pData, u_int _dataSize)
{
	HRESULT hr;
	// �V�F�[�_�[�Ɋ܂ޏ��ɃA�N�Z�X���邽�߂̃��t���N�V���������擾
	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(_pData, _dataSize, IID_ID3D11ShaderReflection, (void**)&pReflection);
	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("���t���N�V�������擾�Ɏ��s");
		return;
	}

	// �V�F�[�_�[�쐬�ł̃v���p�e�B�Ȃǂ̍\����
	D3D11_SHADER_DESC desc;
	pReflection->GetDesc(&desc);	// �擾
	pBuffers.resize(desc.ConstantBuffers);	// �o�b�t�@�̐����z����m�ۂ���

	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	// �萔�o�b�t�@�����擾���A�o�b�t�@���쐬����
	for (u_int i = 0; i < desc.ConstantBuffers; i++)
	{
		// �V�F�[�_�[�̒萔�o�b�t�@�擾����
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;
		ID3D11ShaderReflectionConstantBuffer* refCBuffer = pReflection->GetConstantBufferByIndex(i);
		refCBuffer->GetDesc(&shaderBufDesc);

		// �쐬����
		// �o�b�t�@�����쐬
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = shaderBufDesc.Size;	// �T�C�Y
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		// �o�b�t�@�쐬
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &pBuffers[i]);
		if (FAILED(hr))
		{
			ImGuiDebugLog::Add("�萔�o�b�t�@�쐬���s");
			return;
		}
	}

	// �e�N�X�`���̃T�C�Y���m��
	pTextures.resize(desc.BoundResources, nullptr);

	// �e�V�F�[�_�[���쐬����
	MakeShader(_pData, _dataSize);
}
void Shader::LoadCsoFile(const char* _filePath)
{
	HRESULT hr = E_FAIL;

	// �t�@�C����ǂݍ���
	FILE* fp;
	fopen_s(&fp, _filePath, "rb");
	if (!fp)	// �t�@�C�����Ȃ����
	{
		ImGuiDebugLog::Add("�ǂݍ������Ƃ��� " + std::string(_filePath) + " ������܂���");
		return;
	}

	// �t�@�C���̃T�C�Y�𒲂ׂ�
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);	// �Ō�ǂݎ��ʒu�Ɉړ�����	
	fileSize = ftell(fp);	// �t�@�C���T�C�Y�擾

	// �������ɓǂݍ���
	fseek(fp, 0, SEEK_SET);	// �ŏ��Ɉړ�
	char* pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	// �o�b�t�@�쐬
	MakeBuffer(pData, fileSize);

	// �������
	if (pData)
	{
		delete[] pData;
	}

}

void Shader::UpdateBuffer(u_int _slot, void* _pData)
{
	if (_slot >= pBuffers.size()) return;

	// Map�֐��͔񓯊������ł̃f�b�h���b�N��肪UpdateSubResource��蕡�G�Ȃ̂ŁA
	// �����UpdateSubesource���g�p����
	Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext()
		->UpdateSubresource(
			pBuffers[_slot],		// �X�V�Ώۂ̃��\�[�X
			0,	// �T�u���\�[�X(�萔�o�b�t�@��0)
			nullptr, // ����Ώ۔͈́i�S�̂𑗂�̂�nullptr�j
			_pData,	// ������
			0,	// �s�s�b�`
			0	// �[�x�o�b�t�@�s�b�`
		);
}

void Shader::SetTexture(u_int _slot, Texture* _texture)
{
#ifdef EDIT
	if (_texture == nullptr)
	{
		ImGuiDebugLog::Add("�e�N�X�`�����ݒ肳��Ă��܂���");
		return;
	}
#endif // EDIT

	if (_slot > pTextures.size()) return;	// �X���b�g�ԍ����e�N�X�`���z��������Ȃ�

	ID3D11ShaderResourceView* pSetSrv = _texture->GetSRV();
	if (pSetSrv == nullptr) return;

	pTextures[_slot] = pSetSrv;	// �z��ɒǉ�

	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();
	switch (type)
	{
		case Type::Vertex: pContext->VSSetShaderResources(_slot, 1, &pSetSrv); break;
		case Type::Pixel: pContext->PSSetShaderResources(_slot, 1, &pSetSrv); break;
	}
}

void Shader::SetName(std::string _name)
{
	name = _name;
}
