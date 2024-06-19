#include "MainApplication.h"
#include "SceneManager.h"
#include "ImGuiMethod.h"
#include "VariableFrameRate.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

constexpr short FPS(60);	// フレームレート数

std::unique_ptr<Window> MainApplication::pWindow = nullptr;	// ウィンドウ処理クラス
Direct3D11* MainApplication::pD3D = nullptr;	// Direct3Dの機能を持つクラス
SceneManager* MainApplication::pSceneManager = nullptr;	// シーンマネージャークラス
std::unique_ptr<VariableFrameRate> MainApplication::variableFps = nullptr;	// 可変フレームレート
std::unique_ptr<InputClass> MainApplication::input = nullptr;	// 入力クラス

void MainApplication::Release()
{
	// シングルトンインスタンスを解放
	Direct3D11::Delete();
	SceneManager::Delete();
	// メモリリークの表示に映らないように手動で解放
	pWindow.reset();
	variableFps.reset();
	input.reset();

	// ImGuiの終了処理
	ImGuiMethod::End();
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// ウィンドウクラスの確保
	pWindow = std::make_unique<Window>();
	// ウィンドウの初期化
	pWindow->Init(_hInst);

	// 可変フレームレートクラス生成
	variableFps = std::make_unique<VariableFrameRate>(FPS);
	input = std::make_unique<InputClass>();	// 入力

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

#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
#endif // _DEBUG


	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		// ImGuiの更新処理
		ImGuiMethod::NewFrame();

		input->Update();	// 入力関係の更新

#ifdef _DEBUG
		ImGui::Begin("System");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		GamePad()->DebugInput();
		ImGui::End();
#endif // _DEBUG

		// 更新処理
		pSceneManager->Exec();

		// 対応したフレームレートにするために時間を待機させる
		variableFps->Wait();
	}
}

float MainApplication::DeltaTime()
{
	return variableFps->GetDeltaTime();
}
