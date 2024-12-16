#include "pch.h"
#include "Shader.h"
#include <d3dcompiler.h>
#include "Texture.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

namespace HashiTaku
{
	void Shader::MakeBuffer(const char* _pData, u_int _dataSize)
	{
		HRESULT hr;
		ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

		// ��͗p�̃��t���N�V�����쐬
		ID3D11ShaderReflection* pReflection;
		hr = D3DReflect(_pData, _dataSize, IID_ID3D11ShaderReflection, (void**)&pReflection);
		if (FAILED(hr)) { return; }

		// �萔�o�b�t�@�쐬
		D3D11_SHADER_DESC shaderDesc;
		pReflection->GetDesc(&shaderDesc);
		pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);
		for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
		{
			// �V�F�[�_�[�̒萔�o�b�t�@�̏����擾
			D3D11_SHADER_BUFFER_DESC shaderBufDesc;
			ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);
			cbuf->GetDesc(&shaderBufDesc);

			// �쐬����o�b�t�@�̏��
			D3D11_BUFFER_DESC bufDesc = {};
			bufDesc.ByteWidth = shaderBufDesc.Size;
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			// �o�b�t�@�̍쐬
			hr = pDevice->CreateBuffer(&bufDesc, nullptr, &pBuffers[i]);
			if (FAILED(hr)) { return; }
		}
		// �e�N�X�`���̈�쐬
		pTextures.resize(shaderDesc.BoundResources, nullptr);

		return MakeShader(_pData, _dataSize);
	}

	void Shader::LoadCsoFile(const char* _filePath)
	{
		HRESULT hr = E_FAIL;

		// �t�@�C����ǂݍ���
		FILE* fp;
		fopen_s(&fp, _filePath, "rb");
		if (!fp) { return; }

		// �t�@�C���̃T�C�Y�𒲂ׂ�
		int fileSize = 0;
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);

		// �������ɓǂݍ���
		fseek(fp, 0, SEEK_SET);
		char* pData = new char[fileSize];
		fread(pData, fileSize, 1, fp);
		fclose(fp);

		// �V�F�[�_�[�쐬
		MakeBuffer(pData, fileSize);

		// �I������
		if (pData) { delete[] pData; }
		return;
	}

	void Shader::UpdateSubResource(u_int _slot, void* _pData)
	{
		if (_slot >= pBuffers.size()) return;

		// Map�֐��͔񓯊������ł̃f�b�h���b�N��肪UpdateSubResource��蕡�G�Ȃ̂ŁA
		// �����UpdateSubesource���g�p����
		ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

		pDeviceContext->UpdateSubresource(
			pBuffers[_slot],		// �X�V�Ώۂ̃��\�[�X
			0,	// �T�u���\�[�X(�萔�o�b�t�@��0)
			nullptr, // ����Ώ۔͈́i�S�̂𑗂�̂�nullptr�j
			_pData,	// ������
			0,	// �s�s�b�`
			0	// �[�x�o�b�t�@�s�b�`
		);
	}

	void Shader::Map(u_int _slot, void* _pData, size_t _dataSize)
	{
		if (_slot >= pBuffers.size()) return;

		ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->
			GetRenderer()->GetDeviceContext();

		//�萔�o�b�t�@��������
		D3D11_MAPPED_SUBRESOURCE msr;

		HRESULT hr = pDeviceContext->Map(
			pBuffers[_slot],
			0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (SUCCEEDED(hr))
		{
			memcpy(msr.pData, &_pData, _dataSize);
			pDeviceContext->Unmap(pBuffers[_slot], 0);
		}
	}

	void Shader::SetTexture(u_int _slot, const Texture* _texture)
	{
		if (_texture == nullptr) return;

		if (_slot >= pTextures.size()) return;	// �X���b�g�ԍ����e�N�X�`���z��������Ȃ�

		ID3D11ShaderResourceView* pSetSrv = &_texture->GetSRV();
		if (pSetSrv == nullptr) return;

		pTextures[_slot] = pSetSrv;	// �z��ɒǉ�

		ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();
		switch (type)
		{
		case Type::Vertex: pContext->VSSetShaderResources(_slot, 1, &pSetSrv); break;
		case Type::Pixel: pContext->PSSetShaderResources(_slot, 1, &pSetSrv); break;
		}
	}

	void Shader::SetName(const std::string& _name)
	{
		shaderName = _name;
	}

	Shader::Type Shader::GetShaderType() const
	{
		return type;
	}

	Shader::BufferType Shader::GetBufferType(u_int _slot)
	{
		return bufferTypes[_slot];
	}

	u_int Shader::GetBufferNum()
	{
		return static_cast<u_int>(bufferTypes.size());
	}

	std::string Shader::GetShaderName() const
	{
		return shaderName;
	}
}