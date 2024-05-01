#pragma once
#include "BroadSceneType.h"
class BroadScene_Base;
class SceneMoveInfo;

// ��ǃV�[����ύX����N���X
class ChangeBroadScene
{
	BroadScene_Base* pBroadScene{nullptr};	// ��ǃV�[���ϐ�
	SceneMoveInfo* pMoveInfo{ nullptr };	// �V�[���J�ڏ��

	BROAD_TYPE::TYPE c_BroadId;	// ���݂̑�ǃV�[����id

public:
	ChangeBroadScene(SceneMoveInfo* _moveInfo);
	~ChangeBroadScene();

	// ��ǃV�[����ύX����
	BroadScene_Base* OnChangeBroad();

	// �������
	void Release();

};

