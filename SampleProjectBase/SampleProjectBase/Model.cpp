#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Direct3D11.h"
#include "ModelCollect.h"

// assimp���C�u�����Ǎ�
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
	// ���f���i�[�N���X�Ɋ��Ƀ��[�h����Ă���Ȃ烍�[�h�������Ȃ�
	bool isImported = ModelCollect::GetInstance()->GetIsImported(_settings.modelName);
	if (isImported)
		return true;

	if (_settings.modelPath == nullptr)
	{
		MessageError("���f���̃t�@�C���p�X���ݒ肳��Ă��܂���");
		return false;
	}
	if (_settings.modelName == "")
	{
		MessageError("���f���̖��O���ݒ肳��Ă��܂���");
		return false;
	}

	//���f���̃��[�h����
	Assimp::Importer importer;
	u_int flag = 0;
	flag |= aiProcess_Triangulate;	// �O�p�`�|���S��
	flag |= aiProcess_JoinIdenticalVertices;	// �������_����ɓ���
	flag |= aiProcess_FlipUVs;	//�@UV�l��Y������ɔ��]������
	flag |= aiProcess_PreTransformVertices;	// �m�[�h����ɓ��� �A�j���[�V������񂪏����邱�Ƃɒ���

	if (_settings.isRighthand)
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
				MessageError("���b�V���̃Z�b�g�A�b�v���s");
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

	Float4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Float4 ambient(0.3f, 0.3f, 0.3f, 1.0f);
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		Material* material = new Material();

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
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS) {
			// �t�@�C���`���`�F�b�N
			if (strstr(path.C_Str(), ".psd"))
			{
				MessageError("psd�ɂ͑Ή����Ă��܂���");
			}

			bool isSuccess = false;
			// ���̂܂ܒT��
			material->texture = new Texture();
			// ���f���Ɠ����K�w��T��
			std::string file = dir;
			file += path.C_Str();
			isSuccess = material->texture->Load(file.c_str());

			// �t�@�C�����݂̂ŒT��
			if (!isSuccess) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					isSuccess = material->texture->Load(file.c_str());
				}

				// �t�@�C���ڂ̃p�X��"\\�ł͂Ȃ�"/"�̏ꍇ�̑Ή�
				if (!isSuccess) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						isSuccess = material->texture->Load(file.c_str());
					}
				}
			}
			// ���s
			if (!isSuccess) {
				CLASS_DELETE(material->texture);
			}
		}

		// �}�e���A���ǉ�
		materials.push_back(material);
	}

	return true;
}

void Model::SetupTransform(const Transform& _transform)
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	CbTransformSet& cb = renderer.GetParameter().cbTransformSet;

	// �ړ��s��
	DirectX::XMMATRIX moveMatrix = DirectX::XMMatrixTranslation(_transform.position.x, _transform.position.y, _transform.position.z);
	// �g��s��
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(_transform.scale.x, _transform.scale.y, _transform.scale.z);
	// ��]�s��
	DirectX::XMMATRIX rotateMatX = DirectX::XMMatrixRotationX(_transform.rotation.x);
	DirectX::XMMATRIX rotateMatY = DirectX::XMMatrixRotationY(_transform.rotation.y);
	DirectX::XMMATRIX rotateMatZ = DirectX::XMMatrixRotationZ(_transform.rotation.z);
	DirectX::XMMATRIX rotateMatrix = rotateMatX * rotateMatY * rotateMatZ;

	DirectX::XMMATRIX mtx = scaleMatrix
		* rotateMatrix
		* moveMatrix;

	DirectX::XMStoreFloat4x4(&cb.data.transform, XMMatrixTranspose(mtx));

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	// CBuffer�ɂЂ��Â��n�[�h�E�F�A���\�[�X�}�b�v�擾�i���b�N���Ď擾�j
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
	// �}�b�v����
	pDeviceContext->Unmap(cb.pBuffer, 0);


	pDeviceContext->VSSetConstantBuffers(0, 1, &cb.pBuffer);

	return;
}

void Model::Draw(const Transform& _transform)
{
	SetupTransform(_transform);
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// �e�N�X�`�����s�N�Z���V�F�[�_�[�ɑ���
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
	// ���f�������[�h����
	bool isSuccess = Load(_settings, *Direct3D11::GetInstance()->GetRenderer());
	if (!isSuccess)
		return false;

	// ���f��������������
	modelData = _settings;
	// ���f���������ꂽ�t���O�𗧂Ă�
	isImported = true;

	return true;
}