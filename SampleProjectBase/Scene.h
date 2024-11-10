#pragma once
#include "BulletTest.h"

class InSceneSystemManager;
class EditScene;

// 1シーンクラス
class Scene /*: public HashiTaku::IImGuiUser*/
{
protected:
	/// @brief シーン名
	std::string sceneName;

	/// @brief シーン内システムマネジャー
	InSceneSystemManager* pInSceneSystem;
public:
	Scene(const std::string& _name);
	virtual ~Scene();

	/// @brief 実行関数
	virtual void Exec();

	///// @brief セーブ機能
	//void Save();

	// シーン名設定
	void SetName(const std::string& _name);

	// 名前を取得
	std::string GetName() const;

protected:
	/// @brief シーンない更新
	void SceneUpdate();

	/// @brief シーン内描画
	void SceneDraw();

	/// @brief jsonファイルからシーンをロード
	void SceneLoad();
private:
	/// @brief 描画準備
	void DrawSetup();

	// セーブファイルのパス名
	std::string SaveFilePath();

	/// @brief シーン再生するか？
	/// @return 再生するか？
	bool IsUpdatePlay();

	///// @brief 再生中の表示
	//void ImGuiPlaying();

	///// @brief 停止中の表示
	//void ImGuiStop();

	///// @brief 再生を始める
	//void PlayStart();

	///// @brief 再生を止める
	//void PlayEnd();
};

