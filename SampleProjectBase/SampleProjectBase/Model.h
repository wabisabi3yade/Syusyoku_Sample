#pragma once
#include "ModelSettings.h"
#include "Model.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "MaterialClass.h"

class Mesh;

// 3Dモデルクラス(複数のメッシュで構成される表示物)
class Model
{
	// モデルの設定情報
	ModelSettings modelData;

	std::vector<Mesh*> meshes;	// モデルに使われるメッシュ配列
	u_int meshNum;	// メッシュの数

	std::vector<Material*> pMaterials;	// マテリアル
	std::vector<Texture*> pTextures;	//	テクスチャ

	bool LoadProcess(const ModelSettings& _settings, D3D11_Renderer& _renderer);

	void ResetParam();	// メッシュなどの情報を全て初期化する
public:
	Model();
	~Model();

	// モデルの情報を自身のモデルに設定する(メッシュなど)
	bool SetModel(const Model& _setModel);

	void Draw(const Transform& _transform) const;	// モデルの描画をする
	void Release();	// モデルのインスタンス解放処理

	bool Load(const ModelSettings& _settings);	// 外部から情報渡されてモデルをロードする処理

	u_int GetMeshNum()const { return meshNum; }	// モデルのメッシュ数を取得
	const std::vector<Mesh*> GetMeshes()const { return meshes; }	// メッシュの配列を取得する
	const ModelSettings GetModelData()const { return modelData; }	// モデルのデータを取得する

	// マテリアル全体のシェーダーを変更する
	void SetVertexShader(Shader* _vertexSh);	
	void SetPixelShader(Shader* _pixelSh);

	void SetTexture(std::vector<Texture*> _setTextures);	// モデルのテクスチャをセットする

	void ImGuiSetting();
};

