#include <Windows.h>
#include <crtdbg.h>
#include "MainApplication.h"
// �G���g���|�C���g
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �A�v��������
	MainApplication::Init(hInst);

	// ���[�v : �A�v�����C���̎����������Ƃ���
	MainApplication::GameLoop();

	// �A�v�����
	MainApplication::Release();

	return 0;
}