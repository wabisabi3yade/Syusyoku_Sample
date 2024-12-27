#include "MainApplication.h"

// �V�X�e���֘A
#include "SceneManager.h"
#include "AssetCollection.h"
#include "ShaderCollection.h"
#include "DX11BulletPhisics.h"
#include "DXSoundManager.h"

// Asset�֘A
#include "AssetContacter.h"
#include "AssetDisplay.h"
#include "AssetSaveLoad.h"

namespace HashiTaku
{
	constexpr u_int FPS(60);	// �t���[�����[�g��

	// static�ϐ�������
	std::unique_ptr<Window> MainApplication::pWindow = nullptr;
	Direct3D11* MainApplication::pD3D = nullptr;
	SceneManager* MainApplication::pSceneManager = nullptr;
	std::unique_ptr<VariableFrameRate> MainApplication::pVariableFps = nullptr;
	std::unique_ptr<InputClass> MainApplication::pInput = nullptr;
	std::unique_ptr<AssetCollection> MainApplication::pAssetCollection = nullptr;
	bool MainApplication::isEscapeChecking = false;
	bool MainApplication::isExit = false;

	void MainApplication::Release()
	{
		// �V�X�e���`��I��
		AppSystemDraw::Delete();

		// �A�Z�b�g�̃Z�[�u���s��
		AssetSaveLoader::Save();

		// ImGui�̏I������
		ImGuiMethod::Terminal();

		// �A�Z�b�g�Ǘ����
		pAssetCollection.reset();

		// �V���O���g���C���X�^���X�����
		SceneManager::Delete();
		DXSoundManager::Delete();
		DX11EffecseerManager::Delete();
		ShaderCollection::Delete();
		Direct3D11::Delete();
	}

	bool MainApplication::EscapeCheck()
	{
		// �G�X�P�[�v�L�[�����ꂽ��
		if (pInput->GetKeyboard().GetKeyDown(DIK_ESCAPE))
			isEscapeChecking = true;

		if (!isEscapeChecking) return false;

#if _DEBUG
		// �I�����邩���b�Z�[�W�{�b�N�X���o��
		int id = 0;
		id = MessageBox(NULL, TEXT("�Q�[�����I�����܂����H"),
			TEXT("�m�F"),
			MB_YESNO | MB_ICONQUESTION);

		if (id == IDYES)
		{
			isEscapeChecking = false;
			return true;
		}
		else if (id == IDNO)
		{
			isEscapeChecking = false;
		}

#else
		return true;
#endif // _DEBUG

		return false;
	}

	void MainApplication::SystemDraw()
	{
		ImGuiMethod::Draw();
	}

	void MainApplication::Init(HINSTANCE _hInst)
	{
		WindowSetup(_hInst);

		HWND hwnd = pWindow->GetWindowHandle();

		D3DSetup(hwnd);

		VariableFrameSetup();

		InputSetup(hwnd);

		ShaderSetup();

		// �����N���X������
		Random::Init();

		// �C�[�W���O������
		Easing::Init();

		// �G�t�F�N�V�A������
		DX11EffecseerManager::GetInstance()->Init();

		// �T�E���h������
		DXSoundManager::GetInstance()->Init();

		AssetSysytemSetup();

		ShaderCollection::GetInstance()->InitAssets();

		ImGuiSetup();

		SceneManagerSetup();
	}

	void MainApplication::ExitRequest()
	{
		isExit = true;
	}

	void MainApplication::GameLoop()
	{
		// �O�񎞊Ԃ̏�����
		pVariableFps->Init(FPS);

		while (true)
		{
			//// 1�t���[���̎��Ԃɍ��킹��
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

			pVariableFps->ProcessEnd();

			if (isExit)
				return;
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

	void MainApplication::ImGuiSetup()
	{
#ifdef EDIT
		// ImGui�̏�����
		D3D11_Renderer* pRenderer = pD3D->GetRenderer();
		ImGuiMethod::Initialize(
			pWindow->GetWindowHandle(),
			pRenderer->GetDevice(),
			pRenderer->GetDeviceContext()
		);

		AppSystemDraw::GetInstance()->Init(*pVariableFps);
#endif // EDIT
	}

	void MainApplication::AssetSysytemSetup()
	{
		pAssetCollection = std::make_unique<AssetCollection>();

		// �A�Z�b�g�A���C���^�[�t�F�[�X�ɊǗ��N���X�̎Q�Ƃ�n��
		AssetContacter::SetAssetCollection(*pAssetCollection.get());

		// �A�Z�b�g���[�h����
		AssetSaveLoader::Load();
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
}