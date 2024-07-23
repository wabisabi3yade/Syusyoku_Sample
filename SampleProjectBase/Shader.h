#pragma once

class Texture;

// �V�F�[�_�[�̊��N���X
class Shader
{
public:
	// �V�F�[�_�[�̎��
	enum class Type
	{
		Vertex,	// ���_
		Pixel,	// �s�N�Z��
		Num
	};

	// �o�b�t�@�̎��
	enum class BufferType
	{
		WVP,	// WVP
		Material,	// �}�e���A��
		Light,	// ���C�g
		CameraPos,	// �J�����̍��W
		Geometory_Color	// �W�I���g���g�p���̐F
	};

private:

	/// @brief �V�F�[�_�[�̎��
	Type type;

	/// @brief ���O
	std::string name;
protected:

	/// @brief ���̃V�F�[�_�[�ŗ��p����GPU�ɑ���o�b�t�@�̔z��
	std::vector<ID3D11Buffer*> pBuffers;

	/// @brief �V�F�[�_�ɑ���e�N�X�`��
	std::vector<ID3D11ShaderResourceView*> pTextures;

	/// @brief �o�b�t�@���
	std::vector<BufferType> bufferTypes;

	/// @brief �e�V�F�[�_�[�ł̍쐬����
	/// @param _pData �V�F�[�_�[�̃f�[�^
	/// @param _dataSize �f�[�^�̃T�C�Y
	virtual void MakeShader(const char* _pData, u_int _dataSize) = 0;

	/// @brief �V�F�[�_�[����萔�o�b�t�@���쐬����
	/// @param _pData �V�F�[�_�̕�����
	/// @param _dataSize �T�C�Y
	virtual void MakeBuffer(const char* _pData, u_int _dataSize);

public:
	Shader(Type _type) : type(_type), name("") {}
	virtual ~Shader() {};

	/// @brief cso����V�F�[�_���쐬
	/// @param _filePath cso�̃p�X
	void LoadCsoFile(const char* _filePath);

	/// @brief �o�b�t�@�ɏ�������
	/// @param _slot �X���b�g�̔ԍ�
	/// @param _pData �o�b�t�@�ɓ������
	void UpdateBuffer(u_int _slot, void* _pData);

	/// @brief 1���[�v�Ɉ�񂵂��X�V���Ȃ��Ă��悢�o�b�t�@�͂����ōX�V����
	virtual void OnceUpdateBuffer() {}

	/// @brief GPU�ɏ������Ă���o�b�t�@�𑗂�
	virtual void SetGPU() = 0;

	/// @brief �V�F�[�_�[�ɑ��邽�߂̔z��ɒǉ�
	/// @param _slot �X���b�g�̔ԍ�
	/// @param _pTexture �e�N�X�`��
	void SetTexture(u_int _slot, Texture* _pTexture);

	/// @brief �V�F�[�_�ɖ��O������
	/// @param _name ���O
	void SetName(const std::string& _name);

	/// @brief �o�b�t�@�̎��
	/// @param _slot �X���b�g�ԍ�
	/// @return �o�b�t�@�̎��
	BufferType GetBufferType(u_int _slot);

	/// @brief �o�b�t�@�̐����擾
	/// @return �o�b�t�@�̐�
	u_int GetBufferNum();
};
