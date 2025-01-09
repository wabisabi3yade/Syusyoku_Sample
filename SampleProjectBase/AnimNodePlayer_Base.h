#pragma once
#include "SkeletalMesh.h"

#include "AnimationNode_Base.h"
#include "AnimationNotify_Base.h"

namespace HashiTaku
{
	class AnimationParameters;

	/// @brief �m�[�h�Đ��̊��N���X
	class AnimNodePlayer_Base : public IImGuiUser
	{
		/// @brief ���݂̍Đ�����
		float curPlayRatio;

		/// @brief 1�t���[���O�̍Đ�����
		float lastPlayRatio;

		/// @brief �m�[�h�̍Đ����x
		float playerSpeedTimes;

		/// @brief �Đ��t���O
		bool isPlaying;

		/// @brief ���[�v�����^�C�~���O���H
		bool isJustLoop;

		/// @brief �A�j���[�V�����R���g���[�����̒ʒm�C�x���g����R�s�[���Ă����ʒm�C�x���g���X�g
		std::list<std::unique_ptr<AnimationNotify_Base>> copyNotifys;
	protected:
		/// @brief ���[�g���[�V�����ɂ����W�ړ����x
		DXSimp::Vector3 rootMotionPosSpeedPerSec;

		/// @brief �O��̃��[�g���[�V�������W�l
		DXSimp::Vector3 p_RootMotionPos;

		/// @brief �O��̃��[�g���[�V������]��
		DXSimp::Quaternion p_RootMotionRot;

		/// @brief �������{�[��
		BoneList* pAssetBoneList;

		/// @brief �Đ�����A�j���[�V�����m�[�h
		const AnimationNode_Base* pPlayAnimNode;

		/// @brief ���[�g���[�V�������Ɉړ�����I�u�W�F�N�g�̃g�����X�t�H�[��
		Transform* pObjectTransform;

		/// @brief �v�Z�����S�Ă��l�������Đ����x
		float allPlaySpeed;

		/// @brief �o�ߎ���
		float deltaTime;
	public:
		/// @brief �R���X�g���N�^
		/// @param _playNode �Đ��m�[�h
		/// @param _boneList �{�[���z��
		/// @param _transform �g�����X�t�H�[��
		AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
		virtual ~AnimNodePlayer_Base() {}

		/// @brief �A�j���[�V�����R���g���[���[����ʒm�C�x���g���R�s�[����
		/// @param _notifyList �R�s�[���̒ʒm�C�x���g���X�g
		/// @param _animationParameters �A�j���[�V�����p�����[�^
		/// @param _transform �g�����X�t�H�[�����
		void CopyNotifys(const std::list<std::unique_ptr<AnimationNotify_Base>>& _notifyList, 
			AnimationParameters& _animationParameters, 
			Transform& _transform);

		/// @brief �X�V�����Ăяo��
		/// @param _deltaTime �o�ߎ���
		/// @param _controllerSpeed �R���g���[���[���x
		void UpdateCall(std::vector<BoneTransform>& _outTransforms, float _deltaTime, float _controllerSpeed);

		/// @brief �g�����X�t�H�[���Ƀ��[�g���[�V�����𔽉f����
		/// @return ���̃t���[���̈ړ���
		DXSimp::Vector3 CalcRootMotionToTransform();

		/// @brief �g�����X�t�H�[���ɂ��̃t���[���̃��[�g���[�V�������ړ�����
		/// @param _rootMovement ���[�g���[�V�����̈ړ���
		/// @param _deltaTime �o�ߎ���
		void ApplyRootMotion(const DXSimp::Vector3& _rootMovement);

		void OnInterpolateUpdate(std::vector<BoneTransform>& _outTransforms, float _deltaTime, float _controllerSpeed);

		/// @brief �Đ�������i�߂�
		virtual void ProgressPlayRatio(float _controllerSpeed);

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
		float GetLastPlayRatio() const;

		/// @brief �m�[�h�Đ����x���擾
		/// @return �m�[�h�Đ����x
		float GetNodePlaySpeed() const;

		/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
		/// @param ���݂̍Đ�����
		/// @param _isWorldScaling ���[���h���ɑΉ�����X�P�[�����O���邩�H
		/// @return ���݂̊����̃��[�g���[�V�������W
		virtual DXSimp::Vector3 GetRootMotionPos(float _ratio, bool _isWorldScaling = true) const = 0;

		/// @brief ���[�g���[�V�������擾����i�����ŕK�v�Ȍv�Z���s���j
		/// @param ���݂̍Đ�����
		/// @param _isWorldScaling ���[���h���ɑΉ�����X�P�[�����O���邩�H
		/// @return ���݂̊����̃��[�g���[�V�������W
		void GetCurrentRootPos(DXSimp::Vector3& _outPos, bool _isWorldScaling) const;

		/// @brief �Đ����Ă���m�[�h�����擾
		/// @return �m�[�h��
		const std::string& GetNodeName() const;

		const DXSimp::Vector3& GetRootMotionSpeed() const;
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
		virtual void CalcRootMotionPosSpeed() = 0;

		/// @brief ���[�g���[�V���������[�h���̃X�P�[���E��]�ʂ𔽉f
		/// @param _rootMotionPos ���f�����������[�g���[�V����
		void ApplyLoadTransform(DXSimp::Vector3& _rootMotionPos) const;

		/// @brief �v���C���[�Đ����x���擾����
		/// @return �v���C���[�̍Đ����x
		float GetPlayerSpeed() const;

		void ImGuiDebug() override;
	};
}