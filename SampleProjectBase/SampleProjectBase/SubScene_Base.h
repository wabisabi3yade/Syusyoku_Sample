#pragma once
#include "SceneMoveInfo.h"

// �I�u�W�F�N�g
#include "Camera.h"
#include "Object_3D.h"
#include "Object_2D.h"
#include "Object_UI.h"

// �V�X�e���֘A
#include "SceneObjects.h"
#include "SceneLights.h"
#include "CollisionChecker.h"
#include "ModelLoader.h"
#include "ShaderCollection.h"

// �T�u�V�[���i�����ȃV�[���A�Ⴆ�ΊX�̒��̉Ƃ̒��Ƃ��j�̊��N���X
class SubScene_Base
{
	// �V�[���J�ڂ̏��iBroadScene����������j
	SceneMoveInfo* pSceneMoveInfo;

protected:
	// ���\�[�X�Ǘ�
	ResourceCollection* resourceCollection;	

	// �V�[�����I�u�W�F�N�g�Ǘ�
	std::unique_ptr<SceneObjects> sceneObjects;

	// �V�[��������
	std::unique_ptr<SceneLights> sceneLights;

	// �����蔻��̃`�F�b�N
	std::unique_ptr<CollisionChecker> collisionChecker;	

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

