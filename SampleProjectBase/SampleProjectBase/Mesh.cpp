#include "pch.h"
#include "Mesh.h"
#include "D3D11_Renderer.h"

// assimp���C�u�����Ǎ�
#ifdef _DEBUG
#pragma comment(lib, "assimp-vc142-mtd.lib")
#else
#pragma comment(lib, "assimp-vc142-mt.lib")
#endif
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


bool Mesh::CreateVertexBuffer(D3D11_Renderer& _renderer)
{
	// �o�b�t�@��`�̍\���̂�ݒ�
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertexNum;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// �T�u���\�[�X�ɒ��_�f�[�^����
	D3D11_SUBRESOURCE_DATA vertexSubData;
	vertexSubData.pSysMem = pVertices;

	// �o�b�t�@���쐬����
	auto hr = _renderer.GetDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexSubData,
		&pVertexBuffer
	);

	if (FAILED(hr))
		return false;

	return true;
}

bool Mesh::CreateIndexBuffer(D3D11_Renderer& _renderer)
{
	// �C���f�b�N�X�f�[�^�̃o�b�t�@
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = indexNum * 4;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubData;
	indexSubData.pSysMem = pIndicies;

	auto hr = _renderer.GetDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexSubData,
		&pIndexBuffer
	);

	if (FAILED(hr))
		return false;


	return true;
}

Mesh::Mesh() : vertexNum(0), indexNum(0), materialIndex(0)
{
}

Mesh::~Mesh()
{
	Release();
}

bool Mesh::SetMesh(const Mesh& _setMesh)
{
	return false;
}

bool Mesh::Setup(D3D11_Renderer& _renderer, aiMesh* pMeshData, float _scaleBase)
{
	// ���_�f�[�^���擾 //

	vertexNum = pMeshData->mNumVertices;
	// ���_ �쐬
	pVertices = new Vertex[vertexNum];

	aiVector3D zero(0.f, 0.f, 0.f);

	for (u_int vertexIdx = 0; vertexIdx < vertexNum; vertexIdx++)
	{
		// ���_�̍��W���擾���āA���_�\���֑̂������
		auto& pos = pMeshData->mVertices[vertexIdx];
		pVertices[vertexIdx].position = { pos.x, pos.y, pos.z };
		// �C���|�[�g���̑傫���{�����|���Ă�����
		pVertices[vertexIdx].position *= _scaleBase;

		// �@���׃N�g�����擾
		auto& normal = pMeshData->HasNormals() ? pMeshData->mNormals[vertexIdx] : zero;
		pVertices[vertexIdx].normal = { normal.x, normal.y, normal.z };

		// UV���W���擾
		auto& uv = pMeshData->HasTextureCoords(0) ? pMeshData->mTextureCoords[0][vertexIdx] 
			: zero;
		pVertices[vertexIdx].uv = { uv.x, uv.y };

		constexpr float COLOR = 1.0f;
		pVertices[vertexIdx].color = { COLOR, COLOR, COLOR, 1.0f };
	}

	// ���_�V�F�[�_�[�쐬
	if (CreateVertexBuffer(_renderer) == false)
	{
		return false;
	}

	// ���_�C���f�b�N�X�f�[�^���擾 // 
	indexNum = pMeshData->mNumFaces * 3;	// 1�|���S����3�̃C���f�b�N�X
	pIndicies = new u_int[indexNum];

	// �C���f�b�N�X������U��
	for (u_int faceIdx = 0; faceIdx < pMeshData->mNumFaces; faceIdx++)
	{
		auto& face = pMeshData->mFaces[faceIdx];
		assert(face.mNumIndices == 3);	// �ʂ̃C���f�b�N�X����3�ł��邱�Ƃ��m�F

		for (u_int idx = 0; idx < 3; idx++)
		{
			// ���̖ʂ̒��_�C���f�b�N�X���擾����(0,1,2,0,1,2�E�E�E)
			pIndicies[faceIdx * 3 + idx] = face.mIndices[idx];
		}

	}

	if (CreateIndexBuffer(_renderer) == false)
		return false;

	// �}�e���A���̊��蓖��
	materialIndex = pMeshData->mMaterialIndex;

	return true;
}

void Mesh::Draw(D3D11_Renderer& _renderer)
{
	auto pDeviceContext = _renderer.GetDeviceContext();
	u_int strides[1] = { sizeof(Vertex) };	// ���_�T�C�Y
	u_int offset[1] = { 0 };

	// �`��Ɏg�����f���E�C���f�b�N�X�o�b�t�@���w�肷��
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, strides, offset);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �C���f�b�N�X�o�b�t�@�𗘗p���ĕ`�悷��
	// ��1�����F�`�悷�钸�_��
	pDeviceContext->DrawIndexed(indexNum, 0, 0);
}

void Mesh::SetVertexData(Vertex* _pVerticies, u_int _vertexNum, u_int* _pIndecies, u_int _indexNum)
{
	Release();	// �O�c���Ă���o�b�t�@�����������

	// ���_
	pVertices = _pVerticies;
	vertexNum = _vertexNum;

	// �C���f�b�N�X
	pIndicies = _pIndecies;
	indexNum = _indexNum;

	// ���_�o�b�t�@�E�C���f�b�N�X�o�b�t�@���쐬
	D3D11_Renderer* renderer =  Direct3D11::GetInstance()->GetRenderer();
	CreateVertexBuffer(*renderer);
	CreateIndexBuffer(*renderer);
}

void Mesh::Release()
{
	SAFE_RELEASE(pVertexBuffer);
	SAFE_RELEASE(pIndexBuffer);

	CLASS_DELETE(pVertices);
	CLASS_DELETE(pIndicies);
}
