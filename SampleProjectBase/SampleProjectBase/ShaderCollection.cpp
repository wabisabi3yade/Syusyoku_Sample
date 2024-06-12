#include "ShaderCollection.h"

void ShaderCollection::Load()
{
	// csoファイルが入っているフォルダまでのパス名
	const std::string shaderPath = "assets/Shader/";

	// 頂点シェーダーを作成
	// csoファイルの名前
	std::vector<std::string> vFileNames
	{
		"VertexShader.cso",
	};
	// セットする名前
	std::vector<std::string> vShaderNames
	{
		"VertexShader",
	};
	for (int i = 0; i < vShaderNames.size(); i++)
	{
		std::unique_ptr<VertexShader> vShader = std::make_unique<VertexShader>();
		std::string loadFilePath = shaderPath + vFileNames[i];	// ロードする全体のパス名
		vShader->LoadCsoFile(loadFilePath.c_str());

		shaders.emplace(vShaderNames[i], std::move(vShader));	// 配列に追加する
	}

	// ピクセルシェーダーを作成
	// csoファイルの名前
	std::vector<std::string> pFileNames
	{
		"PixelShader.cso",
		"PS_VertexColor.cso"
	};
	// セットする名前
	std::vector<std::string> pShaderNames
	{
		"PixelShader",
		"PS_VertexColor"
	};
	for (int i = 0; i < pShaderNames.size(); i++)
	{
		std::unique_ptr<PixelShader> pShader = std::make_unique<PixelShader>();
		std::string loadFilePath = shaderPath + pFileNames[i];	// ロードする全体のパス名
		pShader->LoadCsoFile(loadFilePath.c_str());

		shaders.emplace(pShaderNames[i], std::move(pShader));	// 配列に追加する
	}
}

void ShaderCollection::Init()
{
	Load();	// csoファイルをロードする
}

void ShaderCollection::SetShader(std::string _shaderName, std::unique_ptr<Shader> _pSetShader)
{
	auto itr = shaders.find(_shaderName);
	if (itr != shaders.end())	// 同じ名前のシェーダーを見つけたら
	{
		ImGuiDebugLog::Add(_shaderName + " が既に頂点シェーダー配列にあります");
		return;
	}

	// 配列に追加
	shaders.emplace(_shaderName, std::move(_pSetShader));
}

void ShaderCollection::SetInputLayout(ID3D11InputLayout* _pSetInput)
{
	SAFE_RELEASE(pInputLayout);	// 先に解放する

	// アドレスを代入
	pInputLayout = _pSetInput;
}

VertexShader* ShaderCollection::GetVertexShader(std::string _shaderName)
{
	auto itr = shaders.find(_shaderName);
	if (itr == shaders.end())	// 同じ名前のシェーダーを見つけたら
	{
		ImGuiDebugLog::Add(_shaderName + " が見つかりませんでした");
		return nullptr;
	}

	VertexShader* retPtr = dynamic_cast<VertexShader*>(itr->second.get());
#ifdef _DEBUG
	if (retPtr == nullptr)
	{

		ImGuiDebugLog::Add(_shaderName + "指定したシェーダー名は頂点シェーダーではありません");
		return nullptr;
	}
#endif

	return retPtr;
}

PixelShader* ShaderCollection::GetPixelShader(std::string _shaderName)
{
	auto itr = shaders.find(_shaderName);
	if (itr == shaders.end())	// 同じ名前のシェーダーを見つけたら
	{
		ImGuiDebugLog::Add(_shaderName + " が見つかりませんでした");
		return nullptr;
	}

	PixelShader* retPtr = dynamic_cast<PixelShader*>(itr->second.get());
#ifdef _DEBUG
	if (retPtr == nullptr)
	{

		ImGuiDebugLog::Add(_shaderName + "指定したシェーダーはピクセルシェーダーではありません");
		return nullptr;
	}
#endif

	return retPtr;
}


