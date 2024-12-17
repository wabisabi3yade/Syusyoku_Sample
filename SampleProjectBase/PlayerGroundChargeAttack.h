#pragma once
#include "PlayerAttackState.h"

namespace HashiTaku
{
	/// @brief �n�㗭�ߍU��
	class PlayerGroundChargeAttack : public PlayerAttackState
	{
		/// @brief �`���[�W�̃��x��
		enum class ChargeLevel
		{
			Low,	// ��
			Mid,	// ��
			High,	// ��
			MaxNum
		};

		/// @brief �`���[�W�i�K���Ƃ̍U�����
		using ChargeLevelAtkInfos = 
			std::array<PlayerAttackInformation, static_cast<u_int>(ChargeLevel::MaxNum)>;

		/// @brief �`���[�W���x�����Ƃ̍U�����(�e�U����)
		std::vector<ChargeLevelAtkInfos> chargeAtkInfos;

		/// @brief ���x�����Ƃ̃`���[�W�̎���
		std::array<float, static_cast<u_int>(ChargeLevel::MaxNum)> chargeTimes;

		/// @brief �ő嗭�ߎ���
		float maxChargeTime;

		/// @brief ���݂̗��ߎ���
		float curChargingTime;

		/// @brief ���݂̃`���[�W�̃��x��
		ChargeLevel curChargeLevel;

		/// @brief �`���[�W�����ǂ����H
		bool isCharging;
	public:
		PlayerGroundChargeAttack();
		~PlayerGroundChargeAttack() {}

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief �J�n
		void OnStartBehavior() override;

		/// @brief �X�V
		void UpdateBehavior() override;

		/// @brief �U���񐔂�ς����Ƃ��̏���
		void OnChangeAttackTimes() override;

		/// @brief ���͂𗣂������m�F����
		/// @return ���͗��������H
		void CheckInputRelease();

		/// @brief �`���[�W���̍X�V����
		void ChargingUpdate();

		/// @brief �`���[�W�I�����̏���
		void OnChargeEnd();
	
		void ImGuiDebug() override;

		// �U�����̕ҏW
		void ImGuiAttackInfo() override;
	};
}

