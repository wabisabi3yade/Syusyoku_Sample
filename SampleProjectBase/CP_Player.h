#pragma once
#include "CP_Character.h"
#include "IWeaponOwner.h"
#include "PlayerAction.h"
#include "AttackInformation.h"
#include "IUISlider.h"

namespace HashiTaku
{
	class CP_Weapon;
	class CP_CameraMove;
	class CP_StylishUI;

	class CP_Player : public CP_Character, public IWeaponOwner
	{
		/// @brief ����̃I�u�W�F�N�g��
		std::string weaponObjName;

		/// @brief �J�����I�u�W�F�N�g��
		std::string cameraObjName;

		/// @brief HP�X���C�_�[�I�u�W�F�N�g��
		std::string hpBarObjName;

		/// @brief �X�^�C���b�V��UI�̃I�u�W�F�N�g��
		std::string stylishUIName;

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

		/// @brief �X�^�C���b�V��UI
		CP_StylishUI* pStylishUI;

		/// @brief �U���t���O
		const bool* pAttackCollisionFlag;

		/// @brief �q�b�g�X�g�b�v����O�̍Đ����x
		float hitStopBeforeAnimSpeed;

		/// @brief �U���_���[�W�l����X�^�C���b�V���|�C���g�ւ̔{��
		float stylishPointRatioFromAtkDmg;

		/// @brief �󂯂��_���[�W�l����X�^�C���b�V���|�C���g�ւ̔{��
		float stylishPointRatioFromAcceptDmg;

		/// @brief �f�o�b�O�̖��G�t���O
		static bool isDebugInvicible;
	public:
		CP_Player();
		~CP_Player() {}

		// �R���|�[�l���g����
		void Init() override;

		/// @brief �q�b�g�X�g�b�v�J�n�������̏���
		void OnHitStopBegin() override;

		/// @brief �q�b�g�X�g�b�v�I���������̏���
		void OnHitStopEnd() override;

		/// @brief �X�^�C���b�V���|�C���g�����Z
		/// @param _addPoint ���Z����l
		void AddStylishPoint(float _addPoint);

		/// @brief �U�������Z�b�g����
		/// @param _setAttackInfo �U�����
		void SetAttackInfo(const AttackInformation& _setAttackInfo);

		/// @brief �f�o�b�O�p�̖��G�t���O���Z�b�g����
		/// @param _setBool ���G�ɂ��邩�H
		static void SetIsDebugInvincible(bool _setBool);

		/// @brief �U���t���O���擾
		/// @return �U���t���O
		bool GetIsAttackFlag() const;

		/// @brief �f�o�b�O�p���G�t���O���擾
		/// @return �f�o�b�O�p���G���H
		static bool GetIsDebugInvincible();

		json Save() override;
		void Load(const json& _data) override;

		/// @brief ���L�҂̃��[���h���W���擾����
		/// @return ���L�҂̃��[���h���W
		const DXSimp::Vector3& GetOwnerWorldPos() const override;

		/// @brief ����ɂ��U���q�b�g�������Ƃ��ɋN��������
		void OnWeaponAttacking(const AttackInformation& _atkInfo) override;
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
		bool OnDamageBehavior(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _attackerPos) override;
		void OnDeathBehavior() override;

		void ImGuiDebug() override;
		void ImGuiFindObj();
	};
}