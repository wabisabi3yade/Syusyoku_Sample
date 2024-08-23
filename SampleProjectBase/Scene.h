#pragma once

class InSceneSystemManager;

// 1シーンクラス
class Scene
{
	/// @brief シーン名
	std::string sceneName;

	/// @brief シーン内システムマネジャー
	InSceneSystemManager* pInSceneSystem;

public:
	Scene(const std::string& _name);
	~Scene();

	/// @brief 実行関数
	void Exec();

	/// @brief セーブ機能
	void Save();

	/// @brief シーン名設定
	/// @param _name 名前
	void SetName(const std::string& _name);

	/// @brief シーン名を取得
	std::string GetName() const;
private:
	void Load();

	/// @brief 描画準備
	void DrawSetup();

	// セーブファイルのパス名
	std::string SaveFilePath();

	/// @brief シーンオブジェクトをロード
	/// @param _objectData シーンオブジェクトデータ
	void LoadSceneObject(const nlohmann::json& _objectData);
};

