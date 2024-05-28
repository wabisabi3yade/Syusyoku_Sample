#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Direct3D11.h"

#include "TextureCollect.h"
#include "MaterialCollect.h"

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

bool Model::SetModel(const Model& _setModel)
{
	// ���f���������Z�b�g����
	ResetParam();

	// ���b�V���̐����擾
	meshNum = _setModel.GetMeshNum();

	const std::vector<Mesh*> _setMeshes = _setModel.GetMeshes();

	for (int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		// �C���X�^���X�𐶐�����
		Mesh* mesh = new Mesh();

		// �f�[�^�����f���󃁃b�V�����擾����
		const Mesh* setMesh = _setMeshes[meshIdx];
		if (setMesh == nullptr)	// ����������
		{
			// �G���[
			MessageError("���b�V���̃|�C���^��nullptr�ł�");
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

bool Model::Load(const ModelSettings& _settings, D3D11_Renderer& _renderer)
{
	// ���ɂ��̃��f���������Ă�����
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
				MessageError("���b�V���̃Z�b�g�A�b�v���s");
				// �쐬�������b�V�������
				CLASS_DELETE(mesh);

				return false;
			}

			meshes.push_back(mesh);
		}
	}

	// �}�e���A���̍쐬
	// �}�e���A����ۊǃN���X�ɓ����
	MaterialCollect* mC = MaterialCollect::GetInstance();

	// �t�@�C���̒T��
	std::string dir = _settings.modelPath;
	dir = dir.substr(0, dir.find_last_of('/') + 1);

	aiColor3D color(0.0f, 0.0f, 0.0f);
	Float4 diffuse(1.0f, 1.0f, 1.0f, 1.0f);
	Float4 ambient(0.3f, 0.3f, 0.3f, 1.0f);

	// ���f���̊e�}�e���A�����쐬����
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		// �}�e���A���̃Z�b�g���閼�O�� "���f���̖��O ����"
		std::string materialName = _settings.modelName + std::to_string(i);

		// �}�e���A�������Ƀ��[�h����Ă���Ȃ�
		if (mC->GetIsImported(materialName))
		{
			// �擾���āA�z��ɓ����
			Material* loadMaterial =  mC->GetResource(materialName);
			materials.push_back(loadMaterial);
			continue;	// ���̃��[�v��
		}

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
		// �ۊǃN���X�̃C���X�^���X���擾
		TextureCollect* texCollect = TextureCollect::GetInstance();
		// �e�N�X�`���̖��O�̓}�e���A���Ɠ���
		std::string texName = materialName;
		aiString path;
		if (pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
		{
			// �t�@�C���`���`�F�b�N
			if (strstr(path.C_Str(), ".psd"))
			{
				MessageError("psd�ɂ͑Ή����Ă��܂���");
			}
			bool isSuccess = false;	// ���[�h�����������t���O

			// �e�N�X�`���̃C���X�^���X���쐬
			std::unique_ptr<Texture> texture = std::make_unique<Texture>();
			/*material->texture = new Texture();*/
			// ���f���Ɠ����K�w��T��
			std::string file = dir;
			file += path.C_Str();

			// �e�N�X�`���Ǘ��N���X�Ƀ��f���̃e�N�X�`����������(���O�̓��f���ƈꏏ)
			isSuccess = texCollect->Load(file.c_str(), texName);

			// �t�@�C�����݂̂ŒT��
			if(!isSuccess) {
				std::string file = path.C_Str();
				if (size_t idx = file.find_last_of('\\'); idx != std::string::npos)
				{
					file = file.substr(idx + 1);
					file = dir + file;
					isSuccess = texCollect->Load(file.c_str(), texName);
				}

				// �t�@�C���ڂ̃p�X��"\\�ł͂Ȃ�"/"�̏ꍇ�̑Ή�
				if (!isSuccess) {
					if (size_t idx = file.find_last_of('/'); idx != std::string::npos)
					{
						file = file.substr(idx + 1);
						file = dir + file;
						isSuccess = texCollect->Load(file.c_str(), texName);
					}
				}
			}
			// ���s
			if (!isSuccess) {
				std::string message = "���f���̃e�N�X�`���Ǎ����s�@" + materialName;
				MessageError(message.c_str());
				
				return false;
			}
		}
		
		// �ǂݍ��񂾃e�N�X�`�����}�e���A���ɐݒ肷��
		material->texture = texCollect->GetConstResource(texName);
		// �ۊǃN���X�ɓ����
		mC->SetResource(std::move(material), materialName);
		// �}�e���A�����󂯎��
		Material* mat = mC->GetResource(materialName);
		// �}�e���A���ǉ�
		materials.push_back(mat);
	}

	return true;
}

void Model::SetupTransform(const Transform& _transform) const
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

void Model::Draw(const Transform& _transform) const
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

