#pragma once
#include "CP_Enemy.h"
#include "CP_Weapon.h"
#include "BossActionController.h"
#include "IUISlider.h"

namespace HashiTaku
{
	/// @brief �{�X�̃R���|�[�l���g
	class CP_Boss : public CP_Enemy
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

		/// @brief �̗̓o�[
		IUISlider* pHpBar;

		/// @brief �u���C�N�Q�[�W�o�[
		IUISlider* pBreakBar;

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
		void SetAttackInfo(const AttackInformation& _attackInfo);

		/// @brief �u���C�N�l���Z�b�g
		/// @param _set �Z�b�g
		void SetBreakValue(float _set);

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
		void AddBreakValue(float _add);

		/// @brief �u���C�N�������̏���
		void OnBreak();

		/// @brief �u���C�N�I�����̏���
		void OnBreakEnd();

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// 
		
		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _attackerPos �U���������̍��W
		/// @return �_���[�W���󂯂����H
		bool OnDamageBehavior(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _attackerPos) override;

		/// @brief ���񂾂Ƃ��̏���
		void OnDeathBehavior() override;

		/// @brief �̗͂��Z�b�g
		/// @param _setHp �̗�
		void SetCurrentHP(float _setHp)override;

		void ImGuiDebug() override;
	};
}