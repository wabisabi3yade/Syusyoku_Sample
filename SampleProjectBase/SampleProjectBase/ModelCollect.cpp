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
	bool isSucess  = loadModel->Load(_settings);
	if (!isSucess)
		return false;

	// モデルをマップ配列に入れる
	resourceList.emplace(_settings.modelName, std::move(loadModel));

	return true;
}

std::unique_ptr<Model> ModelCollect::GetModel(std::string _modelName)
{
	// データ元のモデルを取得する
	const Model* pOriginModel = GetConstResource(_modelName);

	// モデルがなかったなら
	if (pOriginModel == nullptr)
	{
		std::string message = "モデルがまだロードされていません" + _modelName;
		ImGuiDebugLog::AddDebugLog(message);

		return nullptr;	// nullptrを返す
	}

	// 送る新しくポインタを確保する
	std::unique_ptr<Model> pSetModel = std::make_unique<Model>();
	pSetModel->SetModel(*pOriginModel);	// 元のモデルからメッシュなどの情報をコピーする

	// アドレスを移動させる
	return std::move(pSetModel);
}
