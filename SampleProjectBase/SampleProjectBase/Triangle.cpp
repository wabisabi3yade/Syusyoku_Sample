#include "pch.h"
#include "Triangle.h"
#include "D3D11_Renderer.h"

Triangle::Triangle()
{
	// 座標
	auto sqrt3 = sqrtf(3);
	vertices[0].position = { 0.f, sqrt3 / 3.f, 0.f };
	vertices[1].position = { 0.5f, -sqrt3 / 6.f, 0.f };
	vertices[2].position = { -0.5f, -sqrt3 / 6.f, 0.f };

	// 色
	constexpr float ALPHA = 0.5f;
	vertices[0].color = { 1.f, 0.f, 0.f, ALPHA };
	vertices[1].color = { 0.f, 1.f, 0.f, ALPHA };
	vertices[2].color = { 0.f, 0.f, 1.f, ALPHA };
}

Triangle::~Triangle()
{
	SAFE_RELEASE(pVertexBuffer);
}

bool Triangle::CreateVertexBuffer(D3D11_Renderer& _renderer)
{
	// 頂点バッファ定義の構造体を設定
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// データ本体であるサブリソース定義を頂点データで初期化
	// サブリソース定義 → DirectXではバッファリソースを作成するとき、必ずサブリソース定義を介する。
	D3D11_SUBRESOURCE_DATA vertexSubData;
	vertexSubData.pSysMem = vertices;

	// 頂点バッファを取得
	auto hr = _renderer.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexSubData,
		&pVertexBuffer
	);

	if (FAILED(hr)) return false;

	return true;
}

void Triangle::SetupTransform(D3D11_Renderer& _renderer)
{
	CbTransformSet& cb = _renderer.GetParameter().cbTransformSet;

	auto mtx = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0);

	DirectX::XMStoreFloat4x4(&cb.data.transform, XMMatrixTranspose(mtx));
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	//// CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
	auto pDeviceContext = _renderer.GetDeviceContext();

	HRESULT hr = pDeviceContext->Map(
		cb.pBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);
	if (FAILED(hr)) {
		//DXTRACE_ERR(L"DrawSceneGraph failed", hr);
		return;
	}

	CopyMemory(mappedResource.pData, &cb.data, sizeof(cb.data));
	// マップ解除
	pDeviceContext->Unmap(cb.pBuffer, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &cb.pBuffer);

}

void Triangle::Draw(D3D11_Renderer& _renderer)
{
	auto pDeviceContext = _renderer.GetDeviceContext();
	// Drawする頂点バッファ（モデル）を指定する
	uint32_t strides = sizeof(Vertex);   // 頂点のサイズ
	uint32_t offsets = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &strides, &offsets);
	// 変換行列
	SetupTransform(_renderer);

	// 引数に頂点数、最初の頂点のインデックスを代入する
	pDeviceContext->Draw(VERTEX_NUM, 0);
}
