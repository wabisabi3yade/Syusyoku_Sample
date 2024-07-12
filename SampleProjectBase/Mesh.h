#pragma once

// �o�b�t�@
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// ���b�V���N���X
class Mesh
{
protected:
	/// @brief ���_�f�[�^
	std::vector<Vertex> verticies;

	/// @brief ���_�o�b�t�@
	std::unique_ptr<VertexBuffer> pVertexBuffer;

	/// @brief �C���f�b�N�X
	std::vector<u_int> indicies;

	/// @brief �C���f�b�N�X�o�b�t�@
	std::unique_ptr<IndexBuffer> pIndexBuffer;

	/// @brief ���O
	std::string name;

	/// @brief �}�e���A���C���f�b�N�X
	u_int materialID;

	/// @brief �g�|���W�[
	D3D11_PRIMITIVE_TOPOLOGY topology;

public:
	Mesh();
	Mesh(const Mesh& _other);
	virtual ~Mesh() {}

	Mesh& operator=(const Mesh& _other);

	/// @brief �o�b�t�@�̏�����
	void InitBuffer();

	/// @brief ���_�f�[�^���擾
	/// @return ���_�f�[�^
	std::vector<Vertex>& GetVerticies(); 

	/// @brief ���_�o�b�t�@���擾
	/// @return ���_�o�b�t�@
	const ID3D11Buffer& GetVertexBuffer() const;

	/// @brief �C���f�b�N�X�o�b�t�@���擾
	/// @return �C���f�b�N�X�o�b�t�@
	const ID3D11Buffer& GetIndexBuffer() const;

	/// @brief �C���f�b�N�X���擾
	/// @return �C���f�b�N�X�f�[�^
	std::vector<u_int>& GetIndicies();

	/// @brief �C���f�b�N�X�����擾
	/// @return �C���f�b�N�X��
	u_int GetIndexNum() const;

	/// @brief ���O��ݒ肷��
	/// @param _name ���O
	void SetName(const std::string& _name);

	/// @brief �}�e���A���̃C���f�b�N�X���Z�b�g
	/// @param _materialID �}�e���A���C���f�b�N�X
	void SetMaterialID(u_int _materialID);

private:
	/// @brief �R�s�[
	/// @param _other�@�Ώ� 
	void Copy(const Mesh& _other);
};

