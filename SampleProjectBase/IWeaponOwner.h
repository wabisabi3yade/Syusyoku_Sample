#pragma once

namespace HashiTaku
{
	/// @brief ���폊�����Ă���I�u�W�F�N�g�̃C���^�[�t�F�[�X
	class IWeaponOwner
	{

	public:
		IWeaponOwner() = default;
		virtual ~IWeaponOwner() = default;

		/// @brief ���L�҂̃��[���h���W���擾����
		/// @return ���L�҂̃��[���h���W
		virtual const DXSimp::Vector3& GetOwnerWorldPos() const = 0;

		/// @brief ����ɂ��U���q�b�g�������Ƃ��ɋN��������
		virtual void OnWeaponAttacking() = 0;
	};

}

