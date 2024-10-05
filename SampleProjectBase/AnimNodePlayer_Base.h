#pragma once
#include "SkeletalMesh.h"

#include "AnimationNode_Base.h"

/// @brief �m�[�h�Đ��̊��N���X
class AnimNodePlayer_Base
{
	/// @brief ���݂̍Đ�����
	float curPlayRatio;

	/// @brief 1�t���[���O�̍Đ�����
	float lastPlayRatio;

	/// @brief �m�[�h�̍Đ����x
	float progressNodeSpeed;
protected:
	/// @brief �O��̃��[�g���[�V�������W�l
	DirectX::SimpleMath::Vector3 p_RootMotionPos;

	/// @brief �O��̃��[�g���[�V������]��
	DirectX::SimpleMath::Quaternion p_RootMotionRot;

	/// @brief �������{�[��
	BoneList* pBoneList;

	/// @brief �Đ�����A�j���[�V�����m�[�h
	const AnimationNode_Base* pPlayAnimNode;

	/// @brief ���[�g���[�V�������Ɉړ�����I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform* pObjectTransform;
public:
	AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	virtual ~AnimNodePlayer_Base(){}

	/// @brief �X�V�����Ăяo��
	/// @param _controllerPlaySpeed �R���g���[���[���̂̍Đ����x
	virtual void UpdateCall(float _controllerPlaySpeed);

	/// @brief ���݂̍Đ��������Z�b�g
	/// @param �Đ�����
	void SetCurPlayRatio(float _playRatio);

	/// @brief ���݂̍Đ��������擾����
	/// @return ���݂̍Đ�����
	float GetCurPlayRatio() const;

private:
	/// @brief �Đ�������i�߂�
	/// @param _controllerPlaySpeed �R���g���[���[���̂̍Đ����x
	void ProgressPlayRatio(float _controllerPlaySpeed);

	/// @brief ���[�v�Đ��ł��邩�m�F
	/// @return ���[�v�ł��邩�H
	bool IsCanLoop() const;

	/// @brief �Đ������[�v�������̏���
	void OnPlayLoop();

	/// @brief �g�����X�t�H�[���Ƀ��[�g���[�V�����𔽉f����
	void ApplyRootMotionToTransform();
protected:
	/// @brief �e�m�[�h�v���C���[�̍X�V����
	virtual void Update() = 0;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @return ���݂̊����̃��[�g���[�V�������W
	virtual DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) = 0;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @return ���݂̊����̃��[�g���[�V������]��
	virtual DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio) = 0;
};

