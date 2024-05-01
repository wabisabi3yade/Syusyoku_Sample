#pragma once
#include "BroadSceneType.h"
#include "SceneRunningState.h"


// �V�[���J�ڂɕK�v�ȏ��i�J�ڐ�A�J�ڂ��邩�ǂ����j�������N���X
class SceneMoveInfo
{
	SceneRunningState* pRunningState;	// �V�[���̉ғ���ԁiFINISH�őJ�ڂ���j
	BROAD_TYPE::TYPE broadType;	// �J�ڐ�̑�ǃV�[���^�C�v
	int subType;	// ���̃T�u�V�[���̃^�C�v

public:
	// �����F�ŏ��ɌĂ�	�@�T�u�V�[���@�A��ǃV�[��
	SceneMoveInfo(int _initSub, BROAD_TYPE::TYPE _initBroad);
	~SceneMoveInfo();

	// �V�[���J�ڂ���Ƃ��ɌĂяo�����
	void OnChangeScene(int _nextSub, BROAD_TYPE::TYPE _nextBroad);

	// �J�ڐ�̑�ǃV�[���̃^�C�v���擾
	BROAD_TYPE::TYPE GetNextBroadType()const { return broadType; }

	// �J�ڐ�̃T�u�V�[���̃^�C�v���擾
	int GetNextSubType()const { return subType; }

	// �V�[���̉ғ���Ԃ��擾
	int GetRunningState()const;

	// �V�[���J�ڌ�Ȃǂɏ���������֐�
	void Init();
};