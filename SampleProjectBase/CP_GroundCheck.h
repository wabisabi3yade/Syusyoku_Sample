#pragma once
#include "Component.h"
#include "IGroundNotifyer.h"

namespace HashiTaku
{
	/// @brief �n�ʂ̔���m�F������s���N���X
	class CP_GroundCheck : public Component
	{
		/// @brief �ʒm��̃C���^�[�t�F�[�X
		IGroundNotifyer* pNotifyObject;

	public:
		CP_GroundCheck();
		virtual ~CP_GroundCheck() {}

		/// @brief �ڒn�ʒm��I�u�W�F�N�g���Z�b�g
		/// @param _notifyChecker �ڒn�ʒm��I�u�W�F�N�g
		void SetGroundNotifyer(IGroundNotifyer& _notifyChecker);

		/// @brief �ڒn�ʒm��I�u�W�F�N�g���O��
		/// @param _removeChecker �ڒn�ʒm��I�u�W�F�N�g
		void RemoveGroundNoifyer(IGroundNotifyer& _removeChecker);

		/// @brief �ՓˊJ�n
		/// @param _otherColInfo �Փˏ��
		void OnCollisionEnter(const CollisionInfo& _otherColInfo) override;

		/// @brief �ՓˏI��
		/// @param _otherColInfo �Փˏ��
		void OnCollisionExit(const CollisionInfo& _otherColInfo) override;
	};
}