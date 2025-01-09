#pragma once
#include "AnimNodePlayer_Base.h"

namespace HashiTaku
{
	/// @brief ���C���[�m�[�h�̍Đ��N���X
	class AnimLayerdNodePlayer : public AnimNodePlayer_Base
	{
		/// @brief �u�����h���̌��݂̍Đ�����
		float curBlendPlayRatio;

	public:
		AnimLayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
		~AnimLayerdNodePlayer() {}

		/// @brief �Đ�������i�߂�
		void ProgressPlayRatio(float _controllerSpeed) override;

		/// @brief �A�j���[�V�����̍X�V����
		/// @param _outTransforms �A�j���[�V�����̎p�� 
		void Update(std::vector<BoneTransform>& _outTransforms) override;

		/// @brief ���[�g���[�V�����̍��W�ړ����x���v�Z����
		void CalcRootMotionPosSpeed() override;

		/// @brief ���[�g���[�V�����̈ړ��ʎ擾
		/// @param _ratio ����
		/// @param _isWorldScaling ���[���h�X�P�[�����O�K�p���邩�H 
		/// @return ���[�g���[�V�����ړ���
		DXSimp::Vector3 GetRootMotionPos(float _ratio, bool _isWorldScaling = true) const override;

	private:
		void ImGuiDebug() override;
	};
}