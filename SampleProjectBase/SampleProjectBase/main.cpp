#include <Windows.h>
#include <crtdbg.h>
#include "MainApplication.h"
// �G���g���|�C���g
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	// �A�v��������
	MainApplication::Init(hInst);

	//// ���[�v : �A�v�����C���̎����������Ƃ���
	MainApplication::GameLoop();

	// �A�v�����
	MainApplication::Release();

	// ���������[�N���o
	_CrtDumpMemoryLeaks();
	return 0;
}