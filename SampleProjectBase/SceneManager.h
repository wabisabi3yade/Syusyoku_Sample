#pragma once
#include "Singleton_Base.h"

class BroadScene_Base;
class ChangeBroadScene;
class SceneMoveInfo;

class SceneManager :public Singleton_Base<SceneManager>
{
	friend class Singleton_Base<SceneManager>;

	static SceneManager* pInstance;	// インスタンス
	BroadScene_Base* pNowBroadScene = nullptr;	// 今実行している大局シーン
	ChangeBroadScene* pChaneBroad = nullptr;	// 大局シーンを遷移するクラス
	SceneMoveInfo* pMoveInfo;	// シーン遷移情報

	SceneManager();
	~SceneManager();

	/// @brief アセットの準備
	void AssetSetup();

	void CheckChangeBroad();	// シーンを遷移するか確認する

	// 解放処理
	void Release();

public:

	// 実行関数
	void Exec();

private:
	/// @brief マテリアルの準備
	void MaterialSetup();

	/// @brief メッシュの準備
	void BasicMeshSetup();
};

