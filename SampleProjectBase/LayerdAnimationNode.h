#pragma once
#include "AnimationNode_Base.h"

namespace HashiTaku
{
	class TreeNode;

	// ����㔼�g�A�����g�̍Đ�����������̂�2�����Ή����܂��A�K�v�ɂȂ�����g��

	/// @brief ���ʂ��Ƃɕ����ăA�j���[�V��������m�[�h
	class LayerdAnimationNode : public AnimationNode_Base
	{
		/// @brief �u�����h���̃X�s�[�h�J�[�u��
		AnimationCurve blendSpeedCurve;

		/// @brief �x�[�X���̃A�j���[�V�����𔽉f����{�[����Id
		std::vector<int> baseAnimBoneIds;

		/// @brief �u�����h���̃A�j���[�V�����𔽉f����{�[����Id
		std::vector<int> blendAnimBoneIds;

		/// @brief �x�[�X�ƂȂ�A�j���[�V����(���[�g�{�[���͂�����)
		AnimationData* pBaseAnimation;

		/// @brief �u�����h����A�j���[�V����
		AnimationData* pBlendAnimation;

		/// @brief �{�[�����X�g
		const BoneList* pBoneList;

		/// @brief �u�����h���̃A�j���[�V�������ǂ�����Đ����邩
		u_int beginBlendPlayFrame;

		/// @brief �u�����h�J�n�{�[������̃u�����h����{�[���̐[��(2�Ȃ�J�n�{�[������[��2�܂ł̃{�[����0.5���u�����h����)
		u_int blendDepth;

		/// @brief �ǂ�����u�����h�A�j���[�V�������Đ����邩�w�肷��{�[��ID
		int beginBlendBoneId;

		/// @brief ���b�V���̃��[�J����ԓ��ŉ�]�ʂ��u�����h
		bool isMeshSpaceRotationBlend;
	public:
		/// @brief �R���X�g���N�^
		/// @param _boneList �{�[�����X�g
		/// @param _nodeName �m�[�h��
		LayerdAnimationNode(const BoneList& _boneList, const std::string& _nodeName);
		~LayerdAnimationNode() {}

		/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
		/// @param outTransforms �i�[����{�[���g�����X�t�H�[�����X�g
		/// @param_baseRatio �x�[�X�̃A�j���[�V�������̊���
		/// @param _blendRatio �u�����h���̃A�j���[�V��������
		void GetAnimTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const;

		/// @brief �x�[�X���̃A�j���[�V�����S�̃L�[�������߂�
		/// @return �S�̂̃L�[��
		u_int GetAllKeyFrame() const override;

		/// @brief �u�����h���̃A�j���[�V�����S�̃L�[�������߂�
		/// @return �S�̂̃L�[��
		u_int GetBlendAllKeyFrame() const;

		/// @brief �u�����h���̃A�j���[�V�����J�[�u���x�����߂�
		/// @param _playRatio �Đ�����
		/// @return �Đ�����
		float GetBlendCurveSpeed(float _playRatio) const;

		/// @brief �u�����h�A�j���[�V�����̎��Ԃ��擾����
		/// @return �u�����h�A�j���[�V�����̎���
		float GetBlendAnimationTime() const;

		/// @brief �u�����h�A�j���[�V�����̊J�n���Đ��������擾����
		/// @return �J�n���Đ�����
		float GetBeginBlendPlayRatio() const;

		/// @brief �u�����h�J�n����{�[��ID���擾
		/// @return �u�����h�J�n����{�[��ID
		int GetBeginBlendBoneId() const;

		/// @brief �w�肵�������̃��[�g���[�V�����̈ړ����W���擾
		/// @return ���[�g���[�V�����̈ړ����W
		DXSimp::Vector3 GetRootMotionPos(float _ratio) const;

		/// @brief ���[�g���[�V�����b�����擾
		/// @return ���[�g���[�V�����b��
		const DXSimp::Vector3& GetRootMotionPerSpeed() const;

		/// @brief ���b�V����ԓ��Ńu�����h���s�����擾
		/// @return ���b�V����ԓ��Ńu�����h���s����
		bool GetMotionSpaceRotationBlend() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �@�\�ł��Ă��邩�m�F����
		/// @return �@�\�ł��Ă��邩�H
		bool CanWarking() const;

		/// @brief ���b�V����ԂŃu�����h�������g�����X�t�H�[���擾(�v�Z�R�X�g��)
		/// @param outTransforms �i�[����{�[���g�����X�t�H�[�����X�g
		/// @param _currentNode ���݂̃m�[�h
		/// @param _parentBaseRotation �x�[�X���̐e�{�[���܂ł̃��b�V����ԉ�]��
		/// @param _parentBlendRotation �u�����h���̐e�{�[���܂ł̃��b�V����ԉ�]��
		/// @param _parentCalculateRotation �v�Z����Ă��錻�݂̐e�{�[���܂ł�
		/// @param _baseRatio �x�[�X�̃A�j���[�V�������̊���
		/// @param _blendRatio �u�����h���̃A�j���[�V��������
		/// @param _blendDepth �u�����h�J�n����̐[��
		void GetMeshSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms, 
			const TreeNode* _currentNode, 
			const DXSimp::Quaternion& _parentBaseRotation,
			const DXSimp::Quaternion& _parentBlendRotation,
			const DXSimp::Quaternion& _parentCalculateRotation,
			float _baseRatio, 
			float _blendRatio,
			u_int _blendDepth) const;

		/// @brief �{�[���̃��[�J����ԂŃu�����h�������g�����X�t�H�[���擾(�v�Z�R�X�g��)
		/// @param outTransforms �i�[����{�[���g�����X�t�H�[�����X�g
		/// @param_baseRatio �x�[�X�̃A�j���[�V�������̊���
		/// @param _blendRatio �u�����h���̃A�j���[�V��������
		void GetLocalSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const;


		/// @brief �u�����h�J�n�{�[�����Z�b�g����
		/// @param _beginBoneId �u�����h�J�n�{�[����Id
		void SetBlendBeginBone(int _beginBoneId);

		/// @brief �c���[�m�[�h��H���čs���ă{�[���̃O���[�v�𕪂��Ă���
		/// @param _treeNode �c���[�m�[�h�i�ŏ��̓c���[�m�[�h�j
		void SeparateBoneGroup(const TreeNode& _treeNode);

		/// @brief �u�����h�O���[�v���쐬����
		/// @param _blendBoneNode �u�����h�J�n�m�[�h
		void CreateBlendBoneGroup(const TreeNode& _blendBoneNode);

		void ImGuiDebug() override;
	};
}