#include "pch.h"
#include "ModelLoader.h"

#include <filesystem>

#define MODEL_NC_NUM (3)	// モデルの命名規則で使われる文字数
#define TEX_NC_NUM (2)	// テクスチャの命名規則で使われる文字数

namespace fs = std::filesystem;

Model* ModelLoader::Load(const ModelSettings& _modelData)
{
	// リソース管理に既にロードされているか確認する
	// モデル名にM_がついていなければ
	std::string setName = _modelData.modelName;
	
	// 名前の頭文字3文字が Md_ でないなら
	if (setName.size() >= MODEL_NC_NUM && setName.substr(0, MODEL_NC_NUM) != "Md_")
	{
		setName = "Md_" + setName;	// 名前の最初に命名規則をつける
	}

	// リソース管理に既にあるか確認する
	ResourceCollection* pReCollecter = ResourceCollection::GetInstance();
	if (pReCollecter->GetImpotred(setName))
	{
		// 既にロードされたモデルを返す
		return pReCollecter->GetResource<Model>(setName);	
	}

	// モデルをロードする
	std::unique_ptr<Model> model = std::make_unique<Model>();
	bool isSuccess = model->Load(_modelData);
	if (!isSuccess)
		return nullptr;	// 失敗したら終了

	Model* retModel = model.get();	// 返すモデルを取得
	// リソース管理にセットする
	pReCollecter->SetResource<Model>(setName, std::move(model));

	return retModel;
}

Texture* TextureLoader::Load(TextureSetting _texSetting)
{
	// リソース管理に既にロードされているか確認する
	std::string& setName = _texSetting.setName;

	// リソース管理に既にあるか確認する
	ResourceCollection* pReCollecter = ResourceCollection::GetInstance();
	if (pReCollecter->GetImpotred(setName))
	{
		// 既にロードしているテクスチャを返す
		return pReCollecter->GetResource<Texture>(setName);	
	}

	// テクスチャをロードする
	std::unique_ptr<Texture> pTex = std::make_unique<Texture>();
	bool isSuccess = pTex->Load(_texSetting.pathName);
	if (!isSuccess)
		return nullptr;	// 失敗したら終了

	Texture* retTex = pTex.get();
	// リソース管理にセットする
	pReCollecter->SetResource<Texture>(setName, std::move(pTex));

	return retTex;
}