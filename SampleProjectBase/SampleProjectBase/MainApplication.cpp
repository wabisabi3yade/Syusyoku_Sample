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
bool MainApplication::isEscapeDisplay = false;

void MainApplication::Release()
{
	// シングルトンインスタンスを解放
	Direct3D11::Delete();
	SceneManager::Delete();

#ifdef _DEBUG
	// メモリリークの表示に映らないように手動で解放
	pWindow.reset();
	variableFps.reset();
	input.reset();
#endif // DEBUG

	// ImGuiの終了処理
	ImGuiMethod::End();
}

bool MainApplication::Escape()
{
	// エスケープキー押されたら
	if (!isEscapeDisplay && input->GetKeyboard().GetKeyDown(DIK_ESCAPE))
		isEscapeDisplay = true;

	if (!isEscapeDisplay) return false;

	bool isEscape = false;

	// アプリケーション終了表示

	// ウィンドウ大きさ設定
	ImVec2 windowSize(170, 70);
	ImVec2 windowPos(
		(ImGui::GetIO().DisplaySize.x - windowSize.x) * 0.5f,
		(ImGui::GetIO().DisplaySize.y - windowSize.y) * 0.5f
	);
	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);

	// ボタン処理
	ImGui::Begin(ShiftJisToUtf8("終了しますか").c_str(), nullptr, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::Button(ShiftJisToUtf8("はい").c_str()))
	{
		isEscape = true;
		isEscapeDisplay = false;
	}
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(30.0f, 0.0f));
	ImGui::SameLine();
	if (ImGui::Button(ShiftJisToUtf8("いいえ").c_str()))
	{
		isEscapeDisplay = false;
	}
	ImGui::End();

	return isEscape;
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// ウィンドウクラスの確保
	pWindow = std::make_unique<Window>();
	pWindow->Init(_hInst);

	// Direct3Dクラスの確保
	pD3D = Direct3D11::GetInstance();
	HWND hwnd = pWindow->GetWindowHandle();
	pD3D->Init(hwnd);

	// 可変フレームレートクラス生成
	variableFps = std::make_unique<VariableFrameRate>(FPS);

	// 入力クラスを作成
	input = std::make_unique<InputClass>();
	input->Init(hwnd);

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	while (true)
	{
		static ULONGLONG start = 0;
		static ULONGLONG end = 0;
		start = GetTickCount64();

		bool result = pWindow->MessageLoop();
		if (result == false) break;

		ImGuiMethod::NewFrame();

		// 終了処理チェック
		if (Escape())
			return;

		// 入力更新
		input->Update();

		// 更新処理
		pSceneManager->Exec();

		ImGuiMethod::Draw();

		// スワップチェイン
		Direct3D11::GetInstance()->GetRenderer()->Swap();
		
		ULONGLONG b = 0;
		
		end = GetTickCount64();
		ULONGLONG delta1 = end - start;

		// 待機
		variableFps->Wait();

		
		end = GetTickCount64();

		ULONGLONG delta = end - start;
	}
}

float MainApplication::DeltaTime()
{
	return variableFps->GetDeltaTime();
}
