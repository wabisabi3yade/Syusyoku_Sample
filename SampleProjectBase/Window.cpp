#include "Window.h"

// IMGUIがWindowsAPIのイベントを取得するための関数を外部参照
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

constexpr SIZE DEFAULT_SIZE = { 1280, 720 };
constexpr char CLASS_NAME[] = "Duel Knight";

Window::Window()
{
	// ウィンドウサイズを設定
	sizeWindow = DEFAULT_SIZE;
}

Window::Window(SIZE _sizeWindow)
{
	// 引数のサイズを設定
	sizeWindow = _sizeWindow;
}

Window::~Window()
{
    Terminate();
}

bool Window::Init(HINSTANCE _hInst)
{
    // ウインドウ クラスの登録
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)Window::MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = _hInst;
    wc.hIcon = LoadIcon(_hInst, IDI_APPLICATION);
    wc.hCursor = LoadCursor(_hInst, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME;
    if (!RegisterClass(&wc)) {
        return false;
    }

    // メイン ウインドウ作成
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.right = sizeWindow.cx;
    rect.bottom = sizeWindow.cy;
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindow(wc.lpszClassName, CLASS_NAME,
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        nullptr, nullptr, _hInst, nullptr);
    if (hWnd == nullptr) {
        return false;
    }

    // ウインドウ表示
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    return true;
}

void Window::OnResize(UINT width, UINT height)
{

}

bool Window::MessageLoop()
{
    // Windowsメッセージループ処理
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
    {
        TranslateMessage(&msg); DispatchMessage(&msg);
    }

    // アプリケーションが終わるときにmessageがWM_QUITになる 
    if (msg.message == WM_QUIT) 
    {
        return false;
    }

    return true;
}

bool Window::Terminate()
{
    DestroyWindow(hWnd);

    // ウインドウ クラスの登録解除
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    hWnd = nullptr;
    return true;
}

LRESULT Window::MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_DESTROY:// ウィンドウ破棄のメッセージ
        PostQuitMessage(0);// “WM_QUIT”メッセージを送る　→　アプリ終了
        break;

    case WM_CLOSE:  // xボタンが押されたら
        DestroyWindow(hWnd);  // “WM_DESTROY”メッセージを送る　→　ウィンドウ破棄
        break;

    default:
        // 上のcase以外の場合の処理を実行
        return DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }

    return 0;
}
