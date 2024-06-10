#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

// シェーダーリソースを保有するクラス
class ShaderCollection : public Singleton_Base<ShaderCollection>
{
	friend class Singleton_Base<ShaderCollection>;

	// 頂点シェーダーリスト
	std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
	// 入力レイアウトオブジェクト（VRAMに情報を入力するオブジェクト）
	ID3D11InputLayout* pInputLayout = nullptr;

	ShaderCollection(){};
	~ShaderCollection(){};
	void Load();	// csoファイルをロードする
public:
	void Init();

	void SetShader(std::string _shaderName, std::unique_ptr<Shader> _pSetShader);

	void SetInputLayout(ID3D11InputLayout* _pSetInput);

	// 取得する
	VertexShader* GetVertexShader(std::string _shaderName);
	PixelShader* GetPixelShader(std::string _shaderName);
	ID3D11InputLayout* GetInputLayout() { return pInputLayout; };
};

