#include "MainApplication.h"

// システム関連
#include "SceneManager.h"
#include "AssetCollection.h"
#include "ShaderCollection.h"
#include "DX11BulletPhisics.h"
#include "DXSoundManager.h"

// Asset関連
#include "AssetContacter.h"
#include "AssetDisplay.h"
#include "AssetSaveLoad.h"

namespace HashiTaku
{
	constexpr u_int FPS(60);	// フレームレート数

	// static変数初期化
	std::unique_ptr<Window> MainApplication::pWindow = nullptr;
	Direct3D11* MainApplication::pD3D = nullptr;
	SceneManager* MainApplication::pSceneManager = nullptr;
	std::unique_ptr<VariableFrameRate> MainApplication::pVariableFps = nullptr;
	std::unique_ptr<InputClass> MainApplication::pInput = nullptr;
	std::unique_ptr<AssetCollection> MainApplication::pAssetCollection = nullptr;
	bool MainApplication::isEscapeChecking = false;
	bool MainApplication::isExit = false;

	void MainApplication::Release()
	{
		// システム描画終了
		AppSystemDraw::Delete();

		// アセットのセーブを行う
		AssetSaveLoader::Save();

		// ImGuiの終了処理
		ImGuiMethod::Terminal();

		// アセット管理解放
		pAssetCollection.reset();

		// シングルトンインスタンスを解放
		SceneManager::Delete();
		DXSoundManager::Delete();
		DX11EffecseerManager::Delete();
		ShaderCollection::Delete();
		Direct3D11::Delete();
	}

	bool MainApplication::EscapeCheck()
	{
		// エスケープキー押されたら
		if (pInput->GetKeyboard().GetKeyDown(DIK_ESCAPE))
			isEscapeChecking = true;

		if (!isEscapeChecking) return false;

#if _DEBUG
		// 終了するかメッセージボックスを出す
		int id = 0;
		id = MessageBox(NULL, TEXT("ゲームを終了しますか？"),
			TEXT("確認"),
			MB_YESNO | MB_ICONQUESTION);

		if (id == IDYES)
		{
			isEscapeChecking = false;
			return true;
		}
		else if (id == IDNO)
		{
			isEscapeChecking = false;
		}

#else
		return true;
#endif // _DEBUG

		return false;
	}

	void MainApplication::SystemDraw()
	{
		ImGuiMethod::Draw();
	}

	void MainApplication::Init(HINSTANCE _hInst)
	{
		WindowSetup(_hInst);

		HWND hwnd = pWindow->GetWindowHandle();

		D3DSetup(hwnd);

		VariableFrameSetup();

		InputSetup(hwnd);

		ShaderSetup();

		// 乱数クラス初期化
		Random::Init();

		// イージング初期化
		Easing::Init();

		// エフェクシア初期化
		DX11EffecseerManager::GetInstance()->Init();

		// サウンド初期化
		DXSoundManager::GetInstance()->Init();

		AssetSysytemSetup();

		ShaderCollection::GetInstance()->InitAssets();

		ImGuiSetup();

		SceneManagerSetup();
	}

	void MainApplication::ExitRequest()
	{
		isExit = true;
	}

	void MainApplication::GameLoop()
	{
		// 前回時間の初期化
		pVariableFps->Init(FPS);

		while (true)
		{
			//// 1フレームの時間に合わせる
			if (!CanUpdate()) continue;

			bool result = pWindow->MessageLoop();
			if (result == false) break;

			ImGuiMethod::NewFrame();

			// 終了処理チェック
			if (EscapeCheck())
				return;

			// 入力更新
			pInput->Update();

			// 更新処理
			pSceneManager->Exec();

			// システムのGUI描画
			SystemDraw();

			// スワップチェイン
			Direct3D11::GetInstance()->GetRenderer()->Swap();

			pVariableFps->ProcessEnd();

			if (isExit)
				return;
		}
	}

	float MainApplication::DeltaTime()
	{
		return pVariableFps->GetDeltaTime();
	}

	void MainApplication::WindowSetup(HINSTANCE _hInst)
	{
		// ウィンドウクラスの確保
		pWindow = std::make_unique<Window>();
		pWindow->Init(_hInst);
	}

	void MainApplication::D3DSetup(HWND _hwnd)
	{
		// Direct3Dクラスの確保
		pD3D = Direct3D11::GetInstance();
		pD3D->Init(_hwnd);
	}

	void MainApplication::VariableFrameSetup()
	{
		pVariableFps = std::make_unique<VariableFrameRate>();
	}

	void MainApplication::InputSetup(HWND _hwnd)
	{
		// 入力クラスを作成
		pInput = std::make_unique<InputClass>();
		pInput->Init(_hwnd);
	}

	void MainApplication::ImGuiSetup()
	{
#ifdef EDIT
		// ImGuiの初期化
		D3D11_Renderer* pRenderer = pD3D->GetRenderer();
		ImGuiMethod::Initialize(
			pWindow->GetWindowHandle(),
			pRenderer->GetDevice(),
			pRenderer->GetDeviceContext()
		);

		AppSystemDraw::GetInstance()->Init(*pVariableFps);
#endif // EDIT
	}

	void MainApplication::AssetSysytemSetup()
	{
		pAssetCollection = std::make_unique<AssetCollection>();

		// アセット連絡インターフェースに管理クラスの参照を渡す
		AssetContacter::SetAssetCollection(*pAssetCollection.get());

		// アセットロードする
		AssetSaveLoader::Load();
	}

	void MainApplication::ShaderSetup()
	{
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		shCol->Init();
	}

	void MainApplication::SceneManagerSetup()
	{
		pSceneManager = SceneManager::GetInstance();
	}

	bool MainApplication::CanUpdate()
	{
		return pVariableFps->UpdateCheck();
	}
}