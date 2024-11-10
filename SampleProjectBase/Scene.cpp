#include "pch.h"
#include "Scene.h"

#include "InSceneSystemManager.h"
#include "ShaderCollection.h"
#include "DX11BulletPhisics.h"
#include "Geometory.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Scene::Scene(const std::string& _sceneName) : sceneName(_sceneName)
{
	pInSceneSystem = InSceneSystemManager::GetInstance();
	pInSceneSystem->Init();

	// ロードする
	SceneLoad();
}

Scene::~Scene()
{
	InSceneSystemManager::Delete();
}

void Scene::Exec()
{
	// シーン内更新
	SceneUpdate();

	// シーン描画
	SceneDraw();
}

void Scene::SceneUpdate()
{
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();
	DX11BulletPhisics* pBulletEngine = DX11BulletPhisics::GetInstance();

	// ゲーム内入力更新処理
	pInSceneSystem->InputUpdate();

	// 開始処理
	sceneObjects.Awake();
	sceneObjects.Start();

	// 物理シミュレーションを進める
	pBulletEngine->Update();

	// シーン内の更新処理
	sceneObjects.Update();
	sceneObjects.LateUpdate();

	// Dxのトランスフォームを Bulletに合わせる
	pBulletEngine->UpdateTransformDxToBt();
}

void Scene::SceneDraw()
{
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

	// 描画前準備
	DrawSetup();

	// シーン内の描画処理
	sceneObjects.Draw();

	// 線描画
	Geometory::DrawLine();
}

void Scene::SceneLoad()
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

//bool Scene::IsUpdatePlay()
//{
//#ifdef EDIT
//	if (!isPlaying) return false;
//#endif // EDIT
//
//	return true;
//}

//void Scene::ImGuiPlaying()
//{
//#ifdef EDIT
//	if (ImGui::Button("Stop"))
//		PlayEnd();
//#endif // EDIT
//}
//
//void Scene::ImGuiStop()
//{
//#ifdef EDIT
//	if (ImGui::Button("Play"))
//		PlayStart();
//
//	// 以下再生中は表示しない
//	if (ImGui::Button("Save"))
//	{
//		Save();
//	}
//#endif // EDIT
//}
//
//void Scene::PlayStart()
//{
//	isPlaying = true;
//
//	// シーン再生前にセーブする
//	Save();
//}

//void Scene::PlayEnd()
//{
//	isPlaying = false;
//
//	// 新しくシーンの中を生成する
//	InSceneSystemManager::GetInstance()->Reset();
//	Load();
//}
