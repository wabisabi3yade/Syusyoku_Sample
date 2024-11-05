#include "SceneManager.h"

#include "GameInput.h"
#include "ComponentFactory.h"
#include "DX11BulletPhisics.h"
#include "AnimationNotifyFactory.h"

// アセット初期化
#include "AssetSetter.h"
#include "Geometory.h"
#include "Material.h"

namespace fs = std::filesystem;

SceneManager::SceneManager() : nowSceneName("")
{
	Setup();

	CreateScene("test");

	ChangeScene("test");
}

SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::Setup()
{

	// Bullet物理エンジン初期化
	DX11BulletPhisics::GetInstance()->Init();

	// アセット関連
	AssetSetup();

	// コンポーネント初期化
	ComponentFactory::GetInstance()->Init();

	// シーンリストを準備
	SetupSceneList();
}

void SceneManager::SetupSceneList()
{
	std::string folderPath = "assets/data/scene";
	
	// シーンフォルダにあるシーンファイルを取得する
	for (const auto& entry : fs::recursive_directory_iterator(folderPath))
	{
		if (!entry.is_regular_file() || entry.path().extension() != ".json") continue;

		// ファイルのパスを出力
		sceneList.push_back(entry.path().stem().string());
	}
}

void SceneManager::AssetSetup()
{
	// デフォであるマテリアルを作成
	MaterialSetup();

	// 基本オブジェクトの初期化
	Geometory::Init();
}

void SceneManager::Release()
{
	pNowScene.reset();
	GameInput::Delete();
	ComponentFactory::Delete();
	Geometory::Release();
	DX11BulletPhisics::Delete();
}

void SceneManager::OtherDraw()
{
	DX11BulletPhisics* pBulletPhisics = DX11BulletPhisics::GetInstance();

	//// 当たり判定描画
	pBulletPhisics->Draw();

	// 線描画
	Geometory::DrawLine();
}

void SceneManager::CreateScene(const std::string& _sceneName)
{
	auto itr = std::find(sceneList.begin(), sceneList.end(), _sceneName);

	if (itr != sceneList.end())	// シーン名が重複していたら
	{
		HASHI_DEBUG_LOG(_sceneName + "既にシーン名が使われています");
		return;
	}

	sceneList.push_back(_sceneName);
}

void SceneManager::ImGuiSetting()
{
	ImGui::Begin("SceneList");

	pNowScene->ImGuiCall();

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGuiChangeScene();

	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	ImGuiCreateScene();

	ImGui::End();
}

void SceneManager::ImGuiChangeScene()
{
	ImGui::Text(TO_UTF8("シーン名 " + nowSceneName));

	for (auto& name : sceneList)
	{
		if (ImGui::Button(name.c_str()))
			ChangeScene(name);
	}
}

void SceneManager::ImGuiCreateScene()
{
	constexpr u_int buf = 256;
	static char input[buf];
	ImGui::InputText("name", input, buf);

	if (ImGui::Button("New Scene"))
		CreateScene(input);
}

void SceneManager::Exec()
{
	// ゲーム内入力更新
	GameInput::GetInstance()->Update();

	// シーンの実行
	pNowScene->Exec();

	// オブジェクト以外描画
	OtherDraw();

	ImGuiCall();
}

void SceneManager::ChangeScene(const std::string& _sceneName)
{
	// 現在シーンなら処理しない
	if (nowSceneName == _sceneName) return;

	auto itr = std::find(sceneList.begin(), sceneList.end(), _sceneName);

	if (itr == sceneList.end())	// シーン名がないなら
	{
		HASHI_DEBUG_LOG(_sceneName + "シーン名がありません");
		return;
	}

	pNowScene.reset();

	nowSceneName = _sceneName;
	pNowScene = std::make_unique<Scene>(_sceneName);

	HASHI_DEBUG_LOG(_sceneName + "へ移行");
}

void SceneManager::MaterialSetup()
{
	// Unlitマテリアル作成
	std::unique_ptr<Material> pUnlit = std::make_unique<Material>();
	pUnlit->SetIsntSave();
	AssetSetter::SetAsset("M_Unlit", std::move(pUnlit));
}
