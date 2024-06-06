#include "MainApplication.h"
#include "Direct3D11.h"
#include "SceneManager.h"
#include "Camera.h"
#include "ImGuiMethod.h"
#include "VariableFrameRate.h"

constexpr short FPS(60);	// �t���[�����[�g��

MainApplication::MainApplication()
{
	// �σt���[�����[�g�N���X����
	variableFps = std::make_unique<VariableFrameRate>(FPS);
}

MainApplication::~MainApplication()
{
	Release();
}

void MainApplication::Release()
{
	// �V���O���g���C���X�^���X�����
	Direct3D11::Delete();
	SceneManager::Delete();
	ResourceCollection::Delete();

	// ImGui�̏I������
	ImGuiMethod::End();
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// �E�B���h�E�N���X�̊m��
	pWindow = std::make_unique<Window>();
	// �E�B���h�E�̏�����
	pWindow->Init(_hInst);

	// Direct3D�N���X�̊m��
	pD3D = Direct3D11::GetInstance();
	HWND hwnd = pWindow->GetWindowHandle();
	// ������
	pD3D->Init(hwnd);
	// �V�[���}�l�[�W���[���쐬
	pSceneManager = SceneManager::GetInstance();

	// ImGui�̏�����
	D3D11_Renderer* pRenderer = pD3D->GetRenderer();
	ImGuiMethod::Initialize(
		pWindow->GetWindowHandle(),
		pRenderer->GetDevice(),
		pRenderer->GetDeviceContext()
	);
}

void MainApplication::GameLoop()
{
	variableFps->Init();

	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		// ImGui�̍X�V����
		ImGuiMethod::NewFrame();

		// �X�V����
		pSceneManager->Exec();

		// �Ή������t���[�����[�g�ɂ��邽�߂Ɏ��Ԃ�ҋ@������
		variableFps->Wait();
	}
}