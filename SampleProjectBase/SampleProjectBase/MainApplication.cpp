#include "MainApplication.h"
#include "SceneManager.h"
#include "ImGuiMethod.h"
#include "VariableFrameRate.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

constexpr short FPS(60);	// �t���[�����[�g��

std::unique_ptr<Window> MainApplication::pWindow = nullptr;	// �E�B���h�E�����N���X
Direct3D11* MainApplication::pD3D = nullptr;	// Direct3D�̋@�\�����N���X
SceneManager* MainApplication::pSceneManager = nullptr;	// �V�[���}�l�[�W���[�N���X
std::unique_ptr<VariableFrameRate> MainApplication::variableFps = nullptr;	// �σt���[�����[�g
std::unique_ptr<InputClass> MainApplication::input = nullptr;	// ���̓N���X

void MainApplication::Release()
{
	// �V���O���g���C���X�^���X�����
	Direct3D11::Delete();
	SceneManager::Delete();
	// ���������[�N�̕\���ɉf��Ȃ��悤�Ɏ蓮�ŉ��
	pWindow.reset();
	variableFps.reset();
	input.reset();

	// ImGui�̏I������
	ImGuiMethod::End();
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// �E�B���h�E�N���X�̊m��
	pWindow = std::make_unique<Window>();
	// �E�B���h�E�̏�����
	pWindow->Init(_hInst);

	// �σt���[�����[�g�N���X����
	variableFps = std::make_unique<VariableFrameRate>(FPS);
	input = std::make_unique<InputClass>();	// ����

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

		input->Update();	// ���͊֌W�̍X�V

#ifdef _DEBUG
		ImGui::Begin("System");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		GamePad()->DebugInput();
		ImGui::End();
#endif // _DEBUG

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
