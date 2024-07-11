#pragma once
#include "Texture.h"
#include "Mesh.h"

class Material;

// 四角形ポリゴンにテクスチャを貼って表示するクラス
class Sprite
{
	/// @brief テクスチャ
	Texture* pTexture;	

	/// @brief 四角形ポリゴン
	std::unique_ptr<Mesh> pSquareMesh;

	/// @brief コピー関数
	/// @param _other コピーされる対象
	void Copy(const Sprite& _other);

	/// @brief 四角ポリゴンを作成
	void MakeSquare();
public:
	Sprite();
	Sprite(const Sprite& _other);
	~Sprite() {};

	Sprite& operator=(const Sprite& _other);

	void SetTexture(Texture& _tex) { pTexture = &_tex; }

	Texture& GetTexture()const { return *pTexture; }
};