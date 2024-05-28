#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Direct3D11.h"

#include "TextureCollect.h"
#include "MaterialCollect.h"

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

bool Model::SetModel(const Model& _setModel)
{
	// モデル情報をリセットする
	ResetParam();

	// メッシュの数を取得
	meshNum = _setModel.GetMeshNum();

	const std::vector<Mesh*> _setMeshes = _setModel.GetMeshes();

	for (int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// インスタンスを生成する
		Mesh* mesh = new Mesh();

		// データ元モデル空メッシュを取得する
		const Mesh* setMesh = _setMeshes[meshIdx];
		if (setMesh == nullptr)	// 無かったら
		{
			// エラー
			MessageError("メッシュのポインタがnullptrです");
			// パラメータをリセットする
			ResetParam();
			return false;	// 失敗
		}

		// メッシュの情報をセットする
		mesh->SetMesh(*setMesh);
		// 配列にセットする
		meshes.push_back(mesh);
	}

	// モデルの名前などの情報を取得する
	modelData = _setModel.GetModelData();
	return true;
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
	// マテリアルを保管クラスに入れる
	MaterialCollect* mC = MaterialCollect::GetInstance();

	// ファイルの探索
	std::string dir = _settings.modelPath;
	dir = dir.substr(0, dir.find_last_of('/') + 1);

	aiColor3D color(0.0f, 0.0f, 0.0f);
	Float4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Float4 ambient(0.3f, 0.3f, 0.3f, 1.0f);

	// モデルの各マテリアルを作成する
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		// マテリアルのセットする名前は "モデルの名前 順番"
		std::string materialName = _settings.modelName + std::to_string(i);

		// マテリアルが既にロードされているなら
		if (mC->GetIsImported(materialName))
		{
			// 取得して、配列に入れる
			Material* loadMaterial =  mC->GetResource(materialName);
			materials.push_back(loadMaterial);
			continue;	// 次のループへ
		}

		// マテリアルを作成
		std::unique_ptr<Material> material = std::make_unique<Material>();
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
		// 保管クラスのインスタンスを取得
		TextureCollect* texCollect = TextureCollect::GetInstance();
		// テクスチャの名前はマテリアルと同じ
		std::string texName = materialName;
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			// ファイル形式チェック
			if (strstr(path.C_Str(), ".psd"))
			{
				MessageError("psdには対応していません");
			}
			bool isSuccess = false;	// ロード成功したかフラグ

			// テクスチャのインスタンスを作成
			std::unique_ptr<Texture> texture = std::make_unique<Texture>();
			/*material->texture = new Texture();*/
			// モデルと同じ階層を探索
			std::string file = dir;
			file += path.C_Str();

			// テクスチャ管理クラスにモデルのテクスチャを代入する(名前はモデルと一緒)
			isSuccess = texCollect->Load(file.c_str(), texName);

			// ファイル名のみで探索
			if(!isSuccess) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					isSuccess = texCollect->Load(file.c_str(), texName);
				}

				// ファイル目のパスが"\\ではなく"/"の場合の対応
				if (!isSuccess) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						isSuccess = texCollect->Load(file.c_str(), texName);
					}
				}
			}
			// 失敗
			if (!isSuccess) {
				std::string message = "モデルのテクスチャ読込失敗　" + materialName;
				MessageError(message.c_str());
				
				return false;
			}
		}
		
		// 読み込んだテクスチャをマテリアルに設定する
		material->texture = texCollect->GetConstResource(texName);
		// 保管クラスに入れる
		mC->SetResource(std::move(material), materialName);
		// マテリアルを受け取る
		Material* mat = mC->GetResource(materialName);
		// マテリアル追加
		materials.push_back(mat);
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

void Model::ResetParam()
{
	Release();

	meshes.clear();
	meshes.shrink_to_fit();

	materials.clear();
	materials.shrink_to_fit();

	meshNum = 0;

	modelData = {};

	isImported = false;
	isPermanent = false;
}

void Model::Release()
{
	for (auto mesh : meshes)
	{
		CLASS_DELETE(mesh);
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

