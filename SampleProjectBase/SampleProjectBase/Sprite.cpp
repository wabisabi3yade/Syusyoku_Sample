#include "pch.h"
#include "Sprite.h"

#include "Material.h"

using namespace DirectX::SimpleMath;

Sprite::Sprite() : pTexture(nullptr)
{
	MakeSquare();
}

void Sprite::MakeSquare()
{
	// �e�N�X�`�����͂�|���S���𐶐�����(TRIANGLESTRIP)
	// ���_���W
	constexpr int vertexNum = 4;
	Vertex* pVerticies = new Vertex[vertexNum];
	pVerticies[0].position = Vector3(-0.5f, -0.5f, 0);	// ����
	pVerticies[1].position = Vector3(0.5f, -0.5f, 0);	// �E��
	pVerticies[2].position = Vector3(0.5f, 0.5f, 0);	// �E��
	pVerticies[3].position = Vector3(-0.5f, 0.5f, 0);	// ����

	// uv���W
	pVerticies[0].uv = { 0.0f, 0.0f };	
	pVerticies[1].uv = { 1.0, 0.0f };	
	pVerticies[2].uv = { 1.0f, 1.0f };	
	pVerticies[3].uv = { 0.0f, 1.0f };	

	// �C���f�b�N�X
	constexpr int indexNum = 6;
	u_int* pIndicies = new u_int[indexNum];
	pIndicies[0] = 0;
	pIndicies[1] = 1;
	pIndicies[2] = 2;
	pIndicies[3] = 0;
	pIndicies[4] = 2;
	pIndicies[5] = 3;

	// �F�A���_
	for (int i = 0; i < vertexNum; i++)
	{
		pVerticies[i].color = Vector4::One;	// �F
		pVerticies[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	// �l�p�|���S��
	pSquareMesh = std::make_unique<Mesh>();	// ���b�V���쐬
	pSquareMesh->SetVertexData(pVerticies, vertexNum, pIndicies, indexNum);

}


void Sprite::Copy(const Sprite& _other)
{
	// �A�h���X��n��
	pTexture = _other.pTexture;

	// ���b�V��������ăp�����[�^���R�s�[
	pSquareMesh = std::make_unique<Mesh>();
}


void Sprite::Draw()
{
	pSquareMesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

Sprite::Sprite(const Sprite& _other)
{
	if (this == &_other) return;

	Copy(_other);
}
