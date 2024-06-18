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

private:
	Type type;	// �V�F�[�_�[�̎��
	std::string name;	// ���O
protected:	
	// ���̃V�F�[�_�[�ŗ��p����GPU�ɑ���o�b�t�@�̃��X�g
	std::vector<ID3D11Buffer*> pBuffers;	
	std::vector<ID3D11ShaderResourceView*> pTextures;	// �V�F�[�_�[�֑���e�N�X�`�����
	
	/// <summary>
	//	�e�V�F�[�_�[�ł̍쐬����
	/// </summary>
	/// <param name="pData">�V�F�[�_�[�̃f�[�^</param>
	/// <param name="_dataSize">�f�[�^�̃T�C�Y</param>
	virtual void MakeShader(const char* _pData, u_int _dataSize) = 0;

	// �V�F�[�_�[����萔�o�b�t�@���쐬����
	virtual void MakeBuffer(const char* _pData, u_int _dataSize);

public:
	Shader(Type _type) : type(_type), name("") {}
	virtual ~Shader() {};
	
	// cso�t�@�C������V�F�[�_�[���쐬����
	void LoadCsoFile(const char* _filePath);	

	/// <summary>
	/// �o�b�t�@�ɏ�������
	/// </summary>
	/// <param name="_slot">�X���b�g�̔ԍ�</param>
	/// <param name="_pData">�o�b�t�@�ɓ������</param>
	void UpdateBuffer(u_int _slot, void* _pData);	

	/// <summary>
	/// �V�F�[�_�[�ɑ��邽�߂̔z��ɒǉ�
	/// </summary>
	/// <param name="_slot">�X���b�g�̔ԍ�</param>
	/// <param name="_texture"></param>
	void SetTexture(u_int _slot, Texture* _texture);

	void SetName(std::string _name);

	// GPU�ɏ������Ă���o�b�t�@�𑗂�
	virtual void Bind() = 0;
};

