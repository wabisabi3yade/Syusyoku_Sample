#include "pch.h"
#include "EditScene.h"
#include "AssetSaveLoad.h"
#include "InSceneSystemManager.h"

EditScene::EditScene(const std::string& _sceneName) :
	Scene(_sceneName),
	isPlaying(false),
	isPause(false),
	isOneFrameProgressing(false)

{
}

void EditScene::Exec()
{
	SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();

	// 再生中のみシーン更新
	if (CanSceneUpdate())
	{
		SceneUpdate();
		isOneFrameProgressing = false;
	}

	// ImGuiによる編集
	ImGuiCall();

	// シーン描画
	SceneDraw();
}

bool EditScene::CanSceneUpdate()
{
	if ((isPlaying && !isPause) || isOneFrameProgressing)
		return true;

	return false;
}

void EditScene::SceneSave()
{
	// シーンデータ
	nlohmann::json sceneData;

	// シーン内オブジェクトをセーブ
	SceneObjects& sceneObj = pInSceneSystem->GetSceneObjects();
	sceneData["objects"] = sceneObj.SaveObjectList();

	std::ofstream f(SaveFilePath());

	auto str = sceneData.dump(4);
	auto len = str.length();
	f.write(str.c_str(), len);

	HASHI_DEBUG_LOG(sceneName + " セーブしました");

	AssetSaveLoader::Save();	// アセットセーブ
}

std::string EditScene::SaveFilePath()
{
	// ファイル名を設定
	std::string fileName = "assets/data/scene/";	// パス
	fileName += sceneName;	// ファイル名
	fileName += ".json";	// 拡張子

	return fileName;
}

void EditScene::ImGuiDebug()
{
#ifdef EDIT
	SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();
	SceneLights& sceneLights = pInSceneSystem->GetSceneLights();
	DX11BulletPhisics* pBulletPhisics = DX11BulletPhisics::GetInstance();

	// オブジェクトのImGui編集
	sceneObjects.ImGuiCall();

	// ライティングのImGUi編集
	sceneLights.ImGuiCall();

	ImGui::Begin("Edit Play");
	// 再生中とでないときで出すウィンドウを変える
	if (isPlaying)
		ImGuiPlaying();
	else
		ImGuiStop();

	ImGui::End();

	// 当たり判定描画
	pBulletPhisics->Draw();

	// システム表示 
	AppSystemDraw::GetInstance()->ImGuiCall();

	// デバッグメッセージ表示
	ImGuiDebugLog::DisplayMessage();
#endif // EDIT
}

void EditScene::ImGuiPlaying()
{
#ifdef EDIT
	InputClass& input = MainApplication::GetInput();

	// 終わり
	if (ImGui::Button("End"))
		PlayEnd();

	// ポーズ
	if (ImGui::Button("Pause") || input.GetKeyboard().GetKeyDown(DIK_P))
		isPause = !isPause;

	// 1フレーム再生
	if (ImGui::Button("Progress") || input.GetKeyboard().GetKeyDown(DIK_N))
		StartProgressOneFrame();
#endif // EDIT
}

void EditScene::ImGuiStop()
{
#ifdef EDIT
	if (ImGui::Button("Play"))
		PlayStart();

	// 以下再生中は表示しない
	if (ImGui::Button("Save"))
	{
		SceneSave();
	}
#endif // EDIT
}

void EditScene::PlayStart()
{
	isPlaying = true;

	// シーン再生前にセーブする
	SceneSave();
}

void EditScene::PlayEnd()
{
	isPlaying = false;
	isPause = false;
	isOneFrameProgressing = false;

	// 新しくシーンの中を生成する
	InSceneSystemManager::GetInstance()->Reset();
	SceneLoad();
}

void EditScene::PlayPauseAndBegin()
{
	isPlaying = !isPlaying;
}

void EditScene::StartProgressOneFrame()
{
	isOneFrameProgressing = true;
}

