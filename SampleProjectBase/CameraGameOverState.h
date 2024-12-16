#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief �J�����Q�[���I�[�o�[����
	class CameraGameOverState : public CameraMoveState_Base
	{
		/// @brief �J�������ڂ��g�����X�t�H�[��
		const Transform* pTargetTransform;

		/// @brief �I�t�Z�b�g
		DirectX::SimpleMath::Vector2 lookOffset;

		/// @brief ���̋���
		float horiDistance;

		/// @brief ����
		float heightDistance;
	public:
		CameraGameOverState();
		~CameraGameOverState() {}

		void OnStartBehavior() override;

		void UpdateBehavior() override;

		/// @brief �^�[�Q�b�g�̃g�����X�t�H�[�����Z�b�g
		/// @param _targetPos �^�[�Q�b�g�̃g�����X�t�H�[���̎Q��
		void SetTargetTransform(const Transform& _targetTransform);

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}