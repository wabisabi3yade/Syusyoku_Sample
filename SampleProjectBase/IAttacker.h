#pragma once
#include "AttackInformation.h"

namespace HashiTaku
{
	/// @brief �U������I�u�W�F�N�g�̃C���^�[�t�F�[�X
	class IAttacker
	{

	public:
		IAttacker() = default;
		virtual ~IAttacker() = default;

		/// @brief �U���҂̃��[���h���W���擾����
		/// @return ���L�҂̃��[���h���W
		virtual const DXSimp::Vector3& GetAttackerWorldPos() const = 0;

		/// @brief �U���q�b�g�������Ƃ��ɋN��������
		/// @param _atkInfo �U�����
		/// @param _contactWorldPos �Փ˒n�_�i���[���h���W�j
		virtual void OnAttacking(const AttackInformation& _atkInfo, 
			const DXSimp::Vector3& _contactWorldPos) = 0;
	};

}

