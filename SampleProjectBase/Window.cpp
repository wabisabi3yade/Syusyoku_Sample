#include "Window.h"

// IMGUI��WindowsAPI�̃C�x���g���擾���邽�߂̊֐����O���Q��
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

constexpr SIZE DEFAULT_SIZE = { 1280, 720 };
constexpr char CLASS_NAME[] = "Duel Knight";

Window::Window()
{
	// �E�B���h�E�T�C�Y��ݒ�
	sizeWindow = DEFAULT_SIZE;
}

Window::Window(SIZE _sizeWindow)
{
	// �����̃T�C�Y��ݒ�
	sizeWindow = _sizeWindow;
}

Window::~Window()
{
    Terminate();
}

bool Window::Init(HINSTANCE _hInst)
{
    // �E�C���h�E �N���X�̓o�^
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

    // ���C�� �E�C���h�E�쐬
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

    // �E�C���h�E�\��
    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    return true;
}

void Window::OnResize(UINT width, UINT height)
{

}

bool Window::MessageLoop()
{
    // Windows���b�Z�[�W���[�v����
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
    {
        TranslateMessage(&msg); DispatchMessage(&msg);
    }

    // �A�v���P�[�V�������I���Ƃ���message��WM_QUIT�ɂȂ� 
    if (msg.message == WM_QUIT) 
    {
        return false;
    }

    return true;
}

bool Window::Terminate()
{
    DestroyWindow(hWnd);

    // �E�C���h�E �N���X�̓o�^����
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
    case WM_DESTROY:// �E�B���h�E�j���̃��b�Z�[�W
        PostQuitMessage(0);// �gWM_QUIT�h���b�Z�[�W�𑗂�@���@�A�v���I��
        break;

    case WM_CLOSE:  // x�{�^���������ꂽ��
        DestroyWindow(hWnd);  // �gWM_DESTROY�h���b�Z�[�W�𑗂�@���@�E�B���h�E�j��
        break;

    default:
        // ���case�ȊO�̏ꍇ�̏��������s
        return DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }

    return 0;
}
