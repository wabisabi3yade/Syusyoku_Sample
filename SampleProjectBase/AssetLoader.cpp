#include "pch.h"
#include "AssetLoader.h"

// assimp���C�u�����Ǎ�
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// �e�N�X�`��
// stb_image���C�u����
#define STB_IMAGE_IMPLEMENTATION
#include	"stb_image.h"
#include "Texture.h"

// ���b�V���Q
#include "Mesh_Group.h"
#include "StaticMesh.h"
#include "SkeletalMesh.h"

// �A�j���[�V����
#include "AnimationData.h"

namespace fs = std::filesystem;

using namespace DirectX::SimpleMath;

Assimp::Importer g_importer;

/// @brief aiVector3D�^��SimpleMath��Vector3�ɕϊ�
/// @param _aiVector3 �ϊ�����l
/// @return �ϊ����ꂽVector3
static Vector3 ToVector3(const aiVector3D& _aiVector3);

/// @brief aiColor4D�^��SimpleMath��Color�ɕϊ�
/// @param _aiColor �ϊ�����l
/// @return �ϊ����ꂽColor
static Color ToColor(const aiColor4D& _aiColor);

/// @brief aiMatrix��DirectX�̍s��ɕϊ�
/// @param _aiMatrix assimp�s��
/// @return DirectX�s��
static Matrix ToDirectXMatrix(const aiMatrix4x4& _aiMatrix);

void AssetLoader::MaterialLoad(Mesh_Group* _pMeshgather,
	const aiScene* pScene, std::string texturedirectory)
{
	// �}�e���A���������[�v
	for (unsigned int m = 0; m < pScene->mNumMaterials; m++)
	{
		aiMaterial* material = pScene->mMaterials[m];

		// �}�e���A�����擾
		std::string mtrlname = material->GetName().C_Str();
		if (mtrlname.length() <= 0)
		{
			mtrlname = "M_" + std::string(pScene->mMeshes[m]->mName.C_Str());
		}

		// �}�e���A�����
		aiColor4D ambient;
		aiColor4D diffuse;
		aiColor4D specular;
		aiColor4D emission;
		float shiness;
		// �A���r�G���g
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
		}
		else
		{
			ambient = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// �f�B�t���[�Y
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
		}
		else {
			diffuse = aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �X�y�L����
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
		}
		else {
			specular = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// �G�~�b�V����
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission)) {
		}
		else {
			emission = aiColor4D(0.0f, 0.0f, 0.0f, 0.0f);
		}

		// �V���C�l�X
		if (AI_SUCCESS == aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shiness)) {
		}
		else {
			shiness = 0.0f;
		}

		// �}�e���A���̃p�����[�^���Z�b�g
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
			// t�Ԗڂ̃e�N�X�`���p�X�擾
			if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, t, &path))
			{
				// �e�N�X�`���p�X�擾
				std::string texPath = path.C_Str();

				// �����e�N�X�`�����ǂ����𔻒f����
				if (auto aiTex = pScene->GetEmbeddedTexture(path.C_Str()))
				{
					// �����e�N�X�`���̏ꍇ
					bool sts = LoadEmbeddedTexture(
						*texture.get(),
						*aiTex
					);

					// �ǂݍ��߂���
					if (sts)
						break;

				}
				else // �O���e�N�X�`���t�@�C���̏ꍇ
				{
					std::string texname = texturedirectory + "/" + PathToFileName(texPath, true);

					texture = MakeTexture(texname);
				}
			}
			// �f�B�t���[�Y�e�N�X�`�����Ȃ������ꍇ
			else
			{
				HASHI_DEBUG_LOG("�}�e���A�����F" + mtrlname +
					std::to_string(t) + "�Ԗڂ̃e�N�X�`���͂���܂���");
			}
		}

		// �e�N�X�`�����Ǘ��ɃZ�b�g
		std::string texName = texture->GetName();
		Texture* pTex = SendAsset(texName, std::move(texture));

		createMaterial->SetDiffuseTexture(*pTex);

		// �Ǘ��N���X�ɃZ�b�g����
		Material* materialPtr =
			SendAsset<Material>(mtrlname, std::move(createMaterial));

		// ���b�V���Ƀ}�e���A���ǉ�
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

	// �摜�ǂݍ���
	pixels = stbi_load_from_memory(Data,
		len,
		&texWidth,
		&texHeight,
		nullptr,
		STBI_rgb_alpha);

	// �e�N�X�`��2D���\�[�X����
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

	// SRV����
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, _texture.pSRV.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// �s�N�Z���C���[�W���
	stbi_image_free(pixels);

	// ���O�ݒ�
	std::string texName = PathToFileName(_aiTex.mFilename.C_Str(), true);
	_texture.SetName(texName);

	// ���ݒ�
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

	// �摜�ǂݍ���
	pixels = stbi_load(_filePath.c_str(), &width, &height, nullptr, 4);
	if (pixels == nullptr)
	{

		HASHI_DEBUG_LOG("�摜�Ǎ����s");
		return std::move(pMakeTex);
	}

	// �e�N�X�`��2D���\�[�X����
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
		HASHI_DEBUG_LOG("�e�N�X�`���쐬���s");

		stbi_image_free(pixels);
		return std::move(pMakeTex);
	}

	// SRV����
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, pMakeTex->pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		HASHI_DEBUG_LOG("SRV�쐬���s");
		stbi_image_free(pixels);
		return std::move(pMakeTex);
	}

	// �s�N�Z���C���[�W���
	stbi_image_free(pixels);

	pMakeTex->width = static_cast<u_int>(width);
	pMakeTex->height = static_cast<u_int>(height);

	// �p�X���疼�O�擾����
	std::string texName = PathToFileName(_filePath, true);
	pMakeTex->SetName(texName);

	return std::move(pMakeTex);
}

Texture* AssetLoader::TextureLoad(const std::string& _filePath)
{
	// �e�N�X�`�����쐬
	std::unique_ptr<Texture> pTexture = MakeTexture(_filePath);

	if (pTexture == nullptr)
		return nullptr;

	std::string texName = pTexture->GetName();

	// �A�Z�b�g�Ǘ��N���X�ɃZ�b�g
	Texture* returnPtr = SendAsset<Texture>(texName, std::move(pTexture));

	return returnPtr;
}

Mesh_Group* AssetLoader::ModelLoad(const std::string& _modelPath, float _scale, bool _isLeftHand, bool _isGetScale)
{
	// �V�[�����\�z
	Assimp::Importer importer;

	int flag = aiProcess_Triangulate;
	flag |= aiProcess_JoinIdenticalVertices;
	flag |= aiProcess_FlipUVs;
	flag |= aiProcess_CalcTangentSpace;
	if (_isLeftHand)
		flag |= aiProcess_MakeLeftHanded;


	// �V�[�������\�z
	const aiScene* pScene = importer.ReadFile(
		_modelPath.c_str(),
		flag);			// �O�p�`������

	if (pScene == nullptr)
	{
		HASHI_DEBUG_LOG("�Ǎ����s�F" + _modelPath);
	}

	assert(pScene != nullptr);

	// ���b�V���̃O���[�v���쐬����
	// �����Ń{�[�������ǂݍ���
	std::unique_ptr<Mesh_Group> pMeshGroup = CreateMeshGroup(pScene);

	// �e�p�X��
	std::string parentPath = GetParentPath(_modelPath);

	// �}�e���A�����擾
	MaterialLoad(pMeshGroup.get(), pScene, parentPath);

	// ���b�V���̍ő�E�ŏ����W
	Vector3 modelMaxPos = Vector3::One * -10.0f;
	Vector3 modelMinPos = Vector3::One * 10.0f;

	// �{�[���̃C���f�b�N�X(���[�g�m�[�h�̎�����쐬����)
	u_int boneIdx = 0;

	// ���b�V�������[�v
	for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
	{
		std::unique_ptr<SingleMesh> pCreateMesh = std::make_unique<SingleMesh>();

		aiMesh* pAimesh = pScene->mMeshes[m];
	
		// ���b�V�����擾
		std::string meshname = std::string(pAimesh->mName.C_Str());
		pCreateMesh->SetName(meshname);

		// �}�e���A���C���f�b�N�X�擾
		u_int materialIdx = pAimesh->mMaterialIndex;
		pCreateMesh->SetMaterialID(materialIdx);

		// ���_�̍ő�E�ŏ����W
		Vector3 maxVPos = Vector3::One * -10.0f, minVPos = Vector3::One * 10.0f;

		std::vector<Vertex>& verticies = pCreateMesh->GetVerticies();
		//�@���_�������[�v
		for (unsigned int vidx = 0; vidx < pAimesh->mNumVertices; vidx++)
		{
			// ���_�f�[�^
			Vertex vertex;

			// ���W(�X�P�[���l�𔽉f����)
			vertex.position = ToVector3(pAimesh->mVertices[vidx]) * _scale;

			// �ő�E�ŏ����X�V
			if (_isGetScale)
				UpdateSize(vertex.position, maxVPos, minVPos);


			// �@������H
			if (pAimesh->HasNormals())
				vertex.normal = ToVector3(pAimesh->mNormals[vidx]);
			else
				vertex.normal = Vector3::Zero;

			// ���_�J���[�H�i�O�Ԗځj
			if (pAimesh->HasVertexColors(0))
				vertex.color = ToColor(pAimesh->mColors[0][vidx]);
			else
				vertex.color = Vector4::One;

			// �e�N�X�`������H�i�O�Ԗځj
			if (pAimesh->HasTextureCoords(0))
			{
				vertex.uv.x = pAimesh->mTextureCoords[0][vidx].x;
				vertex.uv.y = pAimesh->mTextureCoords[0][vidx].y;
			}
			else
				vertex.uv = Vector2::Zero;

			// ���_�f�[�^��ǉ�
			verticies.push_back(vertex);
		}

		// �C���f�b�N�X���擾����
		std::vector<u_int>& indicies = pCreateMesh->GetIndicies();

		// �C���f�b�N�X�������[�v
		for (unsigned int fidx = 0; fidx < pAimesh->mNumFaces; fidx++)
		{
			aiFace face = pAimesh->mFaces[fidx];

			assert(face.mNumIndices == 3);	// �O�p�`�̂ݑΉ�

			// �C���f�b�N�X�f�[�^��ǉ�
			for (unsigned int i = 0; i < face.mNumIndices; i++)
				indicies.push_back(face.mIndices[i]);
		}

		// ���f���̍ő�E�ŏ����X�V
		if (_isGetScale)
		{
			// ���b�V���̒��S���W�E�T�C�Y���Z�b�g
			pCreateMesh->SetCenterPosition((maxVPos + minVPos) * 0.5f);
			pCreateMesh->SetSize(maxVPos - minVPos);

			UpdateSize(maxVPos, modelMaxPos, modelMinPos);
			UpdateSize(minVPos, modelMaxPos, modelMinPos);
		}

		// �X�P���^�����b�V���Ȃ�
		if (pMeshGroup->GetType() == Mesh_Group::MeshType::SK)
		{
			SkeletalMesh& pSK = static_cast<SkeletalMesh&>(*pMeshGroup.get());

			// �{�[�������擾����
			BonePerMesh bonesPerMesh = CreateBone(pAimesh, *pCreateMesh.get(), boneIdx);

			pSK.AddBonePerMesh(std::move(bonesPerMesh));
		}

		// �o�b�t�@����
		pCreateMesh->InitBuffer();

		pMeshGroup->AddMesh(std::move(pCreateMesh));
	}
	// ���O�ݒ�
	std::string modelName = PathToFileName(_modelPath, false);
	pMeshGroup->SetName(modelName);

	// ���f���̒��S���W�E�T�C�Y���Z�b�g
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
	
	// ����n�ɕϊ�
	if (_isLeftHand)
		flag |= aiProcess_ConvertToLeftHanded;	

	auto pAiScene = g_importer.ReadFile(_animPath.c_str(), flag);
	if (pAiScene == nullptr)
	{
		HASHI_DEBUG_LOG(_animPath + "�F�A�j���[�V����������ɓǂݍ��߂܂���ł���");
		return nullptr;
	}

	std::unique_ptr<AnimationData> pAnimData = std::make_unique<AnimationData>();
	pAnimData->SetAiScene(pAiScene);

	// ���O���p�X������擾
	std::string assetName = PathToFileName(_animPath, false);
	pAnimData->SetName(assetName);

	// �A�Z�b�g�Ǘ��ɃZ�b�g
	AnimationData* pRetAnim = SendAsset<AnimationData>(assetName, std::move(pAnimData));

	return pRetAnim;
}

std::unique_ptr<Mesh_Group> AssetLoader::CreateMeshGroup(const aiScene* _pScene)
{
	// �{�[�����Ȃ�������
	if (_pScene->mMeshes[0]->mNumBones == 0)
	{
		// �X�^�e�B�b�N���b�V����Ԃ�
		std::unique_ptr<StaticMesh> pSM = std::make_unique<StaticMesh>();
		return std::move(pSM);
	}

	std::unique_ptr<SkeletalMesh>pSkeletalMesh = std::make_unique<SkeletalMesh>();

	//// ���[�g�m�[�h�̃{�[�������쐬����
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

	// ���_�Ƀ{�[����������Ƃ��ɉ��Ԗڂ̔z��ɓ���邩�J�E���g����
	u_int vertexNum = _singleMesh.GetVertexNum();
	std::vector<u_int> vertexBoneCnt(vertexNum);

	// �{�[�������[�v
	for (u_int bi = 0; bi < _pAiMesh->mNumBones; bi++)
	{
		const aiBone* pAiBone = _pAiMesh->mBones[bi];
		std::unique_ptr<Bone> pBone = std::make_unique<Bone>();

		// �p�����[�^���擾
		pBone->SetBoneName(std::string(pAiBone->mName.C_Str()));

		// �f�o�b�O�p
		HASHI_DEBUG_LOG("�{�[���F" + pBone->GetBoneName());

		// �I�t�Z�b�g�s��
		pBone->SetOffeetMtx(ToDirectXMatrix(pAiBone->mOffsetMatrix));

		// �C���f�b�N�X���擾
		pBone->SetIndex(_boneIdx);

		// �E�F�C�g�����擾����		
		// ���_
		std::vector<Vertex>& verticies = _singleMesh.GetVerticies();

		// �E�G�C�g�����[�v
		for (u_int wi = 0; wi < pAiBone->mNumWeights; wi++)
		{
			Weight weight;
			weight.boneName = pBone->GetBoneName();

			weight.weight = pAiBone->mWeights[wi].mWeight;
			weight.vertexIndex = pAiBone->mWeights[wi].mVertexId;

			// ���_�ƃ{�[�����������N������
			Vertex& v = verticies[weight.vertexIndex];
			u_int& idx = vertexBoneCnt[weight.vertexIndex];

			assert(idx < MAX_WEIGHT_NUM);
			v.boneWeight[idx] = weight.weight;
			v.boneIndex[idx] = pBone->GetIndex();

			// �J�E���g�𑝂₷
			idx++;

			// �E�F�C�g��ǉ�����
			pBone->AddWeight(weight);
		}

		// 1���b�V���̃{�[���z��ɒǉ�
		retBones.push_back(std::move(pBone));

		_boneIdx++;	// ���Z
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
