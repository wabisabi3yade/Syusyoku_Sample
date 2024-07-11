#pragma once
#include "Texture.h"
#include "Mesh.h"

class Material;

// �l�p�`�|���S���Ƀe�N�X�`����\���ĕ\������N���X
class Sprite
{
	/// @brief �e�N�X�`��
	Texture* pTexture;	

	/// @brief �l�p�`�|���S��
	std::unique_ptr<Mesh> pSquareMesh;

	/// @brief �R�s�[�֐�
	/// @param _other �R�s�[�����Ώ�
	void Copy(const Sprite& _other);

	/// @brief �l�p�|���S�����쐬
	void MakeSquare();
public:
	Sprite();
	Sprite(const Sprite& _other);
	~Sprite() {};

	Sprite& operator=(const Sprite& _other);

	void SetTexture(Texture& _tex) { pTexture = &_tex; }

	Texture& GetTexture()const { return *pTexture; }
};