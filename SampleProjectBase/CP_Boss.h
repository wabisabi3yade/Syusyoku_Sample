#pragma once
#include "CP_Enemy.h"
#include "IAttacker_AcceptParry.h"
#include "CP_Weapon.h"
#include "BossActionController.h"
#include "BossAttackInformation.h"
#include "IUISlider.h"

namespace HashiTaku
{
	/// @brief �{�X�̃R���|�[�l���g
	class CP_Boss : public CP_Enemy, public IAttacker_AcceptParry
	{
		/// @brief ���햼
		std::string weaponObjName;

		/// @brief �̗̓o�[�I�u�W�F�N�g��
		std::string hpBarObjName;

		/// @brief �u���C�N�o�[�I�u�W�F�N�g��
		std::string breakBarObjName;

		/// @brief �A�N�V�����Ǘ�
		std::unique_ptr<BossActionController> pActionController;

		/// @brief �A�j���[�V����
		CP_Animation* pAnimation;

		/// @brief ����I�u�W�F�N�g
		CP_Weapon* pWeapon;

		/// @brief �J�����ړ�
		CP_CameraMove* pCameraMove;

		/// @brief �̗̓o�[
		IUISlider* pHpBar;

		/// @brief �u���C�N�Q�[�W�o�[
		IUISlider* pBreakBar;

		/// @brief �ŐV�̍U�����
		const BossAttackInformation* pRecentlyAttackInformation;

		/// @brief �U������
		const bool* pCanAttack;

		/// @brief �u���C�N�l�̍ő�l
		float maxBreakValue;

		/// @brief ���݂̃u���C�N�l
		float curBreakValue;

		/// @brief �u���C�N�Q�[�W�����炷���x
		float decadeBreakSpeed;

		/// @brief �u���C�N���Ă��邩�H
		bool isBreaking;
	public:
		CP_Boss();
		~CP_Boss() {}

		/// @brief ��������
		void Init() override;

		/// @brief �U�������Z�b�g
		/// @param _attackInfo �U�����
		void SetAttackInfo(AttackInformation& _attackInfo);

		/// @brief �̗͂��Z�b�g
		/// @param _setHp �̗�
		void SetCurrentHP(float _setHp) override;

		/// @brief �u���C�N�l���Z�b�g
		/// @param _set �Z�b�g
		/// @param _canBreak �ő�l�𒴂����Ƃ��Ƀu���C�N����悤�ɂ��邩�H
		void SetBreakValue(float _set, bool _canBreak = true);

		/// @brief �u���C�N���Ă��邩�擾
		/// @return �u���C�N���Ă��邩�H
		bool GetIsBreaking() const;

		/// @brief �u���C�N�l���擾
		/// @return �u���C�N�l
		float GetBreakValue() const;

		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;

		/// @brief ���L�҂̃��[���h���W���擾����
		/// @return ���L�҂̃��[���h���W
		const DXSimp::Vector3& GetAttackerWorldPos() const override;

		/// @brief �U���q�b�g�������Ƃ��ɋN��������
		/// @param _atkInfo �U�����
		/// @param _contactWorldPos �Փ˒n�_�i���[���h���W�j
		void OnAttacking(const AttackInformation& _atkInfo,
			const DXSimp::Vector3& _contactWorldPos) override;

		/// @brief �p���B���ꂽ�Ƃ��̏���
		/// @param _acceptInfo �p���B����̏��
		void OnAcceptParry(const AcceptParryInfo& _acceptInfo) override;
	private:
		void Awake() override;
		void Start() override;
		void Update() override;
		void Draw() override;

		/// @brief �X�V�ł��邩�擾
		/// @return �X�V�ł��邩�H
		bool CanUpdate();

		/// @brief �U���ł��邩�X�V����
		void UpdateAttackCollision();

		/// @brief �u���C�N�֘A�X�V����
		void BreakUpdate();

		/// @brief �K�v�ȃI�u�W�F�N�g��T��
		void FindRequaireObject();

		/// @brief �u���C�N�l�����Z����
		/// @param _add ���Z�l
		/// @param _canBreak �ő�l�𒴂����Ƃ��Ƀu���C�N����悤�ɂ��邩�H
		void AddBreakValue(float _add, bool _canBreak = true);

		/// @brief �u���C�N�������̏���
		void OnBreak();

		/// @brief �u���C�N�I�����̏���
		void OnBreakEnd();

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _attackerPos �U���������̍��W
		/// @return �_���[�W���󂯂����H
		bool OnDamageBehavior(AttackInformation& _attackInfo) override;

		/// @brief ���񂾂Ƃ��̏���
		void OnDeathBehavior() override;

		void ImGuiDebug() override;
	};
}