#include <Windows.h>
#include <crtdbg.h>
#include "MainApplication.h"

// �G���g���|�C���g
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �A�v��������
	HashiTaku::MainApplication::Init(hInst);

	// ���[�v : �A�v�����C���̎����������Ƃ���
	HashiTaku::MainApplication::GameLoop();

	// �A�v�����
	HashiTaku::MainApplication::Release();

	return 0;
}