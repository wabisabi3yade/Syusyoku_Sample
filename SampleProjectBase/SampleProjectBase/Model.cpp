#include "Model.h"
#include "Mesh.h"
#include "Material.h"

// assimp���C�u�����Ǎ�
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
	// ���f���������Z�b�g����
	ResetParam();

	// ���b�V���̐����擾
	meshNum = _setModel.GetMeshNum();

	const std::vector<Mesh*> _setMeshes = _setModel.GetMeshes();

	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// �C���X�^���X�𐶐�����
		Mesh* mesh = new Mesh();

		// �f�[�^�����f���󃁃b�V�����擾����
		const Mesh* setMesh = _setMeshes[meshIdx];
		if (setMesh == nullptr)	// ����������
		{
			ImGuiDebugLog::Add("���b�V���̃|�C���^��nullptr�ł�");
			// �p�����[�^�����Z�b�g����
			ResetParam();
			return false;	// ���s
		}

		// ���b�V���̏����Z�b�g����
		mesh->SetMesh(*setMesh);
		// �z��ɃZ�b�g����
		meshes.push_back(mesh);
	}

	// ���f���̖��O�Ȃǂ̏����擾����
	modelData = _setModel.GetModelData();
	return true;
}

bool Model::LoadProcess(const ModelSettings& _settings, D3D11_Renderer& _renderer)
{
	// ���ɂ��̃��f���������Ă�����
	if (isImported)
		return true;

	if (_settings.modelPath == nullptr)
	{
		ImGuiDebugLog::Add("���f���̃t�@�C���p�X���ݒ肳��Ă��܂���");
		return false;
	}
	if (_settings.modelName == "")
	{
		ImGuiDebugLog::Add("���f���̖��O���ݒ肳��Ă��܂���");
		return false;
	}

	//���f���̃��[�h����
	Assimp::Importer importer;
	u_int flag = 0;
	flag |= aiProcess_Triangulate;	// �O�p�`�|���S��
	flag |= aiProcess_JoinIdenticalVertices;	// �������_����ɓ���
	flag |= aiProcess_FlipUVs;	//�@UV�l��Y������ɔ��]������
	flag |= aiProcess_PreTransformVertices;	// �m�[�h����ɓ��� �A�j���[�V������񂪏����邱�Ƃɒ���

	if (!_settings.isRighthand)
		flag |= aiProcess_MakeLeftHanded;		// ����n���W�ɕϊ�

	// ���f����ǂݍ���
	auto pScene = importer.ReadFile(_settings.modelPath, flag);
	if (pScene == nullptr) return false;

	// ���b�V���̐����擾����
	meshNum = pScene->mNumMeshes;

	if (meshNum > 0)
	{
		// ���b�V���̐�����������
		for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
		{
			Mesh* mesh = new Mesh();	// ���b�V���쐬
			// ���b�V���̏����擾
			auto pMeshData = pScene->mMeshes[meshIdx];

			// ���b�V���̏���
			if (mesh->Setup(_renderer, pMeshData) == false)
			{
				ImGuiDebugLog::Add("���b�V���̃Z�b�g�A�b�v���s");
				// �쐬�������b�V�������
				CLASS_DELETE(mesh);

				return false;
			}

			meshes.push_back(mesh);
		}
	}

	// �}�e���A���̍쐬
	// �t�@�C���̒T��
	std::string dir = _settings.modelPath;
	dir = dir.substr(0, dir.find_last_of('/') + 1);

	aiColor3D color(0.0f, 0.0f, 0.0f);
	Vector4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 ambient(0.3f, 0.3f, 0.3f, 1.0f);

	// ���\�[�X�Ǘ��N���X�擾
	ResourceCollection* resourceCollection = ResourceCollection::GetInstance();

	// ���f���̊e�}�e���A�����쐬����
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		// �}�e���A���̃Z�b�g���閼�O�� "M_���O + ����"
		std::string materialName = "M_" + _settings.modelName + std::to_string(i);
		// �}�e���A�����쐬
		std::unique_ptr<Material> material = std::make_unique<Material>();
		// �e��p�����[�^�[
		float shininess;
		material->diffuse = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;
		material->ambient = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;
		shininess = pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;
		material->specular = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, shininess) : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, shininess);

		// �e�N�X�`��
		// �e�N�X�`���̃C���X�^���X���쐬
		std::unique_ptr<Texture> texture = std::make_unique<Texture>();
		// �e�N�X�`���̖��O��������T_�ɂ���
		std::string texName = "T_" + _settings.modelName + std::to_string(i);
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			// �t�@�C���`���`�F�b�N
			if (strstr(path.C_Str(), ".psd"))
			{
				ImGuiDebugLog::Add("psd�ɂ͑Ή����Ă��܂���");
			}
			bool isSuccess = false;	// ���[�h�����������t���O
			// ���f���Ɠ����K�w��T��
			std::string file = dir;
			file += path.C_Str();
			// �e�N�X�`���Ǘ��N���X�Ƀ��f���̃e�N�X�`����������(���O�̓��f���ƈꏏ)
			isSuccess = texture->Load(file.c_str());

			// �t�@�C�����݂̂ŒT��
			if (!isSuccess) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					isSuccess = texture->Load(file.c_str());
				}

				// �t�@�C���ڂ̃p�X��"\\�ł͂Ȃ�"/"�̏ꍇ�̑Ή�
				if (!isSuccess) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						isSuccess = texture->Load(file.c_str());
					}
				}
			}
			// ���s
			if (!isSuccess) {
				std::string message = "���f���̃e�N�X�`���Ǎ����s�@" + texName;
				ImGuiDebugLog::Add(message);
			}
			else
			{
				// �e�N�X�`���Ǎ��ݐ���
				// �ǂݍ��񂾃e�N�X�`�����}�e���A���ɐݒ肷��
				pTextures.push_back(texture.get());
				// ���\�[�X�Ǘ��Ƀe�N�X�`���ǉ�
				resourceCollection->SetResource<Texture>(texName, std::move(texture));
			}

			// �}�e���A���ǉ�
			pMaterials.push_back(material.get());
			// �}�e���A����ۊǃN���X�ɓ����
			resourceCollection->SetResource<Material>(materialName, std::move(material));
		}
	}

	return true;
}

void Model::Draw(const Transform& _transform) const
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���[���h�ϊ��s����擾
	DirectX::SimpleMath::Matrix worldMatrix = D3D11_Renderer::GetWorldMtx(_transform);

	// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP();
	wvp.world = worldMatrix;

	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// �o�b�t�@���X�V����
		pMaterials[meshIdx]->pVertexShader->UpdateBuffer(0, &wvp);
		// �e�N�X�`���ݒ�
		if(pTextures.size() > meshIdx)	// �e�N�X�`���Ȃ��Ă��ǂݍ��߂�p�ɂ������̂�
		pMaterials[meshIdx]->pPixelShader->SetTexture(0, pTextures[meshIdx]);
		// �V�F�[�_�[���o�C���h
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
	// ���f�������[�h����
	bool isSuccess = LoadProcess(_settings, *Direct3D11::GetInstance()->GetRenderer());
	if (!isSuccess)
		return false;

	// ���f��������������
	modelData = _settings;
	// ���f���������ꂽ�t���O�𗧂Ă�
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

