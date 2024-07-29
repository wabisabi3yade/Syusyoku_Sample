#include "MainApplication.h"

// システム関連
#include "SceneManager.h"
#include "AssetCollection.h"
#include "ShaderCollection.h"

// Asset関連
#include "AssetContacter.h"
#include "AssetDisplay.h"

// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

constexpr u_int FPS(60);	// フレームレート数

// static変数初期化
std::unique_ptr<Window> MainApplication::pWindow = nullptr;
Direct3D11* MainApplication::pD3D = nullptr;
SceneManager* MainApplication::pSceneManager = nullptr;
std::unique_ptr<VariableFrameRate> MainApplication::pVariableFps = nullptr;
std::unique_ptr<InputClass> MainApplication::pInput = nullptr;
std::unique_ptr<AssetCollection> MainApplication::pAssetCollection = nullptr;
bool MainApplication::isEscapeDisplay = false;

void MainApplication::Release()
{
	// ImGuiの終了処理
	ImGuiMethod::Terminal();

	// シングルトンインスタンスを解放
	SceneManager::Delete();
	ShaderCollection::Delete();
	Direct3D11::Delete();
}

bool MainApplication::EscapeCheck()
{
	// エスケープキー押されたら
	if (!isEscapeDisplay && pInput->GetKeyboard().GetKeyDown(DIK_ESCAPE))
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

void MainApplication::SystemDraw()
{
	ImGui::Begin(ShiftJisToUtf8("システム").c_str());
	AssetDisplay::Draw();
	pVariableFps->Draw();
	ImGui::End();

	ImGuiMethod::Draw();
}

void MainApplication::Init(HINSTANCE _hInst)
{
	WindowSetup(_hInst);

	HWND hwnd = pWindow->GetWindowHandle();

	D3DSetup(hwnd);

	VariableFrameSetup();

	InputSetup(hwnd);

	AssetSysytemSetup();

	ShaderSetup();

	ImuiSetup();

	SceneManagerSetup();
}

void MainApplication::GameLoop()
{
	// 前回時間の初期化
	pVariableFps->Init(FPS);

	while (true)
	{
		// 1フレームの時間に合わせる
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

void MainApplication::ImuiSetup()
{
	// ImGuiの初期化
	D3D11_Renderer* pRenderer = pD3D->GetRenderer();
	ImGuiMethod::Initialize(
		pWindow->GetWindowHandle(),
		pRenderer->GetDevice(),
		pRenderer->GetDeviceContext()
	);
}

void MainApplication::AssetSysytemSetup()
{
	pAssetCollection = std::make_unique<AssetCollection>();

	// アセット連絡インターフェースに管理クラスの参照を渡す
	AssetContacter::SetAssetCollection(*pAssetCollection.get());
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
