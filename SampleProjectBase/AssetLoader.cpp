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
#include "DirectXTex/TextureLoad.h"
#include "Texture.h"

// ���f��
#include "Mesh_Base.h"

namespace fs = std::filesystem;

using namespace DirectX::SimpleMath;

/// @brief aiVector3D�^��SimpleMath��Vector3�ɕϊ�
/// @param _aiVector3 �ϊ�����l
/// @return �ϊ����ꂽVector3
static Vector3 ToVector3(const aiVector3D& _aiVector3);

/// @brief aiColor4D�^��SimpleMath��Color�ɕϊ�
/// @param _aiColor �ϊ�����l
/// @return �ϊ����ꂽColor
static Color ToColor(const aiColor4D& _aiColor);

void AssetLoader::MaterialLoad(Mesh_Base* _pMeshgather,
	const aiScene* pScene, std::string texturedirectory)
{
	// �}�e���A���������[�v
	for (unsigned int m = 0; m < pScene->mNumMaterials; m++)
	{
		aiMaterial* material = pScene->mMaterials[m];

		// �}�e���A�����擾
		std::string mtrlname = material->GetName().C_Str();

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
				if (auto tex = pScene->GetEmbeddedTexture(path.C_Str()))
				{
					// �����e�N�X�`���̏ꍇ
					bool sts = TextureMemoryLoad(
						*texture.get(),
						(unsigned char*)tex->pcData,
						tex->mWidth
					);

					// �ǂݍ��߂���
					if (sts)
						break;

				}
				else // �O���e�N�X�`���t�@�C���̏ꍇ
				{
					std::string texname = texturedirectory + "/" + PathToFileName(texPath,true);

					Texture* diffuseTex = TextureLoad(texname);
					createMaterial->SetDiffuseTexture(*diffuseTex);
				}
			}
			// �f�B�t���[�Y�e�N�X�`�����Ȃ������ꍇ
			else
			{
				ImGuiDebugLog::Add("�}�e���A�����F" + mtrlname +
					std::to_string(t) + "�Ԗڂ̃e�N�X�`���͂���܂���");
			}
		}

		// �Ǘ��N���X�ɃZ�b�g����
		Material* materialPtr =
			SendAsset<Material>(mtrlname, std::move(createMaterial));

		// ���b�V���Ƀ}�e���A���ǉ�
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
		ImGuiDebugLog::Add("�e�N�X�`���̃p�X�������͂���Ă��܂���");
		return nullptr;
	}

	// �e�N�X�`�������[�h����
	std::unique_ptr<Texture> pMakeTex = std::make_unique<NullTexture>();

	// �}���`�o�C�g����������C�h������ɕϊ�
	wchar_t widePath[MAX_PATH];
	size_t wideLen = 0;
	MultiByteToWideChar(0, 0, _filePath.c_str(), -1, widePath, MAX_PATH);

	// �t�@�C���Ǎ�
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT hr;

	// �ǂݍ��܂ꂽ�t�@�C����tga�`���Ȃ�
	if (strstr(_filePath.c_str(), ".tga"))
	{
		hr = DirectX::LoadFromTGAFile(widePath, &metaData, image);
	}
	// �ǂݍ��܂ꂽ�t�@�C����psd�`���Ȃ�
	else if (strstr(_filePath.c_str(), ".psd"))
	{
		std::string message = "psd�t�@�C���͑Ή����Ă��܂���\n" + std::string(_filePath);
		ImGuiDebugLog::Add(message);

		return nullptr;
	}
	else
	{
		// Windows�ɑΉ������摜�`��(bmp, png, gif, tiff, jpeg�Ȃ�)��ǂݍ���
		hr = DirectX::LoadFromWICFile(widePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &metaData, image);
	}

	if (FAILED(hr))
	{
		std::string message = "�e�N�X�`���Ǎ����s�F" + std::string(_filePath);
		ImGuiDebugLog::Add(message);
		return nullptr;
	}

	// SRV���쐬
	ID3D11Device* device = Direct3D11::GetInstance()->GetRenderer()->GetDevice();
	hr = DirectX::CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), metaData, &pMakeTex->pSRV);

	if (FAILED(hr))
	{
		ImGuiDebugLog::Add("SRV�쐬�ŃG���[");
		return nullptr;
	}

	pMakeTex->width = static_cast<u_int>(metaData.width);
	pMakeTex->height = static_cast<u_int>(metaData.height);

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

Mesh_Base* AssetLoader::ModelLoad(const std::string& _modelPath, float _scale, bool _isLeftHand, bool _isGetScale)
{
	std::unique_ptr<Mesh_Base> pMeshGather = std::make_unique<Mesh_Base>();

	// �V�[�����\�z
	Assimp::Importer importer;

	int flag = aiProcess_Triangulate;

	if (_isLeftHand)
		flag |= aiProcess_ConvertToLeftHanded;


	// �V�[�������\�z
	const aiScene* pScene = importer.ReadFile(
		_modelPath.c_str(),
		flag);			// �O�p�`������

	if (pScene == nullptr)
	{
		ImGuiDebugLog::Add("�Ǎ����s�F" + _modelPath);
	}
	assert(pScene != nullptr);

	// �e�p�X��
	std::string parentPath = GetParentPath(_modelPath);

	// �}�e���A�����擾
	MaterialLoad(pMeshGather.get(), pScene, parentPath);

	// ���b�V���̍ő�E�ŏ����W
	Vector3 modelMaxPos;
	Vector3 modelMinPos;

	for (unsigned int m = 0; m < pScene->mNumMeshes; m++)
	{
		std::unique_ptr<SingleMesh> pCreateMesh = std::make_unique<SingleMesh>();

		aiMesh* mesh = pScene->mMeshes[m];

		// ���b�V�����擾
		std::string meshname = std::string(mesh->mName.C_Str());
		pCreateMesh->SetName(meshname);

		// �}�e���A���C���f�b�N�X�擾
		u_int materialIdx = mesh->mMaterialIndex;
		pCreateMesh->SetMaterialID(materialIdx);

		// ���_�̍ő�E�ŏ����W
		Vector3 maxVPos, minVPos;
		
		std::vector<Vertex>& verticies = pCreateMesh->GetVerticies();
		//�@���_�������[�v
		for (unsigned int vidx = 0; vidx < mesh->mNumVertices; vidx++)
		{
			// ���_�f�[�^
			Vertex vertex;

			// ���W(�X�P�[���l�𔽉f����)
			vertex.position = ToVector3(mesh->mVertices[vidx]) * _scale;

			// �ő�E�ŏ����X�V
			if (_isGetScale)
				UpdateSize(vertex.position, maxVPos, minVPos);


			// �@������H
			if (mesh->HasNormals()) {
				vertex.normal = ToVector3(mesh->mNormals[vidx]);
			}
			else
			{
				vertex.normal = Vector3::Zero;
			}

			// ���_�J���[�H�i�O�Ԗځj
			if (mesh->HasVertexColors(0)) {
				vertex.color = ToColor(mesh->mColors[0][vidx]);
			}
			else
			{
				vertex.color = Vector4::One;
			}

			// �e�N�X�`������H�i�O�Ԗځj
			if (mesh->HasTextureCoords(0)) {
				vertex.uv.x = mesh->mTextureCoords[0][vidx].x;
				vertex.uv.y = mesh->mTextureCoords[0][vidx].y;
			}
			else
			{
				vertex.uv = Vector2::Zero;
			}

			// ���_�f�[�^��ǉ�
			verticies.push_back(vertex);
		}

		// �C���f�b�N�X���擾����
		std::vector<u_int>& indicies = pCreateMesh->GetIndicies();

		// �C���f�b�N�X�������[�v
		for (unsigned int fidx = 0; fidx < mesh->mNumFaces; fidx++)
		{
			aiFace face = mesh->mFaces[fidx];

			assert(face.mNumIndices == 3);	// �O�p�`�̂ݑΉ�

			// �C���f�b�N�X�f�[�^��ǉ�
			for (unsigned int i = 0; i < face.mNumIndices; i++)
				indicies.push_back(face.mIndices[i]);
		}

		// �o�b�t�@����
		pCreateMesh->InitBuffer();

		// ���b�V���̃T�C�Y���Z�b�g
		pCreateMesh->SetSize(maxVPos - minVPos);
		
		// ���f���̍ő�E�ŏ����X�V
		if (_isGetScale)
		{
			UpdateSize(maxVPos, modelMaxPos, modelMinPos);
			UpdateSize(minVPos, modelMaxPos, modelMinPos);
		}
		
		pMeshGather->AddMesh(std::move(pCreateMesh));
	}
	// ���O�ݒ�
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
