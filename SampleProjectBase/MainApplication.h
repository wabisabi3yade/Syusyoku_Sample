#pragma once

#include "Singleton_Base.h"
#include "Window.h"
#include "InputClass.h"
#include "VariableFrameRate.h"
#include "AppSystemDraw.h"

class SceneManager;
class Direct3D11;
class AssetCollection;

// �Q�[�����[�v�ȂǃA�v���𓮂����̂Ɏ�v�ȋ@�\�����N���X(�V���O���g���p�^�[��)
class MainApplication
{
	/// @brief �E�B���h�E����
	static std::unique_ptr<Window> pWindow;

	/// @brief Direct3D�̋@�\
	static Direct3D11* pD3D;

	/// @brief �V�[���}�l�[�W���[
	static SceneManager* pSceneManager;

	/// @brief �σt���[�����[�g�N���X
	static std::unique_ptr<VariableFrameRate> pVariableFps;

	/// @brief ���͊Ǘ�
	static std::unique_ptr<InputClass> pInput;

	/// @brief �A�Z�b�g�̊Ǘ�
	static std::unique_ptr<AssetCollection> pAssetCollection;

	/// @brief �A�v���P�[�V�����I�����邩�m�F��
	static bool isEscapeChecking;

	MainApplication() {};
	~MainApplication() {};

	/// @brief �A�v���P�[�V�����I���m�F
	/// @return �I���t���O
	static bool EscapeCheck();

	/// @brief �V�X�e���֘A�̕`��
	static void SystemDraw();

public:

	/// @brief ������
	/// @param _hInst �n���h���C���X�^���X 
	static void Init(HINSTANCE _hInst);

	/// @brief �Q�[�����[�v
	static void GameLoop();

	/// @brief �������
	static void Release();

	/// @brief ��t��Ԃ�
	/// @return ��t(s)
	static float DeltaTime();

	/// @brief ���̓N���X��Ԃ�
	/// @return 
	static InputClass& GetInput() { return *pInput; }

private:
	// �Z�b�g�A�b�v�֐�

	/// @brief �E�B���h�E������
	/// @param _hInst �n���h���C���X�^���X
	static void WindowSetup(HINSTANCE _hInst);

	/// @brief Direct3D�̏�����
	static void D3DSetup(HWND _hwnd);

	/// @brief �σt���[�����[�g������
	static void VariableFrameSetup();

	/// @brief ���͏�����
	static void InputSetup(HWND _hwnd);

	/// @brief ImGui������
	static void ImGuiSetup();

	/// @brief �A�Z�b�g�֌W�̃V�X�e���̏�����
	static void AssetSysytemSetup();

	/// @brief �V�F�[�_�[�̏�����
	static void ShaderSetup();

	/// @brief �V�[���}�l�[�W���[������
	static void SceneManagerSetup();

	/// @brief Update�ł��邩�擾����
	/// @return Update�ł��邩�H
	static bool CanUpdate();
};

