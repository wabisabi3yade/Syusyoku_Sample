#pragma once
#include "AnimationNode_Base.h"

namespace HashiTaku
{
	/// @brief �A�j���[�V������񂪂ЂƂ����̃A�j���[�V�����m�[�h
	class SingleAnimationNode : public AnimationNode_Base
	{
		/// @brief �A�j���[�V�����f�[�^
		AnimationData* pAnimationData;
	public:
		SingleAnimationNode(const std::string& _nodeName) : AnimationNode_Base(_nodeName, NodeType::Single), pAnimationData(nullptr) {}

		~SingleAnimationNode() {}

		/// @brief �A�j���[�V����
		/// @param _animData �A�j���[�V�����f�[�^
		void SetAnimationData(const std::string& _animName);

		// �A�j���[�V�����f�[�^���擾����
		const AnimationData& GetAnimationData() const;

		/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
		/// @param _outTransform �i�[����{�[���g�����X�t�H�[��
		/// @param _boneId �{�[����ID
		/// @param _requestRatio �擾����w��̃A�j���[�V��������
		void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const;

		/// @brief ��������L�[�������߂�
		/// @param _ratio ����
		/// @return �L�[��
		u_int GetRatioToKey(float _ratio) const;

		/// @brief �S�̃L�[�������߂�
		/// @return �S�̂̃L�[��
		u_int GetAllKeyFrame() const override;

		/// @brief ���[�g���[�V�����̈ړ����x���擾����
		/// @return ���[�g���[�V�����̈ړ����x
		const DirectX::SimpleMath::Vector3& GetRootMotionPosSpeed() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}