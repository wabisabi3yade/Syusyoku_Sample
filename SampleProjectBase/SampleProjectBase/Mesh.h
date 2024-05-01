#pragma once

struct aiMesh;

// ���b�V���N���X
// ���_�A�Ӂi�G�b�W�j�A�ʁi�|���S���j�̏W���̂���
class Mesh
{

	Vertex* pVertices{ nullptr };	// ���b�V���̒��_�f�[�^

	u_int* pIndicies{ nullptr };

	u_int vertexNum;	// ���_��

	u_int indexNum;	// ���_�C���f�b�N�X

	ID3D11Buffer* pVertexBuffer{ nullptr };	// ���_�o�b�t�@

	ID3D11Buffer* pIndexBuffer{ nullptr };	// �C���f�b�N�X�o�b�t�@


	// ���_�o�b�t�@�쐬
	bool CreateVertexBuffer(D3D11_Renderer& _renderer);
	// �C���f�b�N�X�o�b�t�@�i���_1��1�̂�ID�I�ȃ��m)���쐬
	bool CreateIndexBuffer(D3D11_Renderer& _renderer);
	// �폜�֐�
	void Release();

public:
	Mesh();
	~Mesh();

	// ���b�V���̓Ǎ�
	bool Setup(D3D11_Renderer& _renderer, aiMesh* pMeshData);
	// �`�揈��
	void Draw(D3D11_Renderer& _renderer);
};

