#include "pch.h"
#include "ModelCollect.h"
#include "ModelSettings.h"
#include "Model.h"

bool ModelCollect::Load(const ModelSettings& _settings)
{
	// モデル格納クラスに既にロードされているならロード処理しない
	bool isImported = ModelCollect::GetInstance()->GetIsImported(_settings.modelName);
	if (isImported)
		return true;

	// ロードされるモデル情報
	std::unique_ptr<Model> loadModel = std::make_unique<Model>();
	// モデルのロード処理を行う
	bool isSucess  = loadModel->SetSetting(_settings);
	if (!isSucess)
		return false;

	// モデルをマップ配列に入れる
	resourceList.emplace(_settings.modelName, std::move(loadModel));

	return true;
}
