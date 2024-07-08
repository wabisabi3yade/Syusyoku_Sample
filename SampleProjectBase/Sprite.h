#pragma once
#include "Texture.h"
#include "Mesh.h"

class Material;

// �l�p�`�|���S���Ƀe�N�X�`����\���ĕ\������N���X
class Sprite
{
	Texture* pTexture;	// �e�N�X�`��

	// �l�p�`
	std::unique_ptr<Mesh> pSquareMesh;

	void Copy(const Sprite& _other);

	// �e�N�X�`���p�̃��b�V���쐬
	void MakeSquare();
public:
	Sprite();
	Sprite(const Sprite& _other);
	~Sprite() {};

	void Draw();

	void SetTexture(Texture& _tex) { pTexture = &_tex; }

	Texture& GetTexture()const { return *pTexture; }
};