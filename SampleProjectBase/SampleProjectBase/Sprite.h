#pragma once
#include "Texture.h"
#include "Mesh.h"

class Material;

// 四角形ポリゴンにテクスチャを貼って表示するクラス
class Sprite
{
	Texture* pTexture;	// テクスチャ

	// 四角形
	std::unique_ptr<Mesh> pSquareMesh;

	void Copy(const Sprite& _other);

	// テクスチャ用のメッシュ作成
	void MakeSquare();
public:
	Sprite();
	Sprite(const Sprite& _other);
	~Sprite() {};

	void Draw();

	void SetTexture(Texture& _tex) { pTexture = &_tex; }

	Texture& GetTexture()const { return *pTexture; }
};