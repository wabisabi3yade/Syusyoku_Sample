#pragma once
#include "ModelSettings.h"
#include "Model.h"

class Mesh;
class Material;

// 3Dモデルクラス(複数のメッシュで構成される表示物)
class Model
{
	// モデルの名前
	std::string modelName;
	// モデルを解放させないようにする
	bool isPermanent;
	// モデル情報があるかフラグ
	bool isImported;

	// モデルの設定情報
	ModelSettings modelData;

	std::vector<Mesh*> meshes;	// モデルに使われるメッシュ配列
	u_int meshNum;	// メッシュの数

	std::vector<Material*> materials;	// マテリアル

	bool Load(const ModelSettings& _settings, D3D11_Renderer& _renderer);
	// 座標変換行列を作成
	void SetupTransform(const Transform& _transform) const;
public:
	Model();
	~Model();

	// モデルの描画をする
	void Draw(const Transform& _transform) const;
	void Release();

	// 外部から情報渡されてモデルをロードする処理
	bool SetSetting(const ModelSettings& _settings);

	// モデルの情報があるかフラグを取得
	bool GetImported()const { return isImported; }
};

