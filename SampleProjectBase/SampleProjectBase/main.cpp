#include <Windows.h>
#include <crtdbg.h>
#include "MainApplication.h"

// エントリポイント
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	// ウィンドウを表示する機能クラスを取得
	MainApplication* application;
	application = MainApplication::GetInstance();

	// アプリ初期化
	application->Init(hInst);

	//// ループ : アプリメインの実装を書くところ
	application->GameLoop();

	// アプリ解放
	MainApplication::Delete();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();
	return 0;
}