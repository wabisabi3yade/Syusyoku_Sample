#include "MainApplication.h"

// �V�X�e���֘A
#include "SceneManager.h"
#include "AssetCollection.h"
#include "ShaderCollection.h"

// Asset�֘A
#include "AssetContacter.h"
#include "AssetDisplay.h"

// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

constexpr u_int FPS(60);	// �t���[�����[�g��

// static�ϐ�������
std::unique_ptr<Window> MainApplication::pWindow = nullptr;
Direct3D11* MainApplication::pD3D = nullptr;
SceneManager* MainApplication::pSceneManager = nullptr;
std::unique_ptr<VariableFrameRate> MainApplication::pVariableFps = nullptr;
std::unique_ptr<InputClass> MainApplication::pInput = nullptr;
std::unique_ptr<AssetCollection> MainApplication::pAssetCollection = nullptr;
bool MainApplication::isEscapeDisplay = false;

void MainApplication::Release()
{
	// ImGui�̏I������
	ImGuiMethod::Terminal();

	// �V���O���g���C���X�^���X�����
	SceneManager::Delete();
	ShaderCollection::Delete();
	Direct3D11::Delete();
}

bool MainApplication::EscapeCheck()
{
	// �G�X�P�[�v�L�[�����ꂽ��
	if (!isEscapeDisplay && pInput->GetKeyboard().GetKeyDown(DIK_ESCAPE))
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

void MainApplication::SystemDraw()
{
	ImGui::Begin(ShiftJisToUtf8("�V�X�e��").c_str());
	AssetDisplay::Draw();
	pVariableFps->Draw();
	ImGui::End();

	ImGuiMethod::Draw();
}

void MainApplication::Init(HINSTANCE _hInst)
{
	WindowSetup(_hInst);

	HWND hwnd = pWindow->GetWindowHandle();

	D3DSetup(hwnd);

	VariableFrameSetup();

	InputSetup(hwnd);

	AssetSysytemSetup();

	ShaderSetup();

	ImuiSetup();

	SceneManagerSetup();
}

void MainApplication::GameLoop()
{
	// �O�񎞊Ԃ̏�����
	pVariableFps->Init(FPS);

	while (true)
	{
		// 1�t���[���̎��Ԃɍ��킹��
		if (!CanUpdate()) continue;

		bool result = pWindow->MessageLoop();
		if (result == false) break;

		ImGuiMethod::NewFrame();

		// �I�������`�F�b�N
		if (EscapeCheck())
			return;

		// ���͍X�V
		pInput->Update();

		// �X�V����
		pSceneManager->Exec();

		// �V�X�e����GUI�`��
		SystemDraw();

		// �X���b�v�`�F�C��
		Direct3D11::GetInstance()->GetRenderer()->Swap();
	}
}

float MainApplication::DeltaTime()
{
	return pVariableFps->GetDeltaTime();
}

void MainApplication::WindowSetup(HINSTANCE _hInst)
{
	// �E�B���h�E�N���X�̊m��
	pWindow = std::make_unique<Window>();
	pWindow->Init(_hInst);
}

void MainApplication::D3DSetup(HWND _hwnd)
{
	// Direct3D�N���X�̊m��
	pD3D = Direct3D11::GetInstance();
	pD3D->Init(_hwnd);
}

void MainApplication::VariableFrameSetup()
{
	pVariableFps = std::make_unique<VariableFrameRate>();
}

void MainApplication::InputSetup(HWND _hwnd)
{
	// ���̓N���X���쐬
	pInput = std::make_unique<InputClass>();
	pInput->Init(_hwnd);
}

void MainApplication::ImuiSetup()
{
	// ImGui�̏�����
	D3D11_Renderer* pRenderer = pD3D->GetRenderer();
	ImGuiMethod::Initialize(
		pWindow->GetWindowHandle(),
		pRenderer->GetDevice(),
		pRenderer->GetDeviceContext()
	);
}

void MainApplication::AssetSysytemSetup()
{
	pAssetCollection = std::make_unique<AssetCollection>();

	// �A�Z�b�g�A���C���^�[�t�F�[�X�ɊǗ��N���X�̎Q�Ƃ�n��
	AssetContacter::SetAssetCollection(*pAssetCollection.get());
}

void MainApplication::ShaderSetup()
{
	ShaderCollection* shCol = ShaderCollection::GetInstance();
	shCol->Init();
}

void MainApplication::SceneManagerSetup()
{
	pSceneManager = SceneManager::GetInstance();
}

bool MainApplication::CanUpdate()
{
	return pVariableFps->UpdateCheck();
}
