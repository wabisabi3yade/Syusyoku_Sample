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

		// 解析用のリフレクション作成
		ID3D11ShaderReflection* pReflection;
		hr = D3DReflect(_pData, _dataSize, IID_ID3D11ShaderReflection, (void**)&pReflection);
		if (FAILED(hr)) { return; }

		// 定数バッファ作成
		D3D11_SHADER_DESC shaderDesc;
		pReflection->GetDesc(&shaderDesc);
		pBuffers.resize(shaderDesc.ConstantBuffers, nullptr);
		for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
		{
			// シェーダーの定数バッファの情報を取得
			D3D11_SHADER_BUFFER_DESC shaderBufDesc;
			ID3D11ShaderReflectionConstantBuffer* cbuf = pReflection->GetConstantBufferByIndex(i);
			cbuf->GetDesc(&shaderBufDesc);

			// 作成するバッファの情報
			D3D11_BUFFER_DESC bufDesc = {};
			bufDesc.ByteWidth = shaderBufDesc.Size;
			bufDesc.Usage = D3D11_USAGE_DEFAULT;
			bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

			// バッファの作成
			hr = pDevice->CreateBuffer(&bufDesc, nullptr, &pBuffers[i]);
			if (FAILED(hr)) { return; }
		}
		// テクスチャ領域作成
		pTextures.resize(shaderDesc.BoundResources, nullptr);

		return MakeShader(_pData, _dataSize);
	}

	void Shader::LoadCsoFile(const char* _filePath)
	{
		HRESULT hr = E_FAIL;

		// ファイルを読み込む
		FILE* fp;
		fopen_s(&fp, _filePath, "rb");
		if (!fp) { return; }

		// ファイルのサイズを調べる
		int fileSize = 0;
		fseek(fp, 0, SEEK_END);
		fileSize = ftell(fp);

		// メモリに読み込み
		fseek(fp, 0, SEEK_SET);
		char* pData = new char[fileSize];
		fread(pData, fileSize, 1, fp);
		fclose(fp);

		// シェーダー作成
		MakeBuffer(pData, fileSize);

		// 終了処理
		if (pData) { delete[] pData; }
		return;
	}

	void Shader::UpdateSubResource(u_int _slot, void* _pData)
	{
		if (_slot >= pBuffers.size()) return;

		// Map関数は非同期処理でのデッドロック問題がUpdateSubResourceより複雑なので、
		// 今回はUpdateSubesourceを使用する
		ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

		pDeviceContext->UpdateSubresource(
			pBuffers[_slot],		// 更新対象のリソース
			0,	// サブリソース(定数バッファは0)
			nullptr, // 送る対象範囲（全体を送るのでnullptr）
			_pData,	// 送る情報
			0,	// 行ピッチ
			0	// 深度バッファピッチ
		);
	}

	void Shader::Map(u_int _slot, void* _pData, size_t _dataSize)
	{
		if (_slot >= pBuffers.size()) return;

		ID3D11DeviceContext* pDeviceContext = Direct3D11::GetInstance()->
			GetRenderer()->GetDeviceContext();

		//定数バッファ書き換え
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

		if (_slot >= pTextures.size()) return;	// スロット番号がテクスチャ配列上限より上なら

		ID3D11ShaderResourceView* pSetSrv = &_texture->GetSRV();
		if (pSetSrv == nullptr) return;

		pTextures[_slot] = pSetSrv;	// 配列に追加

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