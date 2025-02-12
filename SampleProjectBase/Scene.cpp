#include "pch.h"
#include "Scene.h"
#include "ShaderCollection.h"
#include "DX11BulletPhisics.h"
#include "Geometory.h"

namespace HashiTaku
{
	Scene::Scene(const std::string& _sceneName) : sceneName(_sceneName)
	{
		pInSceneSystem = InSceneSystemManager::GetInstance();
		pInSceneSystem->Init();

		pEffectManager = DX11EffecseerManager::GetInstance();

		// ロードする
		SceneLoad();
	}

	Scene::~Scene()
	{
		OnTerminal();
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
		SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();
		DX11BulletPhisics* pBulletEngine = DX11BulletPhisics::GetInstance();

		// ゲーム内入力更新処理
		pInSceneSystem->InputUpdate();

		// 開始処理
		sceneObjects.Awake();
		sceneObjects.Start();

		// 物理シミュレーションを進める
		float deltaTime = pInSceneSystem->GetTimeScale() * MainApplication::DeltaTime();
		pBulletEngine->Update(deltaTime);

		// シーン内の更新処理
		sceneObjects.Update();
		sceneObjects.LateUpdate();

		// Dxのトランスフォームを Bulletに合わせる
		pBulletEngine->UpdateTransformDxToBt();

		// エフェクト更新処理
		pEffectManager->Update();

		// サウンドマネジャー更新処理
		DXSoundManager::GetInstance()->Update();
	}

	void Scene::SceneDraw()
	{
		SceneObjects& sceneObjects = pInSceneSystem->GetSceneObjects();

		// 描画前準備
		DrawSetup();

		// 線描画
		Geometory::DrawLine();

		// シーン内の描画処理
		sceneObjects.ObjectDraw();

		// エフェクト描画
		pEffectManager->EffectDraw();

		// シーン内の描画処理
		sceneObjects.UIDraw();
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

		const json sceneData = json::parse(f);

		// シーンオブジェクトをロード
		const json& sceneObjData = sceneData["objects"];
		pInSceneSystem->GetSceneObjects().LoadObjectList(sceneObjData);

		HASHI_DEBUG_LOG(sceneName + " ロード完了");
	}

	void Scene::OnTerminal()
	{
		InSceneSystemManager::Delete();

		// エフェクトマネジャーの状態をリセット
		pEffectManager->Reset();

		// サウンドを終了
		DXSoundManager::GetInstance()->StopAll();
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
		ShadowDrawer& shadowDrawer = pInSceneSystem->GetShadowDrawer();

		// 画面クリアなど準備
		Direct3D11::GetInstance()->GetRenderer()->SetUpDraw();

		// 光源の更新処理
		sceneLights.Update();

		// ビュー変換行列を更新
		pInSceneSystem->UpdateViewMatrix();

		// 深度テクスチャを作成
		shadowDrawer.CreateDepthTexture();

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
}