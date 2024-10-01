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
	/// @brief �������{�[��
	BoneList* pBoneList;

	/// @brief �Đ�����A�j���[�V�����m�[�h
	const AnimationNode_Base* pPlayAnimNode;
public:
	AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList);
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

protected:
	/// @brief �e�m�[�h�v���C���[�̍X�V����
	virtual void Update() = 0;
};

