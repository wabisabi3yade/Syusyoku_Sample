#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

struct SceneLightsParam;

// シェーダーリソースを保有するクラス
class ShaderCollection : public Singleton_Base<ShaderCollection>
{
	friend class Singleton_Base<ShaderCollection>;

	// シェーダー配列
	std::unordered_map<std::string, std::unique_ptr<Shader>> pShaders;

	ShaderCollection() : defaultVS(""), defaultPS("") {};
	~ShaderCollection(){};

public:
	// デフォルトのシェーダーの名前
	std::string defaultVS;	// 頂点シェーダー
	std::string defaultPS;	// ピクセルシェーダー

	void Init();

	/// @brief オブジェクトごとで変わらないバッファ情報を更新する(ライト、カメラなど)
	void UniqueUpdateBuffer();

	// 取得する
	VertexShader* GetVertexShader(const std::string& _shaderName);
	PixelShader* GetPixelShader(const std::string& _shaderName);

private:
	
	/// @brief CSOファイルから読み込む
	void LoadFromCSO();	

	/// @brief 頂点シェーダー
	void LoadVS();

	/// @brief ピクセルシェーダー
	void LoadPS();

};

