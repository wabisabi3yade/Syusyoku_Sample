#include "pch.h"
#include "ModelLoader.h"

#define MODEL_NC_NUM (3)	// モデルの命名規則で使われる文字数

bool ModelLoader::Load(const ModelSettings& _modelData)
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
		return true;	// 既にロードされているなら処理を抜ける
	}

	// モデルをロードする
	std::unique_ptr<Model> model = std::make_unique<Model>();
	bool isSuccess = model->Load(_modelData);
	if (!isSuccess)
		return false;	// 失敗したら終了

	// リソース管理にセットする
	pReCollecter->SetResource<Model>(setName, std::move(model));

	return true;
}

