#pragma once

#include "Singleton_Base.h"
#include "Window.h"
#include "InputClass.h"

// ����
class Camera;

class SceneManager;
class Direct3D11;
class VariableFrameRate;

// �Q�[�����[�v�ȂǃA�v���𓮂����̂Ɏ�v�ȋ@�\�����N���X(�V���O���g���p�^�[��)
class MainApplication
	: public Singleton_Base<MainApplication>
{
	// ���N���X�ɃR���X�g���N�^��n�����߂�
	friend class Singleton_Base<MainApplication>;

	std::unique_ptr<Window> pWindow;	// �E�B���h�E�����N���X
	Direct3D11* pD3D;	// Direct3D�̋@�\�����N���X
	SceneManager* pSceneManager;	// �V�[���}�l�[�W���[�N���X
	std::unique_ptr<VariableFrameRate> variableFps;	// �σt���[�����[�g�N���X
	std::unique_ptr<InputClass> input;	// ���̓N���X

	MainApplication();
	~MainApplication();

	void Release();
public:
	// ����������
	// �����F�C���X�^���X�n���h��
	void Init(HINSTANCE _hInst);

	// �Q�[�����[�v�̏���������
	void GameLoop();

	float DeltaTime();	// �O�t���[���Ƃ̎���

	GamePad* GamePad()const { return input->GetGamePad(); }	// �Q�[���p�b�h�擾
};

