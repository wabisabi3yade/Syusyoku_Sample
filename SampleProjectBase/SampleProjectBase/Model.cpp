#include "Model.h"
#include "Mesh.h"
#include "Direct3D11.h"

// assimp���C�u�����Ǎ�
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const ModelSettings& _settings)
{
	Load(_settings, *Direct3D11::GetInstance()->GetRenderer());
}

Model::~Model()
{
	Release();
}

bool Model::Load(const ModelSettings& _settings, D3D11_Renderer& _renderer)
{
	if (_settings.modelPath == nullptr)
	{
		MessageError("���f���̃t�@�C���p�X���ݒ肳��Ă��܂���");
		return false;
	}

	//���f���̃��[�h����
	Assimp::Importer importer;
	u_int flag = 0;
	flag |= aiProcess_Triangulate;	// �O�p�`�|���S��
	flag |= aiProcess_JoinIdenticalVertices;	// �������_����ɓ���
	/*flag |= aiProcess_FlipUVs;*/
	/*flag |= aiProcess_PreTransformVertices;*/

	if(_settings.isRighthand)
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

			pMeshes.push_back(mesh);
		}
	}

	return true;
}

void Model::SetupTransform()
{
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	CbTransformSet& cb = renderer.GetParameter().cbTransformSet;

	auto mtx = DirectX::XMMatrixIdentity();

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

void Model::Draw()
{
	SetupTransform();
	// �����_���[�擾
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();
	for (u_int meshIdx = 0; meshIdx < meshNum; meshIdx++)
	{
		pMeshes[meshIdx]->Draw(renderer);
	}
}

void Model::Release()
{
	for (auto mesh : pMeshes)
	{
		CLASS_DELETE(mesh);
	}
}
