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
bool MainApplication::isEscapeDisplay = false;

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

bool MainApplication::Escape()
{
	// �G�X�P�[�v�L�[�����ꂽ��
	if (!isEscapeDisplay && input->GetKeyboard().GetKeyDown(DIK_ESCAPE))
		isEscapeDisplay = true;

	if (!isEscapeDisplay) return false;

	bool isEscape = false;

	// �A�v���P�[�V�����I���\��

	// �E�B���h�E�傫���ݒ�
	ImVec2 windowSize(170, 70);
	ImVec2 windowPos(
		(ImGui::GetIO().DisplaySize.x - windowSize.x) * 0.5f,
		(ImGui::GetIO().DisplaySize.y - windowSize.y) * 0.5f
	);
	ImGui::SetNextWindowPos(windowPos);
	ImGui::SetNextWindowSize(windowSize);

	// �{�^������
	ImGui::Begin(ShiftJisToUtf8("�I�����܂���").c_str(), nullptr, 
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::Button(ShiftJisToUtf8("�͂�").c_str()))
	{
		isEscape = true;
		isEscapeDisplay = false;
	}
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(30.0f, 0.0f));
	ImGui::SameLine();
	if (ImGui::Button(ShiftJisToUtf8("������").c_str()))
	{
		isEscapeDisplay = false;
	}
	ImGui::End();

	return isEscape;
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
		static ULONGLONG start = 0;
		static ULONGLONG end = 0;
		start = GetTickCount64();

		bool result = pWindow->MessageLoop();
		if (result == false) break;

		ImGuiMethod::NewFrame();

		// �I�������`�F�b�N
		if (Escape())
			return;

		// ���͍X�V
		input->Update();

		// �X�V����
		pSceneManager->Exec();

		ImGuiMethod::Draw();

		// �X���b�v�`�F�C��
		Direct3D11::GetInstance()->GetRenderer()->Swap();
		
		ULONGLONG b = 0;
		
		end = GetTickCount64();
		ULONGLONG delta1 = end - start;

		// �ҋ@
		variableFps->Wait();

		
		end = GetTickCount64();

		ULONGLONG delta = end - start;
	}
}

float MainApplication::DeltaTime()
{
	return variableFps->GetDeltaTime();
}
