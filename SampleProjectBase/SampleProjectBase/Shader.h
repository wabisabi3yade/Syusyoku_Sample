#pragma once

// �V�F�[�_�[�̊��N���X
class Shader
{
protected:
	virtual void MakeBuffer() = 0;	// �o�b�t�@���쐬����

public:
	Shader();
	virtual ~Shader();

	void WriteBuffer();	// �o�b�t�@�ɏ�����������
	virtual void Bind();	// GPU�ɑ���
};

