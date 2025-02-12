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
#include "stb_image.h"
#include "Texture.h"

// メッシュ群
#include "Mesh_Group.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"

// アニメーション
#include "AnimationData.h"

// エフェクト
#include "VisualEffect.h"

// サウンド
#include "DXSoundManager.h"

namespace HashiTaku
{
	namespace fs = std::filesystem;
	using namespace DXSimp;

	Matrix AssetLoader::loadNodeTransformMtx = Matrix::Identity;
	Matrix AssetLoader::loadNodeScaleMtx = Matrix::Identity;
	Matrix AssetLoader::loadNodeRotationMtx = Matrix::Identity;

	/// @brief aiVector3D型をSimpleMathのVector3に変換
	/// @param _aiVector3 変換する値
	/// @return 変換されたVector3
	static Vector3 ToVector3(const aiVector3D& _aiVector3);

	/// @brief aiQuaternion型をSimpleMathのQuaternionに変換
	/// @param _aiQuaternion 変換する値
	/// @return 変換されたQuaternion
	static Quaternion ToQuaternion(const aiQuaternion& _aiQuat);

	/// @brief aiColor4D型をSimpleMathのColorに変換
	/// @param _aiColor 変換する値
	/// @return 変換されたColor
	static Color ToColor(const aiColor4D& _aiColor);

	/// @brief aiMatrixをDirectXの行列に変換
	/// @param _aiMatrix assimp行列
	/// @return DirectX行列
	static Matrix ToDirectXMatrix(const aiMatrix4x4& _aiMatrix);

	/// @brief DirectXをaiMatrixの行列に変換
	/// @param _dxMatrix DirectX行列
	/// @return aiMatrix
	static aiMatrix4x4 ToAssimpMatrix(const Matrix& _dxMatrix);

	void AssetLoader::MaterialLoad(Mesh_Group* _pMeshgather,
		const aiScene* _pScene, const std::string& _texturedirectory)
	{
		// マテリアル数文ループ
		for (unsigned int m = 0; m < _pScene->mNumMaterials; m++)
		{
			aiMaterial* material = _pScene->mMaterials[m];

			// マテリアル名取得
			std::string mtrlname = material->GetName().C_Str();
			if (mtrlname.length() <= 0)
			{
				mtrlname = "M_" + std::string(_pScene->mMeshes[m]->mName.C_Str());
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
			createMaterial->SetAssetName(mtrlname);
			createMaterial->SetIsntSave();

			aiString path{};
			std::unique_ptr<Texture> texture = std::make_unique<Texture>();
			texture->SetIsntSave();

			for (unsigned int t = 0; t < material->GetTextureCount(aiTextureType_DIFFUSE); t++)
			{
				// t番目のテクスチャパス取得
				if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, t, &path))
				{
					// テクスチャパス取得
					std::string texPath = path.C_Str();

					// 内蔵テクスチャかどうかを判断する
					if (auto aiTex = _pScene->GetEmbeddedTexture(path.C_Str()))
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
						std::string texname = _texturedirectory + "/" + PathToFileName(texPath);

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

			// 同じ階層の同じ名前の画像を取得する
			if (!texture->GetIsSetSRV())
			{
				// 拡張しなしのパス名
				std::string meshNameNotExt = _texturedirectory + "/" + GetPathNameNotExt(_pMeshgather->GetAssetName());

				// png形式で見る
				texture = MakeTexture(meshNameNotExt + ".png");

				// jpg形式
				if (!texture->GetIsSetSRV())
					texture = MakeTexture(meshNameNotExt + ".jpg");
			}

			// テクスチャを管理にセット
			if (texture->GetIsSetSRV())
			{
				std::string texName = texture->GetAssetName();
				Texture* pTex = SendAsset(texName, std::move(texture));
				createMaterial->SetDiffuseTexture(*pTex);
			}

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
		std::string texName = PathToFileName(_aiTex.mFilename.C_Str());
		_texture.SetAssetName(texName);

		// 幅設定
		_texture.width = texWidth;
		_texture.height = texHeight;

		return true;
	}

	std::unique_ptr<Texture> AssetLoader::MakeTexture(const std::string& _filePath)
	{
		std::unique_ptr<Texture> pMakeTex = std::make_unique<Texture>();

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
		std::string texName = PathToFileName(_filePath);
		pMakeTex->SetAssetName(texName);

		return std::move(pMakeTex);
	}

	Texture* AssetLoader::TextureLoad(const std::string& _filePath)
	{
		// テクスチャを作成
		std::unique_ptr<Texture> pTexture = MakeTexture(_filePath);

		if (pTexture == nullptr)
			return nullptr;

		pTexture->SetPathName(_filePath);
		std::string texName = pTexture->GetAssetName();

		// アセット管理クラスにセット
		Texture* returnPtr = SendAsset<Texture>(texName, std::move(pTexture));

		return returnPtr;
	}

	Mesh_Group* AssetLoader::ModelLoad(const std::string& _modelPath, float _scale, bool _isFlipY, bool _isRightHand, bool _isGetScale)
	{
		// シーン情報構築
		Assimp::Importer importer;

		int flag = 0;

		flag |= aiProcessPreset_TargetRealtime_MaxQuality;	// リアルタイム レンダリング用にデータを最適化するデフォルトの後処理構成。
		flag |= aiProcess_PopulateArmatureData;				// 標準的なボーン,アーマチュアの設定
		flag &= ~aiProcess_FindDegenerates;	// こいつのせいでロードできないものがあったのでフラグを削除する

		if (_isRightHand)
			flag |= aiProcess_ConvertToLeftHanded;	// 左手系変更オプションがまとまったもの


		// シーン情報を構築
		const aiScene* pScene = importer.ReadFile(
			_modelPath.c_str(),
			flag);			// 三角形化する

		if (pScene == nullptr)
		{
			HASHI_DEBUG_LOG("読込失敗：" + _modelPath);
			return nullptr;
		}

		// 名前設定
		std::string modelName = PathToFileName(_modelPath);

		Vector3 loadAngles;
		if (_isFlipY)
			loadAngles.y = 180.0f;

		// メッシュのグループを作成する
		// ここでボーン情報も読み込む
		std::unique_ptr<Mesh_Group> pMeshGroup = CreateMeshGroup(pScene, modelName, _scale, loadAngles);

		pMeshGroup->SetLoadOffsetScale(_scale);
		pMeshGroup->SetLoadFlipY(_isFlipY);
		pMeshGroup->SetIsRightHand(_isRightHand);
		pMeshGroup->SetIsGetSize(_isGetScale);
		pMeshGroup->SetPathName(_modelPath);

		// 親パス名
		std::string parentPath = GetParentPath(_modelPath);

		// マテリアル情報取得
		MaterialLoad(pMeshGroup.get(), pScene, parentPath);

		// メッシュの最大・最小座標
		Vector3 modelMaxPos = Vector3::One * -10.0f;
		Vector3 modelMinPos = Vector3::One * 10.0f;

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
				vertex.position = ToVector3(pAimesh->mVertices[vidx]);

				// 最大・最小を更新
				if (_isGetScale)
					UpdateSize(vertex.position, maxVPos, minVPos);


				// 法線あり？
				if (pAimesh->HasNormals())
				{
					vertex.normal = ToVector3(pAimesh->mNormals[vidx]);
				}
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

				// ボーン情報を頂点に紐づける
				LinkVertexToBone(pAimesh, *pCreateMesh.get(), pSK);
			}

			// バッファ生成
			pCreateMesh->InitBuffer();

			pMeshGroup->AddMesh(std::move(pCreateMesh));
		}

		// モデルの中心座標・サイズをセット
		if (_isGetScale)
		{
			pMeshGroup->SetCenterPosition((modelMaxPos + modelMinPos) * 0.5f);
			pMeshGroup->SetSize((modelMaxPos - modelMinPos) * _scale);
		}

		Mesh_Group* pRetMeshGroup = SendAsset<Mesh_Group>(modelName, std::move(pMeshGroup));

		return pRetMeshGroup;
	}

	AnimationData* AssetLoader::AnimationLoad(const std::string& _animPath, const std::string& _boneName, bool _isRightHand)
	{
		Assimp::Importer importer;

		int flag = 0;

		// 左手系に変換
		if (_isRightHand)
			flag |= aiProcess_ConvertToLeftHanded;

		auto pAiScene = importer.ReadFile(_animPath.c_str(), flag);
		if (pAiScene == nullptr)
		{
			HASHI_DEBUG_LOG(_animPath + "：アニメーションが正常に読み込めませんでした");
			return nullptr;
		}

		std::unique_ptr<AnimationData> pAnimData = std::make_unique<AnimationData>();

		// ロード時のパラメータをセット
		pAnimData->SetPathName(_animPath);
		pAnimData->SetIsRightHand(_isRightHand);

		const aiAnimation* aiAnimation = pAiScene->mAnimations[0];

		// アニメーション対応させるボーンリストを取得する
		const BoneList* pAssetBoneList = AssetGetter::GetAsset<BoneList>(_boneName);
		if (pAssetBoneList == nullptr)
		{
			HASHI_DEBUG_LOG(_boneName + "：ボーンがアセットにありませんでした");
			return nullptr;
		}
		pAnimData->SetBoneListName(_boneName);

		// ノードを作成する
		for (u_int an_i = 0; an_i < aiAnimation->mNumChannels; an_i++)
		{
			std::unique_ptr<AnimationChannel> pAnimChannel = CreateAnimChannel(aiAnimation->mChannels[an_i], *pAssetBoneList);

			// アニメーションの追加していく
			pAnimData->AddAnimationChannel(std::move(pAnimChannel));
		}

		// アニメーションのデータを自作クラスにセット
		pAnimData->SetAnimationTime(GetAnimationTime(aiAnimation));
		pAnimData->SetTimePerKey(GetTimePerKey(aiAnimation));
		pAnimData->SetAllFrameCnt(GetAnimationFrameCnt(aiAnimation));
		pAnimData->CalcRootMotion(pAssetBoneList->GetRootBoneId());

		// 名前をパス名から取得
		std::string assetName = PathToFileName(_animPath);

		// アセット管理にセット
		AnimationData* pRetAnim = SendAsset<AnimationData>(assetName, std::move(pAnimData));

		return pRetAnim;
	}

	VisualEffect* AssetLoader::VFXLoadForEffekseer(const std::string& _pathName, float _loadScale)
	{
		DX11EffecseerManager* pEffekseerManager = DX11EffecseerManager::GetInstance();
		const auto& manager = pEffekseerManager->GetManager();

		// string→u16Stringに変換
		auto u16Path = ConvertToU16String(_pathName);

		// ロードする
		auto effect = Effekseer::Effect::Create(manager, u16Path.c_str(), _loadScale);
		if (effect == nullptr)	// 失敗したなら
		{
			HASHI_DEBUG_LOG(_pathName + " ロード失敗");
			return nullptr;
		}

		// エフェクトを作成
		std::unique_ptr<VisualEffect> pCreateVFX =
			std::make_unique<VisualEffect>(effect, _loadScale);
		VisualEffect* pRetPtr = pCreateVFX.get();

		// パス保存
		pCreateVFX->SetPathName(_pathName);

		// アセット管理に送る
		std::string vfxName = GetPathNameNotExt(_pathName);
		SendAsset<VisualEffect>(vfxName, std::move(pCreateVFX));

		return pRetPtr;
	}

	SoundAsset* AssetLoader::LoadSound(const std::string& _pathName)
	{
		DXSoundManager* pSoundManager = DXSoundManager::GetInstance();
		auto& audioEngine = pSoundManager->GetEngine();

		std::unique_ptr<DX::SoundEffect> loadSoundData;
		// wcha_t型に変換
		std::wstring wstr(_pathName.begin(), _pathName.end());
		try
		{
			loadSoundData = std::make_unique<DX::SoundEffect>(&audioEngine, wstr.c_str());
		}
		catch (...) // 失敗処理
		{
			HASHI_DEBUG_LOG(_pathName + "サウンドロード失敗");
			return nullptr;
		}

		// 自作アセットに移行
		auto soundAsset = std::make_unique<SoundAsset>();
		soundAsset->SetSoundData(std::move(loadSoundData));
		soundAsset->SetPathName(_pathName);

		// アセット名
		std::string assetName = PathToFileName(_pathName);

		// アセット管理に送る
		return SendAsset<SoundAsset>(assetName, std::move(soundAsset));
	}

	std::unique_ptr<Mesh_Group> AssetLoader::CreateMeshGroup(const aiScene* _pScene, const std::string& _assetName, float _loadScale, const DXSimp::Vector3& _loadAngles)
	{
		// ボーンがなかったら
		if (_pScene->mMeshes[0]->mNumBones == 0)
		{
			// スタティックメッシュを返す
			std::unique_ptr<StaticMesh> pSM = std::make_unique<StaticMesh>();
			pSM->SetAssetName(_assetName);
			return std::move(pSM);
		}

		std::unique_ptr<SkeletalMesh> pSkeletalMesh = std::make_unique<SkeletalMesh>();
		pSkeletalMesh->SetAssetName(_assetName);

		// ボーンを生成する
		CreateBone(_pScene, *pSkeletalMesh, _loadScale, _loadAngles);

		// ノードを生成する
		
		// 初期化
		loadNodeScaleMtx = Matrix::Identity;
		loadNodeRotationMtx = Matrix::Identity;
		loadNodeTransformMtx = Matrix::Identity;

		// AssimpFbxとついたノード名のローカル行列を対応したノードに渡すための行列
		std::unique_ptr<TreeNode> pRootNode = CreateNode(*_pScene->mRootNode, *pSkeletalMesh, false);
		pSkeletalMesh->SetRootNode(std::move(pRootNode));

		return std::move(pSkeletalMesh);
	}

	void AssetLoader::CreateBone(const aiScene* _pScene, SkeletalMesh& _skeletalMesh, float _loadScale, const DXSimp::Vector3& _loadOffsetAngles)
	{
		std::unique_ptr<BoneList> pCreateBones = std::make_unique<BoneList>();
		std::vector<std::unique_ptr<Bone>> pBones;

		// メッシュ一個目のボーン情報を見る
		const aiMesh* pAiMesh = _pScene->mMeshes[0];

		u_int boneIdx = 0;

		// ボーンを生成する
		for (u_int b_i = 0; b_i < pAiMesh->mNumBones; b_i++)
		{
			std::unique_ptr<Bone> pBone = std::make_unique<Bone>();
			const aiBone* pAiBone = pAiMesh->mBones[b_i];

			// ID
			pBone->SetIndex(boneIdx);
			boneIdx++;

			// 名前
			pBone->SetBoneName(pAiBone->mName.C_Str());

			// オフセット行列
			pBone->SetOffeetMtx(ToDirectXMatrix(pAiBone->mOffsetMatrix));

			HASHI_DEBUG_LOG(pBone->GetBoneName() + "：ロード完了");

			pBones.push_back(std::move(pBone));
		}

		pCreateBones->SetAssetBoneList(std::move(pBones));
		pCreateBones->loadScale = _loadScale;
		pCreateBones->loadOffsetRotation = Quat::ToQuaternion(_loadOffsetAngles);

		std::string boneName = _skeletalMesh.GetAssetName();
		BoneList* pRetBones = SendAsset<BoneList>(boneName, std::move(pCreateBones));

		// スケルタルメッシュにセット
		_skeletalMesh.SetAssetBoneList(pRetBones);
	}

	std::unique_ptr<TreeNode> AssetLoader::CreateNode(const aiNode& _aiChildNode,
		SkeletalMesh& _skeletalMesh,
		bool _isStoreAssimpFbxMtx)
	{
		std::unique_ptr<TreeNode> pCreateNode = std::make_unique<TreeNode>();

		// パラメータセット
		pCreateNode->SetNodeName(_aiChildNode.mName.C_Str());

		// AssimpFbxとついたノードを排除する
		if (pCreateNode->GetName().find("AssimpFbx") != std::string::npos)
		{
			std::string nodeName = pCreateNode->GetName();
			
			if (nodeName.find("Translation") != std::string::npos)
			{
				loadNodeTransformMtx = ToDirectXMatrix(_aiChildNode.mTransformation);
			}
			else if (nodeName.find("Scaling") != std::string::npos)
			{
				loadNodeScaleMtx = ToDirectXMatrix(_aiChildNode.mTransformation);
			}
			else if (nodeName.find("Rotation") != std::string::npos)
			{
				loadNodeRotationMtx = ToDirectXMatrix(_aiChildNode.mTransformation);
			}

			// 対応ノードに引き渡すために行列に保管する

			// 例外処理
			if (_aiChildNode.mNumChildren == 0) return nullptr;

			pCreateNode = CreateNode(*_aiChildNode.mChildren[0], _skeletalMesh, true);
		}

		// AssimpFbxと付いていないなら
		else
		{
			// 親ノードがAssimpFbxだったらローカル行列を受け継ぐ
			if (_isStoreAssimpFbxMtx)
			{
				// 全ての行列を掛ける
				Matrix localNodeMtx =
					loadNodeScaleMtx * loadNodeRotationMtx * loadNodeTransformMtx;
				pCreateNode->SetTransformMtx(ToDirectXMatrix(_aiChildNode.mTransformation) * localNodeMtx);

				// 初期化
				loadNodeScaleMtx = Matrix::Identity;
				loadNodeRotationMtx = Matrix::Identity;
				loadNodeTransformMtx = Matrix::Identity;
			}
			else
				pCreateNode->SetTransformMtx(ToDirectXMatrix(_aiChildNode.mTransformation));

			// 対応したボーンを名前から取得する
			Bone* pLinkBone = _skeletalMesh.GetBoneByName(_aiChildNode.mName.C_Str());
			if (pLinkBone)
				pCreateNode->SetBone(*pLinkBone);

			// 再帰でノードを生成する
			for (u_int n_i = 0; n_i < _aiChildNode.mNumChildren; n_i++)
			{
				std::unique_ptr<TreeNode> pChildNode =
					CreateNode(*_aiChildNode.mChildren[n_i], _skeletalMesh, false);

				pCreateNode->AddChild(std::move(pChildNode));
			}
		}

		return std::move(pCreateNode);
	}

	void AssetLoader::LinkVertexToBone(const aiMesh* _pAiMesh, SingleMesh& _singleMesh, SkeletalMesh& _skeletalMesh)
	{
		u_int vertexNum = _singleMesh.GetVertexNum();

		// 頂点にボーン情報を入れるときに何番目の配列に入れるかカウントする
		std::vector<u_int> vertexBoneCnt(vertexNum);

		// ボーン数ループ
		for (u_int bi = 0; bi < _pAiMesh->mNumBones; bi++)
		{
			const aiBone* pAiBone = _pAiMesh->mBones[bi];
			const Bone* pBone = _skeletalMesh.GetBoneByName(pAiBone->mName.C_Str());

			assert(pBone != nullptr && "Boneがありません");

			// ウェイト情報を取得する		
			// 頂点
			std::vector<Vertex>& verticies = _singleMesh.GetVerticies();

			// ウエイト数ループ
			for (u_int wi = 0; wi < pAiBone->mNumWeights; wi++)
			{
				float weight = pAiBone->mWeights[wi].mWeight;
				u_int vertexID = pAiBone->mWeights[wi].mVertexId;

				// 頂点とボーン情報をリンクさせる
				Vertex& v = verticies[vertexID];
				u_int& idx = vertexBoneCnt[vertexID];

				assert(idx < MAX_WEIGHT_NUM);
				v.boneWeight[idx] = weight;
				v.boneIndex[idx] = pBone->GetIndex();

				// カウントを増やす
				idx++;
			}
		}
	}

	std::unique_ptr<AnimationChannel> AssetLoader::CreateAnimChannel(const aiNodeAnim* _pAiAnimNode, const BoneList& _boneList)
	{
		std::unique_ptr<AnimationChannel> pCreateChannel = std::make_unique<AnimationChannel>();

		std::string channelName = _pAiAnimNode->mNodeName.C_Str();
		pCreateChannel->SetName(channelName);

		// 名前からボーンIDをセットする
		u_int boneIdx = _boneList.GetIndex(channelName);
		pCreateChannel->SetBoneIdx(boneIdx);

		// 座標キー
		for (u_int k_i = 0; k_i < _pAiAnimNode->mNumPositionKeys; k_i++)
		{
			Vector3 p = ToVector3(_pAiAnimNode->mPositionKeys[k_i].mValue);
			float t = static_cast<float>(_pAiAnimNode->mPositionKeys[k_i].mTime);
			pCreateChannel->AddPosKey(t, p);
		}

		// スケールキー
		for (u_int k_i = 0; k_i < _pAiAnimNode->mNumScalingKeys; k_i++)
		{
			Vector3 s = ToVector3(_pAiAnimNode->mScalingKeys[k_i].mValue);
			float t = static_cast<float>(_pAiAnimNode->mScalingKeys[k_i].mTime);
			pCreateChannel->AddScaleKey(t, s);
		}

		// 回転キー
		for (u_int k_i = 0; k_i < _pAiAnimNode->mNumRotationKeys; k_i++)
		{
			Quaternion q = ToQuaternion(_pAiAnimNode->mRotationKeys[k_i].mValue);
			float t = static_cast<float>(_pAiAnimNode->mRotationKeys[k_i].mTime);
			pCreateChannel->AddQuatKey(t, q);
		}

		return std::move(pCreateChannel);
	}

	float AssetLoader::GetAnimationTime(const aiAnimation* _pAiAnim)
	{
		double animationTime_s = 0.0f;

		if (_pAiAnim->mTicksPerSecond != 0)
		{
			animationTime_s = _pAiAnim->mDuration / _pAiAnim->mTicksPerSecond;
		}
		else
		{
			animationTime_s = _pAiAnim->mDuration;
		}

		return static_cast<float>(animationTime_s);
	}

	u_int AssetLoader::GetAnimationFrameCnt(const aiAnimation* _pAiAnim)
	{
		// アニメーションデータの1秒間のフレーム数
		constexpr u_int FRAME_PERSEC(30);

		//　1秒間のtick速度を求める
		double tickSpeedPerFrame = _pAiAnim->mTicksPerSecond / FRAME_PERSEC;

		// フレーム数を求める
		return static_cast<u_int>(_pAiAnim->mDuration / tickSpeedPerFrame);
	}

	float AssetLoader::GetTimePerKey(const aiAnimation* _pAiAnim)
	{
		double timePerKey_s = 0.0f;

		if (_pAiAnim->mTicksPerSecond != 0)
		{
			timePerKey_s = 1.0 / _pAiAnim->mTicksPerSecond;
		}
		else
		{
			timePerKey_s = 1.0;
		}

		return static_cast<float>(timePerKey_s);
	}

	std::string AssetLoader::PathToFileName(const std::string& _pathName)
	{
		fs::path fsPath = _pathName;

		return fsPath.filename().string();
	}

	std::string AssetLoader::GetParentPath(const std::string& _pathName)
	{
		fs::path fsPath = _pathName;
		return fsPath.parent_path().string();
	}

	std::string AssetLoader::GetPathNameNotExt(const std::string& _pathName)
	{
		fs::path fsPath = _pathName;
		return fsPath.stem().string();
	}

	std::u16string AssetLoader::ConvertToU16String(const std::string& _string)
	{
		std::u16string u16Str;
		for (unsigned char c : _string)
		{
			u16Str.push_back(c);
		}
		return u16Str;
	}

	void AssetLoader::UpdateSize(const DXSimp::Vector3& _vertexPos, DXSimp::Vector3& _max, DXSimp::Vector3& _min)
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

	DXSimp::Vector3 ToVector3(const aiVector3D& _aiVector3)
	{
		Vector3 vector3;
		vector3.x = _aiVector3.x; vector3.y = _aiVector3.y; vector3.z = _aiVector3.z;

		return vector3;
	}

	Quaternion ToQuaternion(const aiQuaternion& _aiQuat)
	{
		Quaternion retQuat = Quaternion(_aiQuat.x, _aiQuat.y, _aiQuat.z, _aiQuat.w);

		return retQuat;
	}

	DXSimp::Color ToColor(const aiColor4D& _aiColor)
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

	aiMatrix4x4 ToAssimpMatrix(const Matrix& _dxMatrix)
	{
		aiMatrix4x4 assimpMtx = aiMatrix4x4(
			_dxMatrix._11, _dxMatrix._21, _dxMatrix._31, _dxMatrix._41,
			_dxMatrix._12, _dxMatrix._22, _dxMatrix._32, _dxMatrix._42,
			_dxMatrix._13, _dxMatrix._23, _dxMatrix._33, _dxMatrix._43,
			_dxMatrix._14, _dxMatrix._24, _dxMatrix._34, _dxMatrix._44
		);

		return assimpMtx;
	}
}