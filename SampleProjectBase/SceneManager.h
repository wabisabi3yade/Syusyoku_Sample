#pragma once
#include "Singleton_Base.h"

#include "EditScene.h"

class SceneManager : public Singleton_Base<SceneManager>, public HashiTaku::IImGuiUser
{
	friend class Singleton_Base<SceneManager>;

	/// @brief シーンリスト
	std::list<std::string> sceneList;

	/// @brief 現在のシーン名
	std::string nowSceneName;

	/// @brief 現在再生しているシーン
	std::unique_ptr<Scene> pNowScene;

	/// @brief 次のシーン名
	std::string nextSceneName;

	/// @brief 変更するか？
	bool isChange;

	SceneManager();
	~SceneManager();
	
public:
	// 実行関数
	void Exec();

	/// @brief シーンを変更するようにリクエストする
	/// @param _sceneName シーン名
	void ChangeSceneRequest(const std::string& _sceneName);

private:
	/// @brief  準備
	void Setup();

	/// @brief 全シーン名を準備
	void SetupSceneList();

	/// @brief アセットの準備
	void AssetSetup();

	/// @brief マテリアルの準備
	void MaterialSetup();

	/// @brief シーンを変更する
	/// @param _sceneName シーン名
	/// @param _isEditScene EditSceneか？
	void ChangeScene(const std::string& _sceneName, bool _isEditScene = false);

	// 解放処理
	void Release();

	/// @brief シーンを作成
	/// @param _sceneName シーン名
	void CreateScene(const std::string& _sceneName);

	void ImGuiDebug() override;
	void ImGuiChangeScene();
	void ImGuiCreateScene();
};

