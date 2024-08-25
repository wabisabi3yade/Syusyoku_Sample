#include "pch.h"
#include "Scene.h"

#include "InSceneSystemManager.h"
#include "ShaderCollection.h"



Scene::Scene(const std::string& _name)
{
	pInSceneSystem = InSceneSystemManager::GetInstance();
	pInSceneSystem->Init();

	sceneName = _name;

	// ロードする
	Load();
}

Scene::~Scene()
{
	InSceneSystemManager::Delete();
}

void Scene::Exec()
{
	SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();

	// シーン内の更新処理
	sceneObjects.Update();

	// Updateの後に行う更新処理(カメラに処理とか)
	sceneObjects.LateUpdate();

	// ImGui編集
	sceneObjects.ImGuiSetting();

	// 描画前準備
	DrawSetup();

	// シーン内の描画処理
	sceneObjects.Draw();
}

void Scene::Save()
{
	// シーンデータ
	nlohmann::json sceneData;

	// シーン内オブジェクトをセーブ
	SceneObjects& sceneObj = pInSceneSystem->GetSceneObjects();
	sceneData["objects"] = sceneObj.SaveObject();

	std::ofstream f(SaveFilePath());

	auto str = sceneData.dump(4);
	auto len = str.length();
	f.write(str.c_str(), len);
}

void Scene::Load()
{
	// ファイルからjsonを読み込み
	std::ifstream f(SaveFilePath());

	if (!f.is_open())
	{
		HASHI_DEBUG_LOG("シーンセーブファイルが開けませんでした");
		return;
	}

	const nlohmann::json sceneData = nlohmann::json::parse(f);

	// シーンオブジェクトをロード
	const nlohmann::json& sceneObjData = sceneData["objects"];
	InSceneSystemManager::GetInstance()->GetSceneObjects().LoadObject(sceneObjData);

	HASHI_DEBUG_LOG(sceneName + " ロード完了");
}

void Scene::SetName(const std::string& _name)
{
	sceneName = _name;
}

std::string Scene::GetName() const
{
	return sceneName;
}

void Scene::DrawSetup()
{
	SceneLights& sceneLights = pInSceneSystem->GetSceneLights();

	// 画面クリアなど準備
	Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();

	// 光源の更新処理
	sceneLights.Update();

	// 1ループ1度だけ更新すればいいバッファ更新
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	shCol->UniqueUpdateBuffer();
}

std::string Scene::SaveFilePath()
{
	// ファイル名を設定
	std::string fileName = "assets/data/scene/";	// パス
	fileName += sceneName;	// ファイル名
	fileName += ".json";	// 拡張子

	return fileName;
}
