#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Direct3D11.h"

#include "TextureCollect.h"

// assimpライブラリ読込
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model() : meshNum(0), modelName(""), isPermanent(false), isImported(false)
{
	
}

Model::~Model()
{
	Release();
}

bool Model::Load(const ModelSettings& _settings, D3D11_Renderer& _renderer)
{
	// 既にこのモデルが入っていたら
	if (isImported)
		return true;

	if (_settings.modelPath == nullptr)
	{
		MessageError("モデルのファイルパスが設定されていません");
		return false;
	}
	if (_settings.modelName == "")
	{
		MessageError("モデルの名前が設定されていません");
		return false;
	}

	//モデルのロード処理
	Assimp::Importer importer;
	u_int flag = 0;
	flag |= aiProcess_Triangulate;	// 三角形ポリゴン
	flag |= aiProcess_JoinIdenticalVertices;	// 同じ頂点を一つに統合
	flag |= aiProcess_FlipUVs;	//　UV値をY軸を基準に反転させる
	flag |= aiProcess_PreTransformVertices;	// ノードを一つに統合 アニメーション情報が消えることに注意

	if (!_settings.isRighthand)
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

			meshes.push_back(mesh);
		}
	}

	// マテリアルの作成

	// ファイルの探索
	std::string dir = _settings.modelPath;
	dir = dir.substr(0, dir.find_last_of('/') + 1);

	aiColor3D color(0.0f, 0.0f, 0.0f);

	Float4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Float4 ambient(0.3f, 0.3f, 0.3f, 1.0f);
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		Material* material = new Material();

		// 各種パラメーター
		float shininess;
		material->diffuse = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;
		material->ambient = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;
		shininess = pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		material->specular = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		// テクスチャ
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			// ファイル形式チェック
			if (strstr(path.C_Str(), ".psd"))
			{
				MessageError("psdには対応していません");
			}

			bool isSuccess = false;
			// そのまま探索
			material->texture = new Texture();
			// モデルと同じ階層を探索
			std::string file = dir;
			file += path.C_Str();

			// テクスチャ管理クラスにモデルのテクスチャを代入する(名前はモデルと一緒)
			isSuccess = TextureCollect::GetInstance()->Load(file.c_str(), _settings.modelName);

			// ファイル名のみで探索
			if(!isSuccess) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					isSuccess = material->texture->Load(file.c_str());
				}

				// ファイル目のパスが"\\ではなく"/"の場合の対応
				if (!isSuccess) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						isSuccess = material->texture->Load(file.c_str());
					}
				}
			}
			// 失敗
			if (!isSuccess) {
				CLASS_DELETE(material->texture);
			}
		}

		// マテリアル追加
		materials.push_back(material);
	}

	return true;
}

void Model::SetupTransform(const Transform& _transform) const
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	CbTransformSet& cb = renderer.GetParameter().cbTransformSet;

	// 移動行列
	DirectX::XMMATRIX moveMatrix = DirectX::XMMatrixTranslation(_transform.position.x, _transform.position.y, _transform.position.z);
	// 拡大行列
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(_transform.scale.x, _transform.scale.y, _transform.scale.z);
	// 回転行列
	DirectX::XMMATRIX rotateMatX = DirectX::XMMatrixRotationX(_transform.rotation.x);
	DirectX::XMMATRIX rotateMatY = DirectX::XMMatrixRotationY(_transform.rotation.y);
	DirectX::XMMATRIX rotateMatZ = DirectX::XMMatrixRotationZ(_transform.rotation.z);
	DirectX::XMMATRIX rotateMatrix = rotateMatX * rotateMatY * rotateMatZ;

	DirectX::XMMATRIX mtx = scaleMatrix
		* rotateMatrix
		* moveMatrix;

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

void Model::Draw(const Transform& _transform) const
{
	SetupTransform(_transform);
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// テクスチャをピクセルシェーダーに送る
		ID3D11ShaderResourceView* srv = materials[meshIdx]->texture->GetSRV();
		renderer.GetDeviceContext()->PSSetShaderResources(0, 1, &srv);

		meshes[meshIdx]->Draw(renderer);
	}
}

void Model::Release()
{
	for (auto mesh : meshes)
	{
		CLASS_DELETE(mesh);
	}

	for (auto mt : materials)
	{
		CLASS_DELETE(mt);
	}
}

bool Model::SetSetting(const ModelSettings& _settings)
{
	// モデルをロードする
	bool isSuccess = Load(_settings, *Direct3D11::GetInstance()->GetRenderer());
	if (!isSuccess)
		return false;

	// モデル情報を所持する
	modelData = _settings;
	// モデル導入されたフラグを立てる
	isImported = true;

	return true;
}

