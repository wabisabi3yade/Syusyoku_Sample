#pragma once
#include "BroadSceneType.h"
#include "Camera.h"
#include "ModelLoader.h"

class SceneMoveInfo;

// �T�u�V�[���i�����ȃV�[���A�Ⴆ�ΊX�̒��̉Ƃ̒��Ƃ��j�̊��N���X
class SubScene_Base
{
	// �V�[���J�ڂ̏��iBroadScene����������j
	SceneMoveInfo* pSceneMoveInfo;

protected:
	ResourceCollection* resourceCollection;	// ���\�[�X�Ǘ��|�C���^
	std::unique_ptr<Camera> mainCamera;	// ���C���J�����I�u�W�F�N�g

	// �X�V����
	virtual void Update() = 0;

	// Update�̂��Ƃɍs����X�V����
	virtual void LateUpdate() = 0;

	// �`�������
	virtual void Draw() = 0;

	// �V�[���ړ��ŌĂяo��
	void OnMoveScene(int _nextSubType, BROAD_TYPE::TYPE _nextBroadType);
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
	virtual void Release() = 0;
};

