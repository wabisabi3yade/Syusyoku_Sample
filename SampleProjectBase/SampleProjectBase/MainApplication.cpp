#include "MainApplication.h"
#include "SceneManager.h"
#include "Camera.h"
#include "ImGuiMethod.h"
#include "VariableFrameRate.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

constexpr short FPS(60);	// フレームレート数

MainApplication::MainApplication()
{
	// 可変フレームレートクラス生成
	variableFps = std::make_unique<VariableFrameRate>(FPS);
	input = std::make_unique<InputClass>();
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

#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
#endif // _DEBUG


	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		// ImGuiの更新処理
		ImGuiMethod::NewFrame();

#ifdef _DEBUG
		ImGui::Begin("System");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
#endif // _DEBUG

		input->Update();	// 入力関係の更新

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
