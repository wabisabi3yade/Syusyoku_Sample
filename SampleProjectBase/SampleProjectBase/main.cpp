#include <Windows.h>
#include <crtdbg.h>
#include "MainApplication.h"
// エントリポイント
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	// アプリ初期化
	MainApplication::Init(hInst);

	//// ループ : アプリメインの実装を書くところ
	MainApplication::GameLoop();

	// アプリ解放
	MainApplication::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();
	return 0;
}