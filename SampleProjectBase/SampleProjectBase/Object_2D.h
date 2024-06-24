#pragma once
#include "GameObject.h"
#include "Texture.h"
#include "Mesh.h"

// 2D�e�N�X�`�����������I�u�W�F�N�g
class Object_2D : public GameObject
{
	std::unique_ptr<Mesh> pSquareMesh;	// �e�N�X�`����\�郁�b�V��
	MaterialClass* pMaterial;	// �g�p����}�e���A��
	Texture* pTexture;	// �g�p����e�N�X�`��

	void MaterialSetting();	// �}�e���A���̐ݒ�
	void MakeVerticies();	// ���_�̈ʒu��ς���
public:
	Object_2D();
	~Object_2D();

	void Draw();
	void DrawSetup(D3D11_Renderer& _renderer);	// �`�揀��

	// �e�N�X�`����ݒ肷��
	void SetTexture(Texture* _texture) { pTexture = _texture; }
	
	//// �T�C�Y��ݒ�
	//void SetSizeWidth(float _width);
	//void SetSizeHeight(float _height);
	//void SetSize(const DirectX::SimpleMath::Vector2& _size);
};

