#pragma once
#include <vector>
#include "ModelSettings.h"

class Mesh;
class Material;

// 3Dモデルクラス(複数のメッシュで構成される表示物)
class Model
{
	std::vector<Mesh*> meshes;	// モデルに使われるメッシュ配列
	u_int meshNum;	// メッシュの数

	std::vector<Material*> materials;	// マテリアル

	bool Load(const ModelSettings& _settings, D3D11_Renderer& _renderer);
	// 座標変換行列を作成
	void SetupTransform();
public:
	Model(const ModelSettings& _settings);
	~Model();

	void Draw();
	void Release();

};

