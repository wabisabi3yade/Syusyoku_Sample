#pragma once
#include "GameObject.h"
#include "Texture.h"
#include "Mesh.h"

// 2Dテクスチャを持ったオブジェクト
class Object_2D : public GameObject
{
	std::unique_ptr<Mesh> pSquareMesh;	// テクスチャを貼るメッシュ
	MaterialClass* pMaterial;	// 使用するマテリアル
	Texture* pTexture;	// 使用するテクスチャ

	void MaterialSetting();	// マテリアルの設定
	void MakeVerticies();	// 頂点の位置を変える
public:
	Object_2D();
	~Object_2D();

	void Draw();
	void DrawSetup(D3D11_Renderer& _renderer);	// 描画準備

	// テクスチャを設定する
	void SetTexture(Texture* _texture) { pTexture = _texture; }
	
	//// サイズを設定
	//void SetSizeWidth(float _width);
	//void SetSizeHeight(float _height);
	//void SetSize(const DirectX::SimpleMath::Vector2& _size);
};

