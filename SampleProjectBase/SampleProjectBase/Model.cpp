#include "Model.h"
#include "Mesh.h"
#include "Material.h"

// assimpライブラリ読込
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace DirectX::SimpleMath;

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

	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// インスタンスを生成する
		Mesh* mesh = new Mesh();

		// データ元モデル空メッシュを取得する
		const Mesh* setMesh = _setMeshes[meshIdx];
		if (setMesh == nullptr)	// 無かったら
		{
			ImGuiDebugLog::Add("メッシュのポインタがnullptrです");
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

bool Model::LoadProcess(const ModelSettings& _settings, D3D11_Renderer& _renderer)
{
	// 既にこのモデルが入っていたら
	if (isImported)
		return true;

	if (_settings.modelPath == nullptr)
	{
		ImGuiDebugLog::Add("モデルのファイルパスが設定されていません");
		return false;
	}
	if (_settings.modelName == "")
	{
		ImGuiDebugLog::Add("モデルの名前が設定されていません");
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
				ImGuiDebugLog::Add("メッシュのセットアップ失敗");
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
	Vector4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 ambient(0.3f, 0.3f, 0.3f, 1.0f);

	// リソース管理クラス取得
	ResourceCollection* resourceCollection = ResourceCollection::GetInstance();

	// モデルの各マテリアルを作成する
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		// マテリアルのセットする名前は "M_名前 + 数字"
		std::string materialName = "M_" + _settings.modelName + std::to_string(i);
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
		// テクスチャのインスタンスを作成
		std::unique_ptr<Texture> texture = std::make_unique<Texture>();
		// テクスチャの名前頭文字をT_にする
		std::string texName = "T_" + _settings.modelName + std::to_string(i);
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			// ファイル形式チェック
			if (strstr(path.C_Str(), ".psd"))
			{
				ImGuiDebugLog::Add("psdには対応していません");
			}
			bool isSuccess = false;	// ロード成功したかフラグ
			// モデルと同じ階層を探索
			std::string file = dir;
			file += path.C_Str();
			// テクスチャ管理クラスにモデルのテクスチャを代入する(名前はモデルと一緒)
			isSuccess = texture->Load(file.c_str());

			// ファイル名のみで探索
			if (!isSuccess) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					isSuccess = texture->Load(file.c_str());
				}

				// ファイル目のパスが"\\ではなく"/"の場合の対応
				if (!isSuccess) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						isSuccess = texture->Load(file.c_str());
					}
				}
			}
			// 失敗
			if (!isSuccess) {
				std::string message = "モデルのテクスチャ読込失敗　" + texName;
				ImGuiDebugLog::Add(message);
			}
			else
			{
				// テクスチャ読込み成功
				// 読み込んだテクスチャをマテリアルに設定する
				pTextures.push_back(texture.get());
				// リソース管理にテクスチャ追加
				resourceCollection->SetResource<Texture>(texName, std::move(texture));
			}

			// マテリアル追加
			pMaterials.push_back(material.get());
			// マテリアルを保管クラスに入れる
			resourceCollection->SetResource<Material>(materialName, std::move(material));
		}
	}

	return true;
}

void Model::Draw(const Transform& _transform) const
{
	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ワールド変換行列を取得
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(_transform);

	// ワールド変換行列の座標にモデルの座標を入れる
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// バッファを更新する
		pMaterials[meshIdx]->pVertexShader->UpdateBuffer(0, &wvp);
		// テクスチャ設定
		if(pTextures.size() > meshIdx)	// テクスチャなくても読み込める用にしたいので
		pMaterials[meshIdx]->pPixelShader->SetTexture(0, pTextures[meshIdx]);
		// シェーダーをバインド
		pMaterials[meshIdx]->pVertexShader->Bind();
		pMaterials[meshIdx]->pPixelShader->Bind();

		meshes[meshIdx]->Draw(renderer);
	}
}

void Model::ResetParam()
{
	Release();

	meshes.clear();
	meshes.shrink_to_fit();

	pMaterials.clear();
	pMaterials.shrink_to_fit();

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

bool Model::Load(const ModelSettings& _settings)
{
	// モデルをロードする
	bool isSuccess = LoadProcess(_settings, *Direct3D11::GetInstance()->GetRenderer());
	if (!isSuccess)
		return false;

	// モデル情報を所持する
	modelData = _settings;
	// モデル導入されたフラグを立てる
	isImported = true;

	return true;
}

void Model::SetVertexShader(Shader* _vertexSh)
{
	for (auto material : pMaterials)
	{
		material->pVertexShader = dynamic_cast<VertexShader*>(_vertexSh);
	}
}

void Model::SetPixelShader(Shader* _pixelSh)
{
	for (auto material : pMaterials)
	{
		material->pPixelShader = dynamic_cast<PixelShader*>(_pixelSh);
	}
}

