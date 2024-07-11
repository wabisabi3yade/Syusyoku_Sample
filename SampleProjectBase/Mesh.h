#pragma once

// ���b�V���N���X
class Mesh
{
protected:
	/// @brief ���_�f�[�^
	std::vector<Vertex> verticies;

	/// @brief �C���f�b�N�X
	std::vector<u_int> indicies;

	/// @brief ���O
	std::string name;

	/// @brief �}�e���A���C���f�b�N�X
	u_int materialID;

public:
	Mesh() : name(""), materialID(0) {}
	Mesh(const Mesh& _other);
	virtual ~Mesh() {}

	Mesh& operator=(const Mesh& _other);

	/// @brief ���_�f�[�^���擾
	/// @return ���_�f�[�^
	std::vector<Vertex>& GetVerticies();

	/// @brief �C���f�b�N�X���擾
	/// @return �C���f�b�N�X�f�[�^
	std::vector<u_int>& GetIndicies();

	/// @brief �C���f�b�N�X�����擾
	/// @return �C���f�b�N�X��
	u_int GetIndexNum();

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

