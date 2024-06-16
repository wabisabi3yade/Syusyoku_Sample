#pragma once
#include "GameObject.h"
#include "Texture.h"
#include "Mesh.h"
#include "Material.h"

// 空間上でなく画面におく2Dオブジェクト(UI)
class Object_UI :
    public GameObject
{
    Texture* pTexture;  // テクスチャ
    std::unique_ptr<Mesh> poligon;  // ポリゴン

    static Material* pMaterial; // 共通のマテリアル

    void MakePoligon(); // ポリゴン作成
    void MakeMaterial();    // 共通のマテリアルを作成する

    void SetupDraw();
public:
    Object_UI();
    ~Object_UI();

    // 一連処理
    void Update() override;
    void LateUpdate() override;
    void Draw() override;

    void SetTexture(Texture* _texture) { pTexture = _texture; } // テクスチャ代入
};

