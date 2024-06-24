#pragma once

struct aiMesh;

// ���b�V���N���X
// ���_�A�Ӂi�G�b�W�j�A�ʁi�|���S���j�̏W���̂���
class Mesh
{
private:

protected:
	Vertex* pVertices{ nullptr };	// ���b�V���̒��_�f�[�^

	u_int* pIndicies{ nullptr };	// ���_�C���f�b�N�X

	u_int vertexNum;	// ���_��

	u_int indexNum;	// ���_�C���f�b�N�X��

	u_int materialIndex;	// �}�e���A���̃C���f�b�N�X

	ID3D11Buffer* pVertexBuffer{ nullptr };	// ���_�o�b�t�@

	ID3D11Buffer* pIndexBuffer{ nullptr };	// �C���f�b�N�X�o�b�t�@

	DirectX::SimpleMath::Vector2 uvScroll;	// UV���W�̌��_(����)
	DirectX::SimpleMath::Vector2 uvSize;;	// UV���W�̃T�C�Y

	// ���_�o�b�t�@�쐬
	bool CreateVertexBuffer(D3D11_Renderer& _renderer);
	// �C���f�b�N�X�o�b�t�@�i���_1��1�̂�ID�I�ȃ��m)���쐬
	bool CreateIndexBuffer(D3D11_Renderer& _renderer);
	// �폜�֐�
	void Release();

public:
	Mesh();
	virtual ~Mesh();
	/// <summary>
	/// ���b�V����V�����m�ۂ��A�Z�b�g����
	/// </summary>
	/// <param name="_setMesh">�Z�b�g���郁�b�V��</param>
	/// <returns></returns>
	bool SetMesh(const Mesh& _setMesh);

	// ���b�V���̓Ǎ�
	bool Setup(D3D11_Renderer& _renderer, aiMesh* pMeshData, float _scaleBase);
	// �`�揈��
	void Draw(D3D11_Renderer& _renderer);
	// �}�e���A���̃C���f�b�N�X���擾
	const u_int GetMaterialIdx() { return materialIndex; }
	// �O�����璸�_�ƃC���f�b�N�X����
	void SetVertexData(Vertex* _pVerticies, u_int _vertexNum, u_int* _pIndecies, u_int _indexNum);
};

