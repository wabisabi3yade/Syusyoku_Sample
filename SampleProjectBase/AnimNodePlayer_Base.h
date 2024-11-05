#pragma once
#include "SkeletalMesh.h"

#include "AnimationNode_Base.h"
#include "AnimationNotify_Base.h"

class AnimationParameters;

/// @brief �m�[�h�Đ��̊��N���X
class AnimNodePlayer_Base : public HashiTaku::IImGuiUser
{
	/// @brief ���݂̍Đ�����
	float curPlayRatio;

	/// @brief 1�t���[���O�̃A�j���[�V��������
	float lastAnimationRatio;

	/// @brief �A�j���[�V��������(�A�j���[�V�����J�[�u�𔽉f������)
	float curAnimationRatio;

	/// @brief �m�[�h�̍Đ����x
	float playerSpeedTimes;

	/// @brief �v�Z�����S�Ă��l�������Đ����x
	float allPlaySpeed;

	/// @brief �Đ��t���O
	bool isPlay;

	/// @brief ���[�v�����^�C�~���O���H
	bool isJustLoop;

	/// @brief �A�j���[�V�����R���g���[�����̒ʒm�C�x���g����R�s�[���Ă����ʒm�C�x���g���X�g
	std::list<std::unique_ptr<AnimationNotify_Base>> copyNotifys;
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
	/// @brief �R���X�g���N�^
	/// @param _playNode �Đ��m�[�h
	/// @param _boneList �{�[���z��
	/// @param _transform �g�����X�t�H�[��
	AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	virtual ~AnimNodePlayer_Base(){}

	/// @brief �A�j���[�V�����R���g���[���[����ʒm�C�x���g���R�s�[����
	/// @param _notifyList �R�s�[���̒ʒm�C�x���g���X�g
	/// @param _animationParameters �A�j���[�V�����p�����[�^
	void CopyNotifys(const std::list<std::unique_ptr<AnimationNotify_Base>>& _notifyList, AnimationParameters& _animationParameters);

	/// @brief �X�V�����Ăяo��
	/// @param _controllerPlaySpeed �R���g���[���[���̂̍Đ����x
	void UpdateCall(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed);

	/// @brief �g�����X�t�H�[���Ƀ��[�g���[�V�����𔽉f����
	/// @return ���̃t���[���̈ړ���
	DirectX::SimpleMath::Vector3 CalcRootMotionToTransform();

	/// @brief �g�����X�t�H�[���ɂ��̃t���[���̃��[�g���[�V�������ړ�����
	/// @param _rootMovement ���[�g���[�V�����̈ړ���
	void ApplyRootMotion(const DirectX::SimpleMath::Vector3& _rootMovement);

	void OnInterpolateUpdate(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed);

	/// @brief �Đ�������i�߂�
	/// @param _controllerPlaySpeed �R���g���[���[���̂̍Đ����x
	void ProgressPlayRatio(float _controllerPlaySpeed);

	/// @brief �ύX�O�I������
	void OnTerminal();

	/// @brief ���݂̍Đ��������Z�b�g
	/// @param �Đ�����
	void SetCurPlayRatio(float _playRatio);

	/// @brief �Đ����x�{�����Z�b�g
	/// @param �Đ����x�{��
	void SetPlaySpeedTimes(float _playSpeed);

	/// @brief ���݂̍Đ��������擾����
	/// @return ���݂̍Đ�����
	float GetCurPlayRatio() const;

	/// @brief 1�t���[���O�̍Đ��������擾
	/// @return 1�t���[���O�̍Đ�����
	float GetLastAnimationRatio() const;

	/// @brief �A�j���[�V�����������擾
	/// @return �A�j���[�V��������
	float GetAnimationRatio() const;

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
	bool IsCanLoop();

	/// @brief �Đ������[�v�������̏���
	void OnPlayLoop();

	/// @brief �ʒm�C�x���g���X�V
	void NotifyUpdate();
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

