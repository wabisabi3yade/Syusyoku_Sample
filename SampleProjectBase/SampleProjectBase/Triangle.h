#pragma once

// 3�_�̒��_����Ȃ�1�̃|���S���̃N���X
class Triangle
{
	static constexpr int VERTEX_NUM = 3;	// 1�|���S���̒��_��
	Vertex vertices[VERTEX_NUM];	// ���_

	// Direct3D�̃p�C�v���C���ɗ������_�o�b�t�@
	ID3D11Buffer* pVertexBuffer = nullptr;	

	// �|���S���̏����V�F�[�_�[�ɓn��
	void SetupTransform(D3D11_Renderer& _renderer);

public:
	Triangle();
	~Triangle();

	// ���_�o�b�t�@���쐬����
	bool CreateVertexBuffer(D3D11_Renderer& _renderer);

	// �|���S����`�悷��
	void Draw(D3D11_Renderer& _renderer);
};

