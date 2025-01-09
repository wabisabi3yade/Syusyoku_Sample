#pragma once
#include "PlayerAttackState.h"

namespace HashiTaku
{
	class CP_CameraMove;

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

		/// @brief ���x�����Ƃ̃`���[�W�̐F
		std::array<DXSimp::Color, static_cast<u_int>(ChargeLevel::MaxNum)> chargeVfxColor;

		/// @brief �`���[�W�Ŏ��̒i�K�Ɉڍs�������̃G�t�F�N�g
		CreateVfxInfo onNextChargeVfx;

		/// @brief �`���[�W�U���Ɉڍs����G�t�F�N�g
		CreateVfxInfo chargeReleaseVfx;

		/// @brief �`���[�W�G�t�F�N�g�̃I�t�Z�b�g���W
		DXSimp::Vector3 chargeVfxOffset;

		/// @brief �`���[�W�U���ڍs���̃G�t�F�N�g�I�t�Z�b�gY���W
		float chargeReleaseVfxOffsetY;

		// �J�����ړ��R���|�[�l���g
		CP_CameraMove* pCamMove;

		/// @brief �ő嗭�ߎ���
		float maxChargeTime;

		/// @brief ���݂̗��ߎ���
		float curChargingTime;

		/// @brief 1�t���[���O�̗̂��ߎ���
		float lastChargingTime;

		/// @brief �`���[�W���̃G�t�F�N�g�̐������ԃI�t�Z�b�g(�`���[�W�����猩��)
		float chargeVfxCreateTimeOffset;

		/// @brief �`���[�W���̃p�b�h�U����
		float chargePadShakePower;

		/// @brief ���݂̃`���[�W�̃��x��
		ChargeLevel curChargeLevel;

		/// @brief �`���[�W�̃G�t�F�N�g�n���h��
		Effekseer::Handle chargeVfxHandle;

		/// @brief �`���[�W�����ǂ����H
		bool isCharging;
	public:
		PlayerGroundChargeAttack();
		~PlayerGroundChargeAttack() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �J�n
		void OnStartBehavior() override;

		/// @brief �X�V
		void UpdateBehavior() override;

		/// @brief �I��
		void OnEndBehavior() override;

		/// @brief �J�������`���[�W��ԂɕҎq������
		void ChangeCameraChargeState();

		/// @brief �U���񐔂�ς����Ƃ��̏���
		void OnChangeAttackTimes() override;

		/// @brief ���͂𗣂������m�F����
		/// @return ���͗��������H
		void CheckInputRelease();

		/// @brief �`���[�W���̍X�V����
		void ChargingUpdate();

		/// @brief �`���[�W�̃G�t�F�N�g���o��
		void CreateChargeVfx();

		/// @brief ���̃`���[�W���x���ֈڍs
		void NextChargeLevel();

		/// @brief �`���[�W�I�����̏���
		void OnChargeEnd();
	
		void ImGuiDebug() override;

		// �U�����̕ҏW
		void ImGuiAttackInfo() override;
	};
}

