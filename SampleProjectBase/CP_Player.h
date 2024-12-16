#pragma once
#include "CP_Character.h"
#include "PlayerAction.h"
#include "AttackInformation.h"
#include "IUISlider.h"

namespace HashiTaku
{
	class CP_Weapon;
	class CP_CameraMove;

	class CP_Player : public CP_Character
	{
		/// @brief ����̃I�u�W�F�N�g��
		std::string weaponObjName;

		/// @brief �J�����I�u�W�F�N�g��
		std::string cameraObjName;

		/// @brief HP�X���C�_�[�I�u�W�F�N�g��
		std::string hpBarObjName;

		/// @brief �A�N�V�����ϐ�
		std::unique_ptr<PlayerAction> pAction;

		/// @brief �A�j���[�V����
		CP_Animation* pAnimation;

		/// @brief ���픻��
		CP_Weapon* pWeapon;

		/// @brief �J�����ړ�
		CP_CameraMove* pCameraMove;

		/// @brief hp�Q�[�W
		IUISlider* pHpSlider;

		/// @brief �U���t���O
		const bool* pAttackCollisionFlag;

		/// @brief �q�b�g�X�g�b�v����O�̍Đ����x
		float hitStopBeforeAnimSpeed;
	public:
		CP_Player();
		~CP_Player() {}

		// �R���|�[�l���g����
		void Init() override;

		/// @brief �q�b�g�X�g�b�v�J�n�������̏���
		void OnHitStopBegin() override;

		/// @brief �q�b�g�X�g�b�v�I���������̏���
		void OnHitStopEnd() override;

		/// @brief �U�������Z�b�g����
		/// @param _setAttackInfo �U�����
		void SetAttackInfo(const AttackInformation& _setAttackInfo);

		/// @brief �U���t���O���擾
		/// @return �U���t���O
		bool GetIsAttackFlag() const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;

	private:
		void Awake() override;
		void Start() override;
		void Update() override;
		void Draw() override;
		void OnDestroy() override;

		/// @brief �X�V�ł��邩�擾
		/// @return �X�V�ł��邩�H
		bool CanUpdate();

		/// @brief ����̃I�u�W�F�N�g���Z�b�g����
		void SetRequireObject();

		/// @brief ����̍U���t���O���Z�b�g
		void SetWeaponAttackFlag();

		/// @brief �v���C���[�̗̑͂��Z�b�g
		/// @param _damageVal �_���[�W�l
		void DecadePlayerHp(float _damageVal);

		/// @brief �v���C���[�̃_���[�W����
		void OnDamageBehavior(const AttackInformation& _attackInfo,
			const DirectX::SimpleMath::Vector3& _attackerPos) override;
		void OnDeathBehavior() override;

		void ImGuiDebug() override;
		void ImGuiFindObj();
	};
}