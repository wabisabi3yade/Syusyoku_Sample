#include <Windows.h>
#include <crtdbg.h>
#include "MainApplication.h"

// �G���g���|�C���g
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
	// �E�B���h�E��\������@�\�N���X���擾
	MainApplication* application;
	application = MainApplication::GetInstance();

	// �A�v��������
	application->Init(hInst);

	//// ���[�v : �A�v�����C���̎����������Ƃ���
	application->GameLoop();

	// �A�v�����
	MainApplication::Delete();

	// ���������[�N���o
	_CrtDumpMemoryLeaks();
	return 0;
}