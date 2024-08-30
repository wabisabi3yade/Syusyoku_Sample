#include "pch.h"
#include "Scene.h"

#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Scene::Scene(const std::string& _name) : isPlay(true)
{
	pInSceneSystem = InSceneSystemManager::GetInstance();
	pInSceneSystem->Init();

	sceneName = _name;

	// ImGui使用するなら
	if (HashiTaku::IImGuiUser::GetIsImGuiUse())
		isPlay = false;

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

	if (IsUpdatePlay())	// 再生中なら
	{
		// シーン内の更新処理
		sceneObjects.Update();
	}

	// ImGui編集
	sceneObjects.ImGuiSetting();

	// 描画前準備
	DrawSetup();

	// シーン内の描画処理
	sceneObjects.Draw();
}

void Scene::ImGuiSetting()
{
	if (isPlay)
		ImGuiPlaying();
	else
		ImGuiStop();
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

	HASHI_DEBUG_LOG(sceneName + " セーブしました");
}

bool Scene::GetIsUpdatePlay() const
{
	return isPlay;
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
	pInSceneSystem->GetSceneObjects().LoadObject(sceneObjData);

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

bool Scene::IsUpdatePlay()
{
#ifdef EDIT
	if (!isPlay) return false;
#endif // EDIT

	return true;
}

void Scene::ImGuiPlaying()
{
	if (ImGui::Button("Stop"))
		PlayStop();
}

void Scene::ImGuiStop()
{
	if (ImGui::Button("Play"))
		PlayStart();

	// 以下再生中は表示しない
	if (ImGui::Button("Save"))
	{
		Save();
	}
}

void Scene::PlayStart()
{
	isPlay = true;

	// シーン再生前にセーブする
	Save();
}

void Scene::PlayStop()
{
	isPlay = false;

	// 新しくシーンの中を生成する
	InSceneSystemManager::GetInstance()->Reset();
	Load();
}
