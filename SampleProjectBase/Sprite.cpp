#include "pch.h"
#include "Sprite.h"

#include "Material.h"

constexpr u_int VERTEX_NUM(4);
constexpr u_int INDEX_NUM(6);

using namespace DirectX::SimpleMath;

Sprite::Sprite() : pTexture(nullptr)
{
	MakeSquare();
}

void Sprite::MakeSquare()
{
	// �l�p�|���S���쐬
	pSquareMesh = std::make_unique<Mesh>();	

	// ���_�쐬
	std::vector<Vertex>& verticies = pSquareMesh->GetVerticies();
	verticies.resize(VERTEX_NUM);

	// ���W
	verticies[0].position = Vector3(-0.5f, -0.5f, 0);	// ����
	verticies[1].position = Vector3(0.5f, -0.5f, 0);	// �E��
	verticies[2].position = Vector3(0.5f, 0.5f, 0);	// �E��
	verticies[3].position = Vector3(-0.5f, 0.5f, 0);	// ����

	// uv���W
	verticies[0].uv = { 0.0f, 0.0f };	
	verticies[1].uv = { 1.0, 0.0f };	
	verticies[2].uv = { 1.0f, 1.0f };	
	verticies[3].uv = { 0.0f, 1.0f };	

	// �F�A���_
	for (int i = 0; i < VERTEX_NUM; i++)
	{
		verticies[i].color = Vector4::One;	// �F
		verticies[i].normal = Vector3(0.0f, 0.0f, -1.0f);
	}

	// �C���f�b�N�X
	std::vector<u_int>& indicies = pSquareMesh->GetIndicies();
	indicies.resize(INDEX_NUM);

	indicies[0] = 0;
	indicies[1] = 1;
	indicies[2] = 2;
	indicies[3] = 0;
	indicies[4] = 2;
	indicies[5] = 3;

	// �o�b�t�@�쐬
	pSquareMesh->InitBuffer();
}


void Sprite::Copy(const Sprite& _other)
{
	// �A�h���X��n��
	pTexture = _other.pTexture;

	// ���b�V��������ăp�����[�^���R�s�[
	Mesh* pMesh = new Mesh(*_other.pSquareMesh);
	pSquareMesh.reset(pMesh);
}

Sprite::Sprite(const Sprite& _other)
{
	if (this == &_other) return;
	Copy(_other);
}

Sprite& Sprite::operator=(const Sprite& _other)
{
	if (this == &_other) return *this;
	Copy(_other);
	return *this;
}
