#pragma once
#include "SkeletalMesh.h"

#include "AnimationNode_Base.h"

/// @brief �m�[�h�Đ��̊��N���X
class AnimNodePlayer_Base : public HashiTaku::IImGuiUser
{
	/// @brief ���݂̍Đ�����
	float curPlayRatio;

	/// @brief 1�t���[���O�̍Đ�����
	float lastPlayRatio;

	/// @brief �m�[�h�̍Đ����x
	float playNodeSpeedTimes;

	/// @brief ���[�v�����^�C�~���O���H
	bool isJustLoop;
protected:
	/// @brief ���[�g���[�V�����ɂ����W�ړ����x
	DirectX::SimpleMath::Vector3 rootMotionPosSpeedPerSec;

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
	void UpdateCall(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed);

	/// @brief �Đ�������i�߂�
	/// @param _controllerPlaySpeed �R���g���[���[���̂̍Đ����x
	void ProgressPlayRatio(float _controllerPlaySpeed);

	/// @brief ���݂̍Đ��������Z�b�g
	/// @param �Đ�����
	void SetCurPlayRatio(float _playRatio);

	/// @brief �Đ����x�{�����Z�b�g
	/// @param �Đ����x�{��
	void SetPlaySpeedTimes(float _playSpeed);

	/// @brief �S�Ă̍Đ����x�����߂�
	/// @param _controllerSpeed �R���g���[���[�Đ����x
	/// @return �Đ����x
	float CalcPlaySpeed(float _controllerSpeed) const;

	/// @brief ���݂̍Đ��������擾����
	/// @return ���݂̍Đ�����
	float GetCurPlayRatio() const;

	/// @brief 1�t���[���O�̍Đ��������擾
	/// @return 1�t���[���O�̍Đ�����
	float GetLastPlayRatio() const;

	/// @brief �m�[�h�Đ����x���擾
	/// @return �m�[�h�Đ����x
	float GetNodePlaySpeed() const;

	/// @brief ���[�g���[�V�����̑O�񂩂�̍����ړ��l
	/// @param _outPos ����
	void GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @param _isLoadScaling ���[�h���̃X�P�[���𔽉f���邩
	/// @return ���݂̊����̃��[�g���[�V�������W
	void GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const;

	/// @brief �Đ����Ă���m�[�h�����擾
	/// @return �m�[�h��
	const std::string& GetNodeName() const;

	const DirectX::SimpleMath::Vector3& GetRootMotionSpeed() const;
private:
	/// @brief ���[�v�Đ��ł��邩�m�F
	/// @return ���[�v�ł��邩�H
	bool IsCanLoop() const;

	/// @brief �Đ������[�v�������̏���
	void OnPlayLoop();

	/// @brief �g�����X�t�H�[���Ƀ��[�g���[�V�����𔽉f����
	void ApplyRootMotionToTransform();
protected:
	/// @brief �e�m�[�h�v���C���[�̍X�V����
	virtual void Update(std::vector<BoneTransform>& _outTransforms) = 0;

	/// @brief ���[�g���[�V�����̍��W�ړ����x���v�Z����
	/// @param _controllerSpeed �R���g���[���[���x
	/// @return ���W�ړ����x
	virtual void CalcRootMotionPosSpeed(float _controllerSpeed) = 0;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @param _isLoadScaling ���[�h���̃X�P�[���𔽉f���邩
	/// @return ���݂̊����̃��[�g���[�V�������W
	virtual DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio, bool _isLoadScaling = true) const = 0;

	/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
	/// @param ���݂̍Đ�����
	/// @param _isLoadScaling ���[�h���̃X�P�[���𔽉f���邩
	/// @return ���݂̊����̃��[�g���[�V������]��
	virtual DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio, bool _isLoadScaling = true) const = 0;

	/// @brief ���[�g���[�V���������[�h���̃X�P�[���E��]�ʂ𔽉f
	/// @param _rootMotionPos ���f�����������[�g���[�V����
	void ApplyLoadTransform(DirectX::SimpleMath::Vector3& _rootMotionPos) const;

	void ImGuiSetting() override;
};

