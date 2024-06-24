#include "pch.h"
#include "Mesh.h"
#include "D3D11_Renderer.h"

// assimpライブラリ読込
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


bool Mesh::CreateVertexBuffer(D3D11_Renderer& _renderer)
{
	// バッファ定義の構造体を設定
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexNum;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// サブリソースに頂点データを代入
	D3D11_SUBRESOURCE_DATA vertexSubData;
	vertexSubData.pSysMem = pVertices;

	// バッファを作成する
	auto hr = _renderer.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexSubData,
		&pVertexBuffer
	);

	if (FAILED(hr))
		return false;

	return true;
}

bool Mesh::CreateIndexBuffer(D3D11_Renderer& _renderer)
{
	// インデックスデータのバッファ
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = indexNum * 4;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubData;
	indexSubData.pSysMem = pIndicies;

	auto hr = _renderer.GetDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexSubData,
		&pIndexBuffer
	);

	if (FAILED(hr))
		return false;


	return true;
}

Mesh::Mesh() : vertexNum(0), indexNum(0), materialIndex(0)
{
}

Mesh::~Mesh()
{
	Release();
}

bool Mesh::SetMesh(const Mesh& _setMesh)
{
	return false;
}

bool Mesh::Setup(D3D11_Renderer& _renderer, aiMesh* pMeshData, float _scaleBase)
{
	// 頂点データを取得 //

	vertexNum = pMeshData->mNumVertices;
	// 頂点 作成
	pVertices = new Vertex[vertexNum];

	aiVector3D zero(0.f, 0.f, 0.f);

	for (u_int vertexIdx = 0; vertexIdx < vertexNum; vertexIdx++)
	{
		// 頂点の座標を取得して、頂点構造体へ代入する
		auto& pos = pMeshData->mVertices[vertexIdx];
		pVertices[vertexIdx].position = { pos.x, pos.y, pos.z };
		// インポート時の大きさ倍率を掛けてあげる
		pVertices[vertexIdx].position *= _scaleBase;

		// 法線べクトルを取得
		auto& normal = pMeshData->HasNormals() ? pMeshData->mNormals[vertexIdx] : zero;
		pVertices[vertexIdx].normal = { normal.x, normal.y, normal.z };

		// UV座標を取得
		auto& uv = pMeshData->HasTextureCoords(0) ? pMeshData->mTextureCoords[0][vertexIdx] 
			: zero;
		pVertices[vertexIdx].uv = { uv.x, uv.y };

		constexpr float COLOR = 1.0f;
		pVertices[vertexIdx].color = { COLOR, COLOR, COLOR, 1.0f };
	}

	// 頂点シェーダー作成
	if (CreateVertexBuffer(_renderer) == false)
	{
		return false;
	}

	// 頂点インデックスデータを取得 // 
	indexNum = pMeshData->mNumFaces * 3;	// 1ポリゴンに3個のインデックス
	pIndicies = new u_int[indexNum];

	// インデックスを割り振る
	for (u_int faceIdx = 0; faceIdx < pMeshData->mNumFaces; faceIdx++)
	{
		auto& face = pMeshData->mFaces[faceIdx];
		assert(face.mNumIndices == 3);	// 面のインデックス数が3であることを確認

		for (u_int idx = 0; idx < 3; idx++)
		{
			// この面の頂点インデックスを取得する(0,1,2,0,1,2・・・)
			pIndicies[faceIdx * 3 + idx] = face.mIndices[idx];
		}

	}

	if (CreateIndexBuffer(_renderer) == false)
		return false;

	// マテリアルの割り当て
	materialIndex = pMeshData->mMaterialIndex;

	return true;
}

void Mesh::Draw(D3D11_Renderer& _renderer)
{
	auto pDeviceContext = _renderer.GetDeviceContext();
	u_int strides[1] = { sizeof(Vertex) };	// 頂点サイズ
	u_int offset[1] = { 0 };

	// 描画に使うモデル・インデックスバッファを指定する
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, strides, offset);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// インデックスバッファを利用して描画する
	// 第1引数：描画する頂点数
	pDeviceContext->DrawIndexed(indexNum, 0, 0);
}

void Mesh::SetVertexData(Vertex* _pVerticies, u_int _vertexNum, u_int* _pIndecies, u_int _indexNum)
{
	Release();	// 前残っているバッファ等を解放する

	// 頂点
	pVertices = _pVerticies;
	vertexNum = _vertexNum;

	// インデックス
	pIndicies = _pIndecies;
	indexNum = _indexNum;

	// 頂点バッファ・インデックスバッファを作成
	D3D11_Renderer* renderer =  Direct3D11::GetInstance()->GetRenderer();
	CreateVertexBuffer(*renderer);
	CreateIndexBuffer(*renderer);
}

void Mesh::Release()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);

	CLASS_DELETE(pVertices);
	CLASS_DELETE(pIndicies);
}
