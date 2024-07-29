#pragma once

#include "Vertex.h"

// �o�b�t�@
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// 1�̃��b�V���N���X
class SingleMesh
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

	/// @brief ���S���W
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief ���b�V���̃T�C�Y
	DirectX::SimpleMath::Vector3 size;

	// �����ݒ�
	void InitSetting(const std::string& _name, D3D11_PRIMITIVE_TOPOLOGY _topology);

public:
	SingleMesh();
	SingleMesh(const SingleMesh& _other);
	virtual ~SingleMesh() {}

	SingleMesh& operator=(const SingleMesh& _other);

	/// @brief �o�b�t�@�̏�����
	void InitBuffer();

	/// @brief ���_�f�[�^���擾
	/// @return ���_�f�[�^
	std::vector<Vertex>& GetVerticies();

	/// @brief ���_�o�b�t�@���擾
	/// @return ���_�o�b�t�@
	const VertexBuffer& GetVertexBuffer() const;

	/// @brief �C���f�b�N�X�o�b�t�@���擾
	/// @return �C���f�b�N�X�o�b�t�@
	const IndexBuffer& GetIndexBuffer() const;

	/// @brief �C���f�b�N�X���擾
	/// @return �C���f�b�N�X�f�[�^
	std::vector<u_int>& GetIndicies();

	/// @brief ���_�����擾
	/// @return ���_��
	u_int GetVertexNum() const;

	/// @brief �C���f�b�N�X�����擾
	/// @return �C���f�b�N�X��
	u_int GetIndexNum() const;

	/// @brief �}�e���A���C���f�b�N�X���擾
	/// @return �}�e���A���C���f�b�N�X
	u_int GetMaterialID() const;

	/// @brief ���S���W���擾����
	/// @return ���S���W
	DirectX::SimpleMath::Vector3 GetCenterPos();

	/// @brief �T�C�Y���擾����
	/// @return ���b�V���̑傫��
	DirectX::SimpleMath::Vector3 GetSize();

	/// @brief �g�|���W�[���擾
	/// @return �g�|���W�[
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() const;

	/// @brief ���O��ݒ肷��
	/// @param _name ���O
	void SetName(const std::string& _name);

	/// @brief �}�e���A���̃C���f�b�N�X���Z�b�g
	/// @param _materialID �}�e���A���C���f�b�N�X
	void SetMaterialID(u_int _materialID);

	/// @brief �g�|���W�[���Z�b�g
	/// @param _topology �g�|���W�[
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _topology);

	/// @brief ���S���W���Z�b�g
	/// @param _centerPos ���S���W
	void SetCenterPosition(const DirectX::SimpleMath::Vector3& _centerPos);

	/// @brief �T�C�Y���Z�b�g
	/// @param _size ���f���̃T�C�Y
	void SetSize(const DirectX::SimpleMath::Vector3& _size);

protected:
	/// @brief �R�s�[
	/// @param _other�@�Ώ�
	void Copy(const SingleMesh& _other);
};
