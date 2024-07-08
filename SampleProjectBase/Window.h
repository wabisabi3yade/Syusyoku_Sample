#pragma once
#include <windows.h>
// ウィンドウを処理するクラス
class Window
{
	WNDCLASS wc;	// ウィンドウクラス
	HWND hWnd = nullptr;	// ウィンドウハンドル（１つのウィンドウを表すID的なもの）
	SIZE sizeWindow;	// ウィンドウの大きさ(cx:横幅, cy:高さ)
	MSG msg = {};
public:
	Window();
	Window(SIZE _sizeWindow);
	virtual ~Window();

	/// <summary>
	/// 初期化する
	/// </summary>
	/// <param name="_hInst">ハンドルインスタンス</param>
	/// <returns>初期化成功したかどうか</returns>
	bool Init(HINSTANCE _hInst);

	// ウィンドウの大きさを再設定
	void OnResize(UINT width, UINT height);

	bool MessageLoop();

	// ウィンドウの終了処理
	bool Terminate(); 

	//メッセージを処理する関数
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ハンドルウィンドウを取得
	HWND GetWindowHandle() { return hWnd; }
};

