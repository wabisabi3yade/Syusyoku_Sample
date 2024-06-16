#pragma once
#include "GameObject.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

// ��ԏ�łȂ���ʂɂ���2D�I�u�W�F�N�g(UI)
class Object_UI :
    public GameObject
{
    Texture* pTexture;  // �e�N�X�`��
    std::unique_ptr<Mesh> poligon;  // �|���S��

    static Material* pMaterial; // ���ʂ̃}�e���A��

    void MakePoligon(); // �|���S���쐬
    void MakeMaterial();    // ���ʂ̃}�e���A�����쐬����

    void SetupDraw();
public:
    Object_UI();
    ~Object_UI();

    // ��A����
    void Update() override;
    void LateUpdate() override;
    void Draw() override;

    void SetTexture(Texture* _texture) { pTexture = _texture; } // �e�N�X�`�����
};

