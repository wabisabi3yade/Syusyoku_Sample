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
// stb_imageライブラリ
#define STB_IMAGE_IMPLEMENTATION
#include	"stb_image.h"
#include "Texture.h"

// メッシュ群
#include "Mesh_Group.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"

// アニメーション
#include "AnimationData.h"

namespace fs = std::filesystem;

using namespace DirectX::SimpleMath;

Assimp::Importer g_importer;

/// @brief aiVector3D型をSimpleMathのVector3に変換
/// @param _aiVector3 変換する値
/// @return 変換されたVector3
static Vector3 ToVector3(const aiVector3D& _aiVector3);

/// @brief aiColor4D型をSimpleMathのColorに変換
/// @param _aiColor 変換する値
/// @return 変換されたColor
static Color ToColor(const aiColor4D& _aiColor);

/// @brief aiMatrixをDirectXの行列に変換
/// @param _aiMatrix assimp行列
/// @return DirectX行列
static Matrix ToDirectXMatrix(const aiMatrix4x4& _aiMatrix);

void AssetLoader::MaterialLoad(Mesh_Group* _pMeshgather,
	const aiScene* pScene, std::string texturedirectory)
{
	// マテリアル数文ループ
	for (unsigned int m = 0; m < pScene->mNumMaterials; m++)
	{
		aiMaterial* material = pScene->mMaterials[m];

		// マテリアル名取得
		std::string mtrlname = material->GetName().C_Str();
		if (mtrlname.length() <= 0)
		{
			mtrlname = "M_" + std::string(pScene->mMeshes[m]->mName.C_Str());
		}

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
				if (auto aiTex = pScene->GetEmbeddedTexture(path.C_Str()))
				{
					// 内蔵テクスチャの場合
					bool sts = LoadEmbeddedTexture(
						*texture.get(),
						*aiTex
					);

					// 読み込めたら
					if (sts)
						break;

				}
				else // 外部テクスチャファイルの場合
				{
					std::string texname = texturedirectory + "/" + PathToFileName(texPath, true);

					texture = MakeTexture(texname);
				}
			}
			// ディフューズテクスチャがなかった場合
			else
			{
				HASHI_DEBUG_LOG("マテリアル名：" + mtrlname +
					std::to_string(t) + "番目のテクスチャはありません");
			}
		}

		// テクスチャを管理にセット
		std::string texName = texture->GetName();
		Texture* pTex = SendAsset(texName, std::move(texture));

		createMaterial->SetDiffuseTexture(*pTex);

		// 管理クラスにセットする
		Material* materialPtr =
			SendAsset<Material>(mtrlname, std::move(createMaterial));

		// メッシュにマテリアル追加
		_pMeshgather->AddMaterial(materialPtr);
	}
}

bool AssetLoader::LoadEmbeddedTexture(Texture& _texture, const aiTexture& _aiTex)
{
	bool sts = true;
	unsigned char* pixels;

	const unsigned char* Data = (unsigned char*)_aiTex.pcData;
	int len = _aiTex.mWidth;

	int texWidth = 0;
	int texHeight = 0;

	// 画像読み込み
	pixels = stbi_load_from_memory(Data,
		len,
		&texWidth,
		&texHeight,
		nullptr,
		STBI_rgb_alpha);

	// テクスチャ2Dリソース生成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = texWidth;
	desc.Height = texHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * 4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, _texture.pSRV.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// ピクセルイメージ解放
	stbi_image_free(pixels);

	// 名前設定
	std::string texName = PathToFileName(_aiTex.mFilename.C_Str(), true);
	_texture.SetName(texName);

	// 幅設定
	_texture.width = texWidth;
	_texture.height = texHeight;

	return true;
}

std::unique_ptr<Texture> AssetLoader::MakeTexture(const std::string& _filePath)
{
	std::unique_ptr<Texture> pMakeTex = std::make_unique<NullTexture>();

	bool sts = true;
	unsigned char* pixels;

	int width = 0;
	int height = 0;

	// 画像読み込み
	pixels = stbi_load(_filePath.c_str(), &width, &height, nullptr, 4);
	if (pixels == nullptr)
	{

		HASHI_DEBUG_LOG("画像読込失敗");
		return std::move(pMakeTex);
	}

	// テクスチャ2Dリソース生成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * 4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = Direct3D11::GetInstance()->GetRenderer()->GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		HASHI_DEBUG_LOG("テクスチャ作成失敗");

		stbi_image_free(pixels);
		return std::move(pMakeTex);
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, pMakeTex->pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		HASHI_DEBUG_LOG("SRV作成失敗");
		stbi_image_free(pixels);
		return std::move(pMakeTex);
	}

	// ピクセルイメージ解放
	stbi_image_free(pixels);

	pMakeTex->width = static_cast<u_int>(width);
	pMakeTex->height = static_cast<u_int>(height);

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

Mesh_Group* AssetLoader::ModelLoad(const std::string& _modelPath, float _scale, bool _isLeftHand, bool _isGetScale)
{
	// シーン情報構築
	Assimp::Importer importer;

	int flag = aiProcess_Triangulate;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_FlipUVs;
	flag |= aiProcess_CalcTangentSpace;
	if (_isLeftHand)
		flag |= aiProcess_MakeLeftHanded;


	// シーン情報を構築
	const aiScene* pScene = importer.ReadFile(
		_modelPath.c_str(),
		flag);			// 三角形化する

	if (pScene == nullptr)
	{
		HASHI_DEBUG_LOG("読込失敗：" + _modelPath);
	}

	assert(pScene != nullptr);

	// メッシュのグループを作成する
	// ここでボーン情報も読み込む
	std::unique_ptr<Mesh_Group> pMeshGroup = CreateMeshGroup(pScene);

	// 親パス名
	std::string parentPath = GetParentPath(_modelPath);

	// マテリアル情報取得
	MaterialLoad(pMeshGroup.get(), pScene, parentPath);

	// メッシュの最大・最小座標
	Vector3 modelMaxPos = Vector3::One * -10.0f;
	Vector3 modelMinPos = Vector3::One * 10.0f;

	// ボーンのインデックス(ルートノードの次から作成する)
	u_int boneIdx = 0;

	// メッシュ分ループ
	for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
	{
		std::unique_ptr<SingleMesh> pCreateMesh = std::make_unique<SingleMesh>();

		aiMesh* pAimesh = pScene->mMeshes[m];
	
		// メッシュ名取得
		std::string meshname = std::string(pAimesh->mName.C_Str());
		pCreateMesh->SetName(meshname);

		// マテリアルインデックス取得
		u_int materialIdx = pAimesh->mMaterialIndex;
		pCreateMesh->SetMaterialID(materialIdx);

		// 頂点の最大・最小座標
		Vector3 maxVPos = Vector3::One * -10.0f, minVPos = Vector3::One * 10.0f;

		std::vector<Vertex>& verticies = pCreateMesh->GetVerticies();
		//　頂点数分ループ
		for (unsigned int vidx = 0; vidx < pAimesh->mNumVertices; vidx++)
		{
			// 頂点データ
			Vertex vertex;

			// 座標(スケール値を反映する)
			vertex.position = ToVector3(pAimesh->mVertices[vidx]) * _scale;

			// 最大・最小を更新
			if (_isGetScale)
				UpdateSize(vertex.position, maxVPos, minVPos);


			// 法線あり？
			if (pAimesh->HasNormals())
				vertex.normal = ToVector3(pAimesh->mNormals[vidx]);
			else
				vertex.normal = Vector3::Zero;

			// 頂点カラー？（０番目）
			if (pAimesh->HasVertexColors(0))
				vertex.color = ToColor(pAimesh->mColors[0][vidx]);
			else
				vertex.color = Vector4::One;

			// テクスチャあり？（０番目）
			if (pAimesh->HasTextureCoords(0))
			{
				vertex.uv.x = pAimesh->mTextureCoords[0][vidx].x;
				vertex.uv.y = pAimesh->mTextureCoords[0][vidx].y;
			}
			else
				vertex.uv = Vector2::Zero;

			// 頂点データを追加
			verticies.push_back(vertex);
		}

		// インデックスを取得する
		std::vector<u_int>& indicies = pCreateMesh->GetIndicies();

		// インデックス数分ループ
		for (unsigned int fidx = 0; fidx < pAimesh->mNumFaces; fidx++)
		{
			aiFace face = pAimesh->mFaces[fidx];

			assert(face.mNumIndices == 3);	// 三角形のみ対応

			// インデックスデータを追加
			for (unsigned int i = 0; i < face.mNumIndices; i++)
				indicies.push_back(face.mIndices[i]);
		}

		// モデルの最大・最小を更新
		if (_isGetScale)
		{
			// メッシュの中心座標・サイズをセット
			pCreateMesh->SetCenterPosition((maxVPos + minVPos) * 0.5f);
			pCreateMesh->SetSize(maxVPos - minVPos);

			UpdateSize(maxVPos, modelMaxPos, modelMinPos);
			UpdateSize(minVPos, modelMaxPos, modelMinPos);
		}

		// スケルタルメッシュなら
		if (pMeshGroup->GetType() == Mesh_Group::MeshType::SK)
		{
			SkeletalMesh& pSK = static_cast<SkeletalMesh&>(*pMeshGroup.get());

			// ボーン情報を取得する
			BonePerMesh bonesPerMesh = CreateBone(pAimesh, *pCreateMesh.get(), boneIdx);

			pSK.AddBonePerMesh(std::move(bonesPerMesh));
		}

		// バッファ生成
		pCreateMesh->InitBuffer();

		pMeshGroup->AddMesh(std::move(pCreateMesh));
	}
	// 名前設定
	std::string modelName = PathToFileName(_modelPath, false);
	pMeshGroup->SetName(modelName);

	// モデルの中心座標・サイズをセット
	if (_isGetScale)
	{
		pMeshGroup->SetCenterPosition((modelMaxPos + modelMinPos) * 0.5f);
		pMeshGroup->SetSize(modelMaxPos - modelMinPos);
	}

	Mesh_Group* pRetMeshGroup = SendAsset<Mesh_Group>(modelName, std::move(pMeshGroup));

	return pRetMeshGroup;
}

AnimationData* AssetLoader::AnimationLoad(const std::string& _animPath, bool _isLeftHand)
{
	int flag = 0;
	
	// 左手系に変換
	if (_isLeftHand)
		flag |= aiProcess_ConvertToLeftHanded;	

	auto pAiScene = g_importer.ReadFile(_animPath.c_str(), flag);
	if (pAiScene == nullptr)
	{
		HASHI_DEBUG_LOG(_animPath + "：アニメーションが正常に読み込めませんでした");
		return nullptr;
	}

	std::unique_ptr<AnimationData> pAnimData = std::make_unique<AnimationData>();
	pAnimData->SetAiScene(pAiScene);

	// 名前をパス名から取得
	std::string assetName = PathToFileName(_animPath, false);
	pAnimData->SetName(assetName);

	// アセット管理にセット
	AnimationData* pRetAnim = SendAsset<AnimationData>(assetName, std::move(pAnimData));

	return pRetAnim;
}

std::unique_ptr<Mesh_Group> AssetLoader::CreateMeshGroup(const aiScene* _pScene)
{
	// ボーンがなかったら
	if (_pScene->mMeshes[0]->mNumBones == 0)
	{
		// スタティックメッシュを返す
		std::unique_ptr<StaticMesh> pSM = std::make_unique<StaticMesh>();
		return std::move(pSM);
	}

	std::unique_ptr<SkeletalMesh>pSkeletalMesh = std::make_unique<SkeletalMesh>();

	//// ルートノードのボーンだけ作成する
	//const aiNode* pAiRoot = _pScene->mRootNode;
	//std::unique_ptr<Bone> pRootBone = std::make_unique<Bone>();
	//
	//pRootBone->SetBoneName(pAiRoot->mName.C_Str());
	//pRootBone->SetIndex(0);

	//BonePerMesh boneGroup;
	//boneGroup.push_back(std::move(pRootBone));

	//pSkeletalMesh->AddBonePerMesh(std::move(boneGroup));
	
	return std::move(pSkeletalMesh);
}

std::vector<std::unique_ptr<Bone>> AssetLoader::CreateBone(const aiMesh* _pAiMesh, SingleMesh& _singleMesh, u_int& _boneIdx)
{
	BonePerMesh retBones;

	// 頂点にボーン情報を入れるときに何番目の配列に入れるかカウントする
	u_int vertexNum = _singleMesh.GetVertexNum();
	std::vector<u_int> vertexBoneCnt(vertexNum);

	// ボーン数ループ
	for (u_int bi = 0; bi < _pAiMesh->mNumBones; bi++)
	{
		const aiBone* pAiBone = _pAiMesh->mBones[bi];
		std::unique_ptr<Bone> pBone = std::make_unique<Bone>();

		// パラメータを取得
		pBone->SetBoneName(std::string(pAiBone->mName.C_Str()));

		// デバッグ用
		HASHI_DEBUG_LOG("ボーン：" + pBone->GetBoneName());

		// オフセット行列
		pBone->SetOffeetMtx(ToDirectXMatrix(pAiBone->mOffsetMatrix));

		// インデックスを取得
		pBone->SetIndex(_boneIdx);

		// ウェイト情報を取得する		
		// 頂点
		std::vector<Vertex>& verticies = _singleMesh.GetVerticies();

		// ウエイト数ループ
		for (u_int wi = 0; wi < pAiBone->mNumWeights; wi++)
		{
			Weight weight;
			weight.boneName = pBone->GetBoneName();

			weight.weight = pAiBone->mWeights[wi].mWeight;
			weight.vertexIndex = pAiBone->mWeights[wi].mVertexId;

			// 頂点とボーン情報をリンクさせる
			Vertex& v = verticies[weight.vertexIndex];
			u_int& idx = vertexBoneCnt[weight.vertexIndex];

			assert(idx < MAX_WEIGHT_NUM);
			v.boneWeight[idx] = weight.weight;
			v.boneIndex[idx] = pBone->GetIndex();

			// カウントを増やす
			idx++;

			// ウェイトを追加する
			pBone->AddWeight(weight);
		}

		// 1メッシュのボーン配列に追加
		retBones.push_back(std::move(pBone));

		_boneIdx++;	// 加算
	}

	return retBones;
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

Matrix ToDirectXMatrix(const aiMatrix4x4& _aiMatrix)
{
	Matrix dxMatrix = Matrix(
		_aiMatrix.a1, _aiMatrix.b1, _aiMatrix.c1, _aiMatrix.d1,
		_aiMatrix.a2, _aiMatrix.b2, _aiMatrix.c2, _aiMatrix.d2,
		_aiMatrix.a3, _aiMatrix.b3, _aiMatrix.c3, _aiMatrix.d3,
		_aiMatrix.a4, _aiMatrix.b4, _aiMatrix.c4, _aiMatrix.d4
	);

	return dxMatrix;
}
