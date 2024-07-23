#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

struct SceneLightsParam;

// シェーダーリソースを保有するクラス
class ShaderCollection : public Singleton_Base<ShaderCollection>
{
	friend class Singleton_Base<ShaderCollection>;

	// 頂点シェーダーリスト
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;

	ShaderCollection() : defaultVS(""), defaultPS("") {};
	~ShaderCollection(){};
	void Load();	// csoファイルをロードする
public:
	// デフォルトのシェーダーの名前
	std::string defaultVS;	// 頂点シェーダー
	std::string defaultPS;	// ピクセルシェーダー

	void Init();

	/// @brief オブジェクトごとに変わらないバッファは更新する(ライト、カメラなど)
	void UniqueBufferUpdate();

	// 取得する
	VertexShader* GetVertexShader(const std::string& _shaderName);
	PixelShader* GetPixelShader(const std::string& _shaderName);
};

