#pragma once
#include "SceneMoveInfo.h"

// �V�X�e���֘A
#include "AssetLoader.h"
#include "ShaderCollection.h"
#include "InSceneSystemManager.h"

// �A�Z�b�g
#include "StaticMesh.h"
#include "SkeletalMesh.h"

class SceneObjects;
class CollisionChecker;

// �T�u�V�[���i�����ȃV�[���A�Ⴆ�ΊX�̒��̉Ƃ̒��Ƃ��j�̊��N���X
class SubScene_Base
{
	// �V�[���J�ڂ̏��iBroadScene����������j
	SceneMoveInfo* pSceneMoveInfo;

protected:
	//	�V�[�����̃V�X�e���}�l�W���[
	InSceneSystemManager* systemManager;

	// �X�V����
	virtual void Update() = 0;

	// Update�̂��Ƃɍs����X�V����
	virtual void LateUpdate() = 0;

	// �`�������
	virtual void Draw() = 0;
	void DrawSetup();

	// �V�[���ړ��ŌĂяo��
	void OnMoveScene(int _nextSubType, BroadType::Type _nextBroadType);
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_pSceneMoveInfo">��ǃN���X����󂯎�����V�[���J�ڏ��</param>
	SubScene_Base(SceneMoveInfo* _pSceneMoveInfo);
	virtual ~SubScene_Base();

	// �V�[�����s����
	void Exec();

	// �������
	virtual void Release() {};
};

