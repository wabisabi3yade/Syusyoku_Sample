#include "MainApplication.h"
#include "Direct3D11.h"
#include "SceneManager.h"
#include "Camera.h"

MainApplication::MainApplication()
{
}

MainApplication::~MainApplication()
{
	Release();
}

void MainApplication::Release()
{
	Direct3D11::Delete();
	SceneManager::Delete();

	CLASS_DELETE(pWindow);
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// �E�B���h�E�N���X�̊m��
	pWindow = new Window();
	// �E�B���h�E�̏�����
	pWindow->Init(_hInst);

	// Direct3D�N���X�̊m��
	pD3D = Direct3D11::GetInstance();
	HWND hwnd = pWindow->GetWindowHandle();
	// ������
	pD3D->Init(hwnd);
	// �V�[���}�l�[�W���[���쐬
	pSceneManager = SceneManager::GetInstance();
}

void MainApplication::GameLoop()
{
	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		// �X�V����
		pSceneManager->Exec();

		// ����
		Sleep(10);
	}
}