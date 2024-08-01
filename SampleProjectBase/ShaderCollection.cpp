#include "ShaderCollection.h"

#include "ShaderDefine.h"

// csoファイルが入っているフォルダまでのパス名
constexpr const char* FILE_FOLDER = "assets/Shader/";

void ShaderCollection::LoadFromCSO()
{
	LoadVS();
	
	LoadPS();
}

void ShaderCollection::LoadVS()
{
	// 頂点シェーダーを作成
	// csoファイルの名前
	std::vector<std::string> vFileNames
	{
		"VS_Gouraud.cso",
		"VS_Primitive.cso",
		"VS_SkinAnimation.cso"
	};

	// セットする名前
	std::vector<std::string> vShaderNames
	{
		"VS_Gouraud",
		"VS_Primitive",
		"VS_SkinAnimation"
	};
	defaultVS = vShaderNames[0];

	// 頂点シェーダー読み込み配列
	std::vector<std::unique_ptr<VertexShader>> pVsShaderList;

	// グロー
	std::unique_ptr<VertexShader> pVShader = std::make_unique<VS_Gouraud>();
	pVsShaderList.push_back(std::move(pVShader));

	pVShader = std::make_unique<VS_Primitive>();
	pVsShaderList.push_back(std::move(pVShader));

	pVShader = std::make_unique<VS_SkinnedAnimation>();
	pVsShaderList.push_back(std::move(pVShader));

	for (int vsCount = 0; vsCount < static_cast<int>(pVsShaderList.size()); vsCount++)
	{
		// ロードする全体のパス名
		std::string loadFilePath = FILE_FOLDER + vFileNames[vsCount];
		pVsShaderList[vsCount]->LoadCsoFile(loadFilePath.c_str());
		pVsShaderList[vsCount]->SetName(vShaderNames[vsCount]);

		// 配列に追加する
		pShaders.emplace(vShaderNames[vsCount], std::move(pVsShaderList[vsCount]));
	}
}

void ShaderCollection::LoadPS()
{
	// ピクセルシェーダーを作成
	// csoファイルの名前
	std::vector<std::string> pFileNames
	{
		"PS_TexColor.cso",
		"PS_Unlit.cso",
		"PS_Primitive.cso"
	};
	// セットする名前
	std::vector<std::string> pShaderNames
	{
		"PS_TexColor",
		"PS_Unlit",
		"PS_Primitive"
	};
	defaultPS = pShaderNames[0];


	std::vector<std::unique_ptr<PixelShader>> pPsShaderList;

	// テクスチャ
	std::unique_ptr<PixelShader> pPShader = std::make_unique<PS_TexColor>();
	pPsShaderList.push_back(std::move(pPShader));

	// アンリット
	pPShader = std::make_unique<PS_Unlit>();
	pPsShaderList.push_back(std::move(pPShader));

	pPShader = std::make_unique<PixelShader>();
	pPsShaderList.push_back(std::move(pPShader));

	for (int psCount = 0; psCount < static_cast<int>(pPsShaderList.size()); psCount++)
	{
		// ロードする全体のパス名
		std::string loadFilePath = FILE_FOLDER + pFileNames[psCount];
		pPsShaderList[psCount]->LoadCsoFile(loadFilePath.c_str());
		pPsShaderList[psCount]->SetName((pShaderNames[psCount]));

		// 配列に追加する
		pShaders.emplace(pShaderNames[psCount], std::move(pPsShaderList[psCount]));
	}
}

void ShaderCollection::Init()
{
	LoadFromCSO();	// csoファイルをロードする
}

void ShaderCollection::UniqueUpdateBuffer()
{
	// オブジェクトごとに依存しないバッファ更新
	for (auto& shader : pShaders)
	{
		shader.second->OnceUpdateBuffer();
	}
}

VertexShader* ShaderCollection::GetVertexShader(const std::string& _shaderName)
{
	auto itr = pShaders.find(_shaderName);
	if (itr == pShaders.end())	// シェーダーがなければ
	{
		HASHI_DEBUG_LOG(_shaderName + " が見つかりませんでした");
		return nullptr;
	}

	VertexShader* retPtr = dynamic_cast<VertexShader*>(itr->second.get());
	if (retPtr == nullptr)
	{
		HASHI_DEBUG_LOG(_shaderName + "指定したシェーダー名は頂点シェーダーではありません");
		return nullptr;
	}

	return retPtr;
}

PixelShader* ShaderCollection::GetPixelShader(const std::string& _shaderName)
{
	auto itr = pShaders.find(_shaderName);
	if (itr == pShaders.end())	// 同じ名前のシェーダーを見つけたら
	{
		HASHI_DEBUG_LOG(_shaderName + " が見つかりませんでした");
		return nullptr;
	}

	PixelShader* retPtr = dynamic_cast<PixelShader*>(itr->second.get());
#ifdef EDIT
	if (retPtr == nullptr)
	{

		HASHI_DEBUG_LOG(_shaderName + "指定したシェーダーはピクセルシェーダーではありません");
		return nullptr;
	}
#endif

	return retPtr;
}


