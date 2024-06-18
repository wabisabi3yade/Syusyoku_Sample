#include "MainApplication.h"
#include "SceneManager.h"
#include "Camera.h"
#include "ImGuiMethod.h"
#include "VariableFrameRate.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

constexpr short FPS(60);	// �t���[�����[�g��

MainApplication::MainApplication()
{
	// �σt���[�����[�g�N���X����
	variableFps = std::make_unique<VariableFrameRate>(FPS);
	input = std::make_unique<InputClass>();
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

#ifdef _DEBUG
	ImGuiIO& io = ImGui::GetIO();
#endif // _DEBUG


	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		// ImGui�̍X�V����
		ImGuiMethod::NewFrame();

#ifdef _DEBUG
		ImGui::Begin("System");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
#endif // _DEBUG

		input->Update();	// ���͊֌W�̍X�V

		// �X�V����
		pSceneManager->Exec();

		// �Ή������t���[�����[�g�ɂ��邽�߂Ɏ��Ԃ�ҋ@������
		variableFps->Wait();
	}
}

float MainApplication::DeltaTime()
{
	return variableFps->GetDeltaTime();
}
