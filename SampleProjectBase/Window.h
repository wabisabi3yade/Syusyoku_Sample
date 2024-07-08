#pragma once
#include <windows.h>
// �E�B���h�E����������N���X
class Window
{
	WNDCLASS wc;	// �E�B���h�E�N���X
	HWND hWnd = nullptr;	// �E�B���h�E�n���h���i�P�̃E�B���h�E��\��ID�I�Ȃ��́j
	SIZE sizeWindow;	// �E�B���h�E�̑傫��(cx:����, cy:����)
	MSG msg = {};
public:
	Window();
	Window(SIZE _sizeWindow);
	virtual ~Window();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="_hInst">�n���h���C���X�^���X</param>
	/// <returns>�����������������ǂ���</returns>
	bool Init(HINSTANCE _hInst);

	// �E�B���h�E�̑傫�����Đݒ�
	void OnResize(UINT width, UINT height);

	bool MessageLoop();

	// �E�B���h�E�̏I������
	bool Terminate(); 

	//���b�Z�[�W����������֐�
	static LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// �n���h���E�B���h�E���擾
	HWND GetWindowHandle() { return hWnd; }
};

