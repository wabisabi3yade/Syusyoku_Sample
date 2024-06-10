#include "pch.h"
#include "Shader.h"
#include <d3dcompiler.h>
#include "Direct3D11.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

void Shader::MakeBuffer(const char* _pData, u_int _dataSize)
{
	HRESULT hr;
	// シェーダーに含む情報にアクセスするためのリフレクション情報を取得
	ID3D11ShaderReflection* pReflection;
	hr = D3DReflect(_pData, _dataSize, IID_ID3D11ShaderReflection, (void**)&pReflection);
	if (FAILED(hr))
	{
		ImGuiDebugLog::AddDebugLog("リフレクション情報取得に失敗");
		return;
	}

	// シェーダー作成でのプロパティなどの構造体
	D3D11_SHADER_DESC desc;
	pReflection->GetDesc(&desc);	// 取得
	pBuffers.resize(desc.ConstantBuffers);	// バッファの数分配列を確保する

	ID3D11Device* pDevice = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	// 定数バッファ情報を取得し、バッファを作成する
	for (u_int i = 0; i < desc.ConstantBuffers; i++)
	{
		// シェーダーの定数バッファ取得する
		D3D11_SHADER_BUFFER_DESC shaderBufDesc;
		ID3D11ShaderReflectionConstantBuffer* refCBuffer = pReflection->GetConstantBufferByIndex(i);
		refCBuffer->GetDesc(&shaderBufDesc);

		// 作成する
		// バッファ情報を作成
		D3D11_BUFFER_DESC bufDesc = {};
		bufDesc.ByteWidth = shaderBufDesc.Size;	// サイズ
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		// バッファ作成
		hr = pDevice->CreateBuffer(&bufDesc, nullptr, &pBuffers[i]);
		if (FAILED(hr))
		{
			ImGuiDebugLog::AddDebugLog("頂点シェーダーで定数バッファ作成失敗");
			return;
		}
	}

	// テクスチャのサイズを確保
	pTextures.resize(desc.BoundResources, nullptr);

	// 各シェーダーを作成する
	MakeShader(_pData, _dataSize);
}
void Shader::LoadCsoFile(const char* _filePath)
{
	HRESULT hr = E_FAIL;

	// ファイルを読み込む
	FILE* fp;
	fopen_s(&fp, _filePath, "rb");
	if (!fp)	// ファイルがなければ
	{
		ImGuiDebugLog::AddDebugLog("読み込もうとした " + std::string(_filePath) + " がありません");
		return;
	}

	// ファイルのサイズを調べる
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);	// 最後読み取り位置に移動する	
	fileSize = ftell(fp);	// ファイルサイズ取得

	// メモリに読み込み
	fseek(fp, 0, SEEK_SET);	// 最初に移動
	char* pData = new char[fileSize];
	fread(pData, fileSize, 1, fp);
	fclose(fp);

	// シェーダー作成
	MakeShader(pData, fileSize);

	// 解放処理
	if (pData)
	{
		delete[] pData;
	}

}

void Shader::UpdateBuffer(u_int _slot, void* _pData)
{
	if (_slot >= pBuffers.size()) return;

	// Map関数は非同期処理でのデッドロック問題がUpdateSubResourceより複雑なので、
	// 今回はUpdateSubesourceを使用する
	Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext()
		->UpdateSubresource(
			pBuffers[_slot],		// 更新対象のリソース
			0,	// サブリソース(定数バッファは0)
			nullptr, // 送る対象範囲（全体を送るのでnullptr）
			_pData,	// 送る情報
			0,	// 行ピッチ
			0	// 深度バッファピッチ
		);
}