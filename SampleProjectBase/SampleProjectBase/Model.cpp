#include "Model.h"
#include "Mesh.h"
#include "MaterialClass.h"

// assimp���C�u�����Ǎ�
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"


using namespace DirectX::SimpleMath;

Model::Model() : meshNum(0)
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
	if (meshNum > 0)
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
	if (pScene == nullptr)
	{
		ImGuiDebugLog::Add("���f���t�@�C������������܂���ł����B" 
			+ std::string(_settings.modelPath));
		return false;
	}

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
			float baseScale = _settings.scaleBase;
			if (mesh->Setup(_renderer, pMeshData, baseScale) == false)
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
	Color diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Color ambient(0.3f, 0.3f, 0.3f, 1.0f);
	Color specular(0.0f, 0.0f, 0.0f, 0.0f);
	Color emissive(0.0f, 0.0f, 0.0f, 0.0f);
	float shininess = 0.0f;

	// ���\�[�X�Ǘ��N���X�擾
	ResourceCollection* resourceCollection = ResourceCollection::GetInstance();

	// ���f���̊e�}�e���A�����쐬����
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		// �}�e���A���̃Z�b�g���閼�O�� "M_���O + ����"
		std::string materialName = "M_" + _settings.modelName + std::to_string(i);
		// �}�e���A�����쐬
		std::unique_ptr<MaterialClass> material = std::make_unique<MaterialClass>();
		// �e��p�����[�^�[
		MaterialParameter materialParam;
		materialParam.diffuse = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : diffuse;

		materialParam.ambient = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : ambient;

		materialParam.shininess = pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS ? shininess : 0.0f;

		materialParam.specular = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : specular;

		materialParam.emissive = pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS ?
			DirectX::XMFLOAT4(color.r, color.g, color.b, 1.0f) : emissive;

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
			std::string file = dir;	// ���f���Ɠ����K�w��T��
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
				std::string message = "���f���̃e�N�X�`���Ǎ����s�@" + file;
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
		}
		else // �V�[��������e�N�X�`���̃p�X���������Ȃ�������
		{
			std::string message = "���f���̃e�N�X�`���Ǎ����s�@" + std::string(path.C_Str());
			ImGuiDebugLog::Add(message);
		}

		// �p�����[�^���}�e���A���N���X�ɐݒ肷��
		material->SetMaterialParameter(materialParam);
		// �}�e���A���ǉ�
		pMaterials.push_back(material.get());
		// �}�e���A����ۊǃN���X�ɓ����
		resourceCollection->SetResource<MaterialClass>(materialName, std::move(material));
	}

	modelData = _settings;	// ���f����������
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
		if (meshIdx > pMaterials.size() - 1) return;	// �}�e���A�����Ȃ��Ȃ�I���
		MaterialClass& material = *pMaterials[meshIdx];

		// �p�����[�^
		MaterialParameter& materialParam = material.GetMaterialParameter();

		// �o�b�t�@���X�V����
		// ���_�V�F�[�_�[
		material.GetVertexShader().UpdateBuffer(0, &wvp);
		material.GetVertexShader().UpdateBuffer(1, &materialParam);


		material.GetPixelShader().UpdateBuffer(1, &materialParam);

		if (pTextures.size() > meshIdx && pTextures[meshIdx] != nullptr)	
			material.GetPixelShader().SetTexture(0, pTextures[meshIdx]);

		// �V�F�[�_�[���o�C���h
		material.GetVertexShader().Bind();
		material.GetPixelShader().Bind();

		// ���b�V���`��
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

	return true;
}

void Model::SetVertexShader(Shader* _vertexSh)
{
	for (auto material : pMaterials)
	{
		material->SetVertexShader(dynamic_cast<VertexShader*>(_vertexSh));
	}
}

void Model::SetPixelShader(Shader* _pixelSh)
{
	for (auto material : pMaterials)
	{
		material->SetPixelShader(dynamic_cast<PixelShader*>(_pixelSh));
	}
}

void Model::SetTexture(std::vector<Texture*> _setTextures)
{
	// �O�̃e�N�X�`���z������Z�b�g
	pTextures.clear();

	pTextures = _setTextures;	//�V�����e�N�X�`���z����Z�b�g����
}


