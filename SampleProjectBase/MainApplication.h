#pragma once

#include "Singleton_Base.h"
#include "Window.h"
#include "InputClass.h"
#include "VariableFrameRate.h"

class SceneManager;
class Direct3D11;



// �Q�[�����[�v�ȂǃA�v���𓮂����̂Ɏ�v�ȋ@�\�����N���X(�V���O���g���p�^�[��)
class MainApplication
{
	static std::unique_ptr<Window> pWindow;	// �E�B���h�E�����N���X
	static Direct3D11* pD3D;	// Direct3D�̋@�\�����N���X
	static SceneManager* pSceneManager;	// �V�[���}�l�[�W���[�N���X
	static std::unique_ptr<VariableFrameRate> variableFps;	// �σt���[�����[�g�N���X
	static std::unique_ptr<InputClass> input;	// ���̓N���X

	// �A�v���P�[�V�����I��
	static bool isEscapeDisplay;

	MainApplication(){};
	~MainApplication(){};

	static bool Escape();
public:
	// ����������
	// �����F�C���X�^���X�n���h��
	static void Init(HINSTANCE _hInst);

	// �Q�[�����[�v�̏���������
	static void GameLoop();

	static void Release();

	static float DeltaTime();

	static InputClass& GetInput() { return *input; }
};

