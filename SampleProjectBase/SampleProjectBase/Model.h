#pragma once
#include "ModelSettings.h"
#include "Model.h"
#include "VertexShader.h"
#include "PixelShader.h"

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

	// このモデルで使うシェーダー
	VertexShader* pVertexShader;
	PixelShader* pPixelShader;

	bool LoadProcess(const ModelSettings& _settings, D3D11_Renderer& _renderer);

	void SetupDraw(const Transform& _transform) const;	// 座標変換行列を作成

	void ResetParam();	// メッシュなどの情報を全て初期化する
public:
	Model();
	~Model();

	// モデルの情報を自身のモデルに設定する(メッシュなど)
	bool SetModel(const Model& _setModel);

	void Draw(const Transform& _transform) const;	// モデルの描画をする
	void Release();	// モデルのインスタンス解放処理

	bool Load(const ModelSettings& _settings);	// 外部から情報渡されてモデルをロードする処理

	bool GetImported()const { return isImported; }	// モデルの情報があるかフラグを取得
	u_int GetMeshNum()const { return meshNum; }	// モデルのメッシュ数を取得
	const std::vector<Mesh*> GetMeshes()const { return meshes; }	// メッシュの配列を取得する
	const ModelSettings GetModelData()const { return modelData; }	// モデルのデータを取得する

	void SetVertexShader(Shader* _vertexSh);
	void SetPixelShader(Shader* _pixelSh);
};

