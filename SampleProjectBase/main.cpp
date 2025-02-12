#include <Windows.h>
#include <crtdbg.h>
#include "MainApplication.h"

// エントリポイント
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// アプリ初期化
	HashiTaku::MainApplication::Init(hInst);

	// ループ : アプリメインの実装を書くところ
	HashiTaku::MainApplication::GameLoop();

	// アプリ解放
	HashiTaku::MainApplication::Release();

	return 0;
}