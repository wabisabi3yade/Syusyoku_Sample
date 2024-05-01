#include "Model.h"
#include "Mesh.h"
#include "Direct3D11.h"

// assimpライブラリ読込
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const ModelSettings& _settings)
{
	Load(_settings, *Direct3D11::GetInstance()->GetRenderer());
}

Model::~Model()
{
	Release();
}

bool Model::Load(const ModelSettings& _settings, D3D11_Renderer& _renderer)
{
	if (_settings.modelPath == nullptr)
	{
		MessageError("モデルのファイルパスが設定されていません");
		return false;
	}

	//モデルのロード処理
	Assimp::Importer importer;
	u_int flag = 0;
	flag |= aiProcess_Triangulate;	// 三角形ポリゴン
	flag |= aiProcess_JoinIdenticalVertices;	// 同じ頂点を一つに統合
	/*flag |= aiProcess_FlipUVs;*/
	/*flag |= aiProcess_PreTransformVertices;*/

	if(_settings.isRighthand)
		flag |= aiProcess_MakeLeftHanded;		// 左手系座標に変換

	// モデルを読み込む
	auto pScene = importer.ReadFile(_settings.modelPath, flag);
	if (pScene == nullptr) return false;

	// メッシュの数を取得する
	meshNum = pScene->mNumMeshes;

	if (meshNum > 0)
	{	
		// メッシュの数準備をする
		for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
		{
			Mesh* mesh = new Mesh();	// メッシュ作成
			// メッシュの情報を取得
			auto pMeshData = pScene->mMeshes[meshIdx];

			// メッシュの準備
			if (mesh->Setup(_renderer, pMeshData) == false)
			{
				MessageError("メッシュのセットアップ失敗");
				// 作成したメッシュを解放
				CLASS_DELETE(mesh);

				return false;
			}

			pMeshes.push_back(mesh);
		}
	}

	return true;
}

void Model::SetupTransform()
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	CbTransformSet& cb = renderer.GetParameter().cbTransformSet;

	auto mtx = DirectX::XMMatrixIdentity();

	DirectX::XMStoreFloat4x4(&cb.data.transform, XMMatrixTranspose(mtx));
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// CBufferにひもづくハードウェアリソースマップ取得（ロックして取得）
	auto pDeviceContext = renderer.GetDeviceContext();

	HRESULT hr = pDeviceContext->Map(
		cb.pBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);
	if (FAILED(hr)) 
	{
		return;
	}

	CopyMemory(mappedResource.pData, &cb.data, sizeof(cb.data));
	// マップ解除
	pDeviceContext->Unmap(cb.pBuffer, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &cb.pBuffer);

	return;
}

void Model::Draw()
{
	SetupTransform();
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		pMeshes[meshIdx]->Draw(renderer);
	}
}

void Model::Release()
{
	for (auto mesh : pMeshes)
	{
		CLASS_DELETE(mesh);
	}
}
