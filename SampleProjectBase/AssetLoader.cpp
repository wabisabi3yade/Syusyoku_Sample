#include "pch.h"
#include "AssetLoader.h"

// assimpライブラリ読込
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// テクスチャ
#include "DirectXTex/TextureLoad.h"
#include "Texture.h"

// モデル
#include "Mesh_Base.h"

namespace fs = std::filesystem;

using namespace DirectX::SimpleMath;

/// @brief aiVector3D型をSimpleMathのVector3に変換
/// @param _aiVector3 変換する値
/// @return 変換されたVector3
static Vector3 ToVector3(const aiVector3D& _aiVector3);

/// @brief aiColor4D型をSimpleMathのColorに変換
/// @param _aiColor 変換する値
/// @return 変換されたColor
static Color ToColor(const aiColor4D& _aiColor);

void AssetLoader::MaterialLoad(Mesh_Base* _pMeshgather,
	const aiScene* pScene, std::string texturedirectory)
{
	// マテリアル数文ループ
	for (unsigned int m = 0; m < pScene->mNumMaterials; m++)
	{
		aiMaterial* material = pScene->mMaterials[m];

		// マテリアル名取得
		std::string mtrlname = material->GetName().C_Str();

		// マテリアル情報
		aiColor4D ambient;
		aiColor4D diffuse;
		aiColor4D specular;
		aiColor4D emission;
		float shiness;
		// アンビエント
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
		}
		else
		{
			ambient = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// ディフューズ
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
		}
		else {
			diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// スペキュラ
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
		}
		else {
			specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// エミッション
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission)) {
		}
		else {
			emission = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// シャイネス
		if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shiness)) {
		}
		else {
			shiness = 0.0f;
		}

		// マテリアルのパラメータをセット
		std::unique_ptr<Material> createMaterial = std::make_unique<Material>();
		createMaterial->SetDiffuse(ToColor(diffuse));
		createMaterial->SetAmbeint(ToColor(ambient));
		createMaterial->SetSpecular(ToColor(specular));
		createMaterial->SetEmissive(ToColor(emission));
		createMaterial->SetShiness(shiness);
		createMaterial->SetName(mtrlname);

		aiString path{};
		std::unique_ptr<Texture> texture = std::make_unique<Texture>();

		for (unsigned int t = 0; t < material->GetTextureCount(aiTextureType_DIFFUSE); t++)
		{
			// t番目のテクスチャパス取得
			if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, t, &path))
			{
				// テクスチャパス取得
				std::string texPath = path.C_Str();

				// 内蔵テクスチャかどうかを判断する
				if (auto tex = pScene->GetEmbeddedTexture(path.C_Str()))
				{
					// 内蔵テクスチャの場合
					bool sts = TextureMemoryLoad(
						*texture.get(),
						(unsigned char*)tex->pcData,
						tex->mWidth
					);

					// 読み込めたら
					if (sts)
						break;

				}
				else // 外部テクスチャファイルの場合
				{
					std::string texname = texturedirectory + "/" + PathToFileName(texPath,true);

					Texture* diffuseTex = TextureLoad(texname);
					createMaterial->SetDiffuseTexture(*diffuseTex);
				}
			}
			// ディフューズテクスチャがなかった場合
			else
			{
				ImGuiDebugLog::Add("マテリアル名：" + mtrlname +
					std::to_string(t) + "番目のテクスチャはありません");
			}
		}

		// 管理クラスにセットする
		Material* materialPtr =
			SendAsset<Material>(mtrlname, std::move(createMaterial));

		// メッシュにマテリアル追加
		_pMeshgather->AddMaterial(*materialPtr);
	}
}

bool AssetLoader::TextureMemoryLoad(Texture& _texture, const unsigned char* Data, int len)
{
	return false;
}

std::unique_ptr<Texture> AssetLoader::MakeTexture(const std::string& _filePath)
{
	if (_filePath == "")
	{
		ImGuiDebugLog::Add("テクスチャのパス名が入力されていません");
		return nullptr;
	}

	// テクスチャをロードする
	std::unique_ptr<Texture> pMakeTex = std::make_unique<NullTexture>();

	// マルチバイト文字列をワイド文字列に変換
	wchar_t widePath[MAX_PATH];
	size_t wideLen = 0;
	MultiByteToWideChar(0, 0, _filePath.c_str(), -1, widePath, MAX_PATH);

	// ファイル読込
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT hr;

	// 読み込まれたファイルがtga形式なら
	if (strstr(_filePath.c_str(), ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(widePath, &metaData, image);
	}
	// 読み込まれたファイルがpsd形式なら
	else if (strstr(_filePath.c_str(), ".psd"))
	{
		std::string message = "psdファイルは対応していません\n" + std::string(_filePath);
		ImGuiDebugLog::Add(message);

		return nullptr;
	}
	else
	{
		// Windowsに対応した画像形式(bmp, png, gif, tiff, jpegなど)を読み込む
		hr = DirectX::LoadFromWICFile(widePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metaData, image);
	}

	if (FAILED(hr))
	{
		std::string message = "テクスチャ読込失敗：" + std::string(_filePath);
		ImGuiDebugLog::Add(message);
		return nullptr;
	}

	// SRVを作成
	ID3D11Device* device = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &pMakeTex->pSRV);

	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("SRV作成でエラー");
		return nullptr;
	}

	pMakeTex->width = static_cast<u_int>(metaData.width);
	pMakeTex->height = static_cast<u_int>(metaData.height);

	// パスから名前取得する
	std::string texName = PathToFileName(_filePath, true);
	pMakeTex->SetName(texName);

	return std::move(pMakeTex);
}

Texture* AssetLoader::TextureLoad(const std::string& _filePath)
{
	// テクスチャを作成
	std::unique_ptr<Texture> pTexture = MakeTexture(_filePath);

	if (pTexture == nullptr)
		return nullptr;

	std::string texName = pTexture->GetName();

	// アセット管理クラスにセット
	Texture* returnPtr = SendAsset<Texture>(texName, std::move(pTexture));

	return returnPtr;
}

Mesh_Base* AssetLoader::ModelLoad(const std::string& _modelPath, float _scale, bool _isLeftHand, bool _isGetScale)
{
	std::unique_ptr<Mesh_Base> pMeshGather = std::make_unique<Mesh_Base>();

	// シーン情報構築
	Assimp::Importer importer;

	int flag = aiProcess_Triangulate;

	if (_isLeftHand)
		flag |= aiProcess_ConvertToLeftHanded;


	// シーン情報を構築
	const aiScene* pScene = importer.ReadFile(
		_modelPath.c_str(),
		flag);			// 三角形化する

	if (pScene == nullptr)
	{
		ImGuiDebugLog::Add("読込失敗：" + _modelPath);
	}
	assert(pScene != nullptr);

	// 親パス名
	std::string parentPath = GetParentPath(_modelPath);

	// マテリアル情報取得
	MaterialLoad(pMeshGather.get(), pScene, parentPath);

	// メッシュの最大・最小座標
	Vector3 modelMaxPos;
	Vector3 modelMinPos;

	for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
	{
		std::unique_ptr<SingleMesh> pCreateMesh = std::make_unique<SingleMesh>();

		aiMesh* mesh = pScene->mMeshes[m];

		// メッシュ名取得
		std::string meshname = std::string(mesh->mName.C_Str());
		pCreateMesh->SetName(meshname);

		// マテリアルインデックス取得
		u_int materialIdx = mesh->mMaterialIndex;
		pCreateMesh->SetMaterialID(materialIdx);

		// 頂点の最大・最小座標
		Vector3 maxVPos, minVPos;
		
		std::vector<Vertex>& verticies = pCreateMesh->GetVerticies();
		//　頂点数分ループ
		for (unsigned int vidx = 0; vidx < mesh->mNumVertices; vidx++)
		{
			// 頂点データ
			Vertex vertex;

			// 座標(スケール値を反映する)
			vertex.position = ToVector3(mesh->mVertices[vidx]) * _scale;

			// 最大・最小を更新
			if (_isGetScale)
				UpdateSize(vertex.position, maxVPos, minVPos);


			// 法線あり？
			if (mesh->HasNormals()) {
				vertex.normal = ToVector3(mesh->mNormals[vidx]);
			}
			else
			{
				vertex.normal = Vector3::Zero;
			}

			// 頂点カラー？（０番目）
			if (mesh->HasVertexColors(0)) {
				vertex.color = ToColor(mesh->mColors[0][vidx]);
			}
			else
			{
				vertex.color = Vector4::One;
			}

			// テクスチャあり？（０番目）
			if (mesh->HasTextureCoords(0)) {
				vertex.uv.x = mesh->mTextureCoords[0][vidx].x;
				vertex.uv.y = mesh->mTextureCoords[0][vidx].y;
			}
			else
			{
				vertex.uv = Vector2::Zero;
			}

			// 頂点データを追加
			verticies.push_back(vertex);
		}

		// インデックスを取得する
		std::vector<u_int>& indicies = pCreateMesh->GetIndicies();

		// インデックス数分ループ
		for (unsigned int fidx = 0; fidx < mesh->mNumFaces; fidx++)
		{
			aiFace face = mesh->mFaces[fidx];

			assert(face.mNumIndices == 3);	// 三角形のみ対応

			// インデックスデータを追加
			for (unsigned int i = 0; i < face.mNumIndices; i++)
				indicies.push_back(face.mIndices[i]);
		}

		// バッファ生成
		pCreateMesh->InitBuffer();

		// メッシュのサイズをセット
		pCreateMesh->SetSize(maxVPos - minVPos);
		
		// モデルの最大・最小を更新
		if (_isGetScale)
		{
			UpdateSize(maxVPos, modelMaxPos, modelMinPos);
			UpdateSize(minVPos, modelMaxPos, modelMinPos);
		}
		
		pMeshGather->AddMesh(std::move(pCreateMesh));
	}
	// 名前設定
	std::string modelName = PathToFileName(_modelPath, false);
	pMeshGather->SetName(modelName);

	pMeshGather->SetSize(modelMaxPos - modelMinPos);

	Mesh_Base* pReturnMeshGather = SendAsset<Mesh_Base>(modelName, std::move(pMeshGather));

	return pReturnMeshGather;
}

std::string AssetLoader::PathToFileName(const std::string& _pathName, bool _isExtension)
{
	fs::path fsPath = _pathName;

	if (_isExtension)
		return fsPath.filename().string();
	else
		return fsPath.stem().string();
}

std::string AssetLoader::GetParentPath(const std::string& _pathName)
{
	fs::path fsPath = _pathName;
	return fsPath.parent_path().string();
}

void AssetLoader::UpdateSize(const DirectX::SimpleMath::Vector3& _vertexPos, DirectX::SimpleMath::Vector3& _max, DirectX::SimpleMath::Vector3& _min)
{
	if (_vertexPos.x > _max.x)
		_max.x = _vertexPos.x;

	if (_vertexPos.y > _max.y)
		_max.y = _vertexPos.y;

	if (_vertexPos.z > _max.z)
		_max.z = _vertexPos.z;

	if (_vertexPos.x < _min.x)
		_min.x = _vertexPos.x;

	if (_vertexPos.y < _min.y)
		_min.y = _vertexPos.y;

	if (_vertexPos.z < _min.z)
		_min.z = _vertexPos.z;
}

DirectX::SimpleMath::Vector3 ToVector3(const aiVector3D& _aiVector3)
{
	Vector3 vector3;
	vector3.x = _aiVector3.x; vector3.y = _aiVector3.y; vector3.z = _aiVector3.z;

	return vector3;
}

DirectX::SimpleMath::Color ToColor(const aiColor4D& _aiColor)
{
	Color color;
	color.x = _aiColor.r; color.y = _aiColor.g; color.z = _aiColor.b; color.w = _aiColor.a;

	return color;
}
