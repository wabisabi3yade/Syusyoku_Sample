#include "MainApplication.h"
#include "SceneManager.h"
#include "ImGuiMethod.h"
#include "VariableFrameRate.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"


#ifdef _DEBUG
#define _EDIT
#endif

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

#ifdef _DEBUG
	// ���������[�N�̕\���ɉf��Ȃ��悤�Ɏ蓮�ŉ��
	pWindow.reset();
	variableFps.reset();
	input.reset();
#endif // DEBUG

	// ImGui�̏I������
	ImGuiMethod::End();
}

void MainApplication::Init(HINSTANCE _hInst)
{
	// �E�B���h�E�N���X�̊m��
	pWindow = std::make_unique<Window>();
	pWindow->Init(_hInst);

	// Direct3D�N���X�̊m��
	pD3D = Direct3D11::GetInstance();
	HWND hwnd = pWindow->GetWindowHandle();
	pD3D->Init(hwnd);

	// �σt���[�����[�g�N���X����
	variableFps = std::make_unique<VariableFrameRate>(FPS);

	// ���̓N���X���쐬
	input = std::make_unique<InputClass>();
	input->Init(hwnd);

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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	while (true)
	{
		bool result = pWindow->MessageLoop();
		if (result == false) break;

		ImGuiMethod::NewFrame();
		// ���͍X�V
		input->Update();

		// �X�V����
		pSceneManager->Exec();

#ifdef _DEBUG
		ImGui::Begin("System");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
#endif // _DEBUG

		ImGuiMethod::Draw();
		// �X���b�v�`�F�C��
		Direct3D11::GetInstance()->GetRenderer()->Swap();

		// deltaTime���v�Z
		variableFps->CaluculateDelta();
		// �ҋ@
		variableFps->Wait();
	}
}

float MainApplication::DeltaTime()
{
	return variableFps->GetDeltaTime();
}
