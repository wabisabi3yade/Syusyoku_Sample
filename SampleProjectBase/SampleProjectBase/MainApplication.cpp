#include "MainApplication.h"
#include "Direct3D11.h"
#include "SceneManager.h"
#include "Camera.h"
#include "ImGuiMethod.h"
#include "VariableFrameRate.h"

constexpr short FPS(60);	// フレームレート数

MainApplication::MainApplication()
{
	// 可変フレームレートクラス生成
	variableFps = std::make_unique<VariableFrameRate>(FPS);
}

MainApplication::~MainApplication()
{
	Release();
}

void MainApplication::Release()
{
	// シングルトンインスタンスを解放
	Direct3D11::Delete();
	SceneManager::Delete();
	ResourceCollection::Delete();

	// ImGuiの終了処理
	ImGuiMethod::End();
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// ウィンドウクラスの確保
	pWindow = std::make_unique<Window>();
	// ウィンドウの初期化
	pWindow->Init(_hInst);

	// Direct3Dクラスの確保
	pD3D = Direct3D11::GetInstance();
	HWND hwnd = pWindow->GetWindowHandle();
	// 初期化
	pD3D->Init(hwnd);
	// シーンマネージャーを作成
	pSceneManager = SceneManager::GetInstance();

	// ImGuiの初期化
	D3D11_Renderer* pRenderer = pD3D->GetRenderer();
	ImGuiMethod::Initialize(
		pWindow->GetWindowHandle(),
		pRenderer->GetDevice(),
		pRenderer->GetDeviceContext()
	);
}

void MainApplication::GameLoop()
{
	variableFps->Init();

	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		// ImGuiの更新処理
		ImGuiMethod::NewFrame();

		// 更新処理
		pSceneManager->Exec();

		// 対応したフレームレートにするために時間を待機させる
		variableFps->Wait();
	}
}