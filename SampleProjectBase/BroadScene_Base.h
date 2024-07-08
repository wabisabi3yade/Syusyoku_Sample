#pragma once
#include "SceneMoveInfo.h"

class SubScene_Base;
class ChangeSubScene;

// �����̊֘A�����V�[�����W�܂�����ǃN���X(����ɂ���ĕ�����)
class BroadScene_Base
{
protected:
	SubScene_Base* pNowScene = nullptr;	// �����s���Ă���V�[���ϐ�
	ChangeSubScene* pChangeSubScene = nullptr;	// �V�[����ύX����N���X

	// �V�[���J�ڃN���X�̏�����������
	virtual void InitChangeScene(SceneMoveInfo* _moveInfo) = 0;

public:
	BroadScene_Base(SceneMoveInfo* _pMoveInfo);

	virtual ~BroadScene_Base();

	// �T�u�V�[����J�ڂ���
	SubScene_Base* ChangeSubScene();

	// �V�[���̎��s�֐�(�O������͂����ǂݍ���)
	// ������
	// �O���̓V�[���̒��g���ǂ��Ȃ��Ă邩�Ȃǒm��K�v���Ȃ�
	// �ɗ̓V���v���ȌĂяo���������s��
	void Exec(); 

	virtual void Release();
};

