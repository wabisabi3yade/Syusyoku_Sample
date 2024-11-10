#include "SceneManager.h"

#include "GameInput.h"
#include "ComponentFactory.h"
#include "DX11BulletPhisics.h"
#include "AnimationNotifyFactory.h"

// アセット初期化
#include "AssetSetter.h"
#include "Material.h"
#include "Geometory.h"

namespace fs = std::filesystem;

SceneManager::SceneManager() : nowSceneName("")
{
	Setup();

	ChangeScene("test", true);
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
	ComponentFactory::Delete();
	Geometory::Release();
	DX11BulletPhisics::Delete();
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

void SceneManager::ImGuiDebug()
{
#ifdef EDIT
	ImGui::Begin("SceneList");

	ImGuiChangeScene();

	ImGuiCreateScene();

	ImGui::End();
#endif
}

void SceneManager::ImGuiChangeScene()
{
#ifdef EDIT
	ImGui::Text(TO_UTF8("シーン名 " + nowSceneName));
	
	// 再生のみのシーン
	ImGui::Text("Play Only");
	for (auto& name : sceneList)
	{
		if (ImGui::Button(name.c_str()))
			ChangeScene(name);
	}

	ImGui::Text("--------------");

	// 編集できるシーン
	ImGui::Text("Edit");
	ImGui::PushID("Edit");
	for (auto& name : sceneList)
	{
		if (ImGui::Button(name.c_str()))
			ChangeScene(name, true);
	}
	ImGui::PopID();
#endif
}

void SceneManager::ImGuiCreateScene()
{
#ifdef EDIT
	constexpr u_int buf = 256;
	static char input[buf];
	ImGui::InputText("name", input, buf);

	if (ImGui::Button("New Scene"))
		CreateScene(input);
#endif
}

void SceneManager::Exec()
{
	// シーンの実行
	pNowScene->Exec();

	ImGuiCall();
}

void SceneManager::ChangeScene(const std::string& _sceneName, bool _isEditScene)
{
	auto itr = std::find(sceneList.begin(), sceneList.end(), _sceneName);

	if (itr == sceneList.end())	// シーン名がないなら
	{
		HASHI_DEBUG_LOG(_sceneName + "シーン名がありません");
		return;
	}

	pNowScene.reset();
	nowSceneName = _sceneName;

	// 編集するシーンかどうか
	if (!_isEditScene)
		pNowScene = std::make_unique<Scene>(_sceneName);
	else
		pNowScene = std::make_unique<EditScene>(_sceneName);

	HASHI_DEBUG_LOG(_sceneName + "へ移行");
}

void SceneManager::MaterialSetup()
{
	// Unlitマテリアル作成
	std::unique_ptr<Material> pUnlit = std::make_unique<Material>();
	pUnlit->SetIsntSave();
	AssetSetter::SetAsset("M_Unlit", std::move(pUnlit));
}
