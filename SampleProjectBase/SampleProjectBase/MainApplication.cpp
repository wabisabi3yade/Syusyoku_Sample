#include "MainApplication.h"
#include "Direct3D11.h"
#include "SceneManager.h"
#include "Camera.h"

MainApplication::MainApplication()
{
}

MainApplication::~MainApplication()
{
	Release();
}

void MainApplication::Release()
{
	Direct3D11::Delete();
	SceneManager::Delete();

	CLASS_DELETE(pWindow);
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// ウィンドウクラスの確保
	pWindow = new Window();
	// ウィンドウの初期化
	pWindow->Init(_hInst);

	// Direct3Dクラスの確保
	pD3D = Direct3D11::GetInstance();
	HWND hwnd = pWindow->GetWindowHandle();
	// 初期化
	pD3D->Init(hwnd);
	// シーンマネージャーを作成
	pSceneManager = SceneManager::GetInstance();
}

void MainApplication::GameLoop()
{
	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		// 更新処理
		pSceneManager->Exec();

		// 消す
		Sleep(10);
	}
}