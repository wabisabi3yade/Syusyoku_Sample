#pragma once
#include "CP_Character.h"
#include "ITargetAccepter.h"
#include "IParryAccepter.h"
#include "PerlinShake.h"

namespace HashiTaku
{
	class CP_MeshRenderer;

	class CP_Enemy : public CP_Character, public ITargetAccepter
	{
		/// @brief �U�����x�����Ƃ̃_���[�W�󂯂��Ƃ��ɗh�炷����
		std::array<PerlinShakeParameter,
			static_cast<u_int>(AttackInformation::AttackLevel::MaxNum)> acceptDamageShakeParams;

		/// @brief �\�񂵂Ă���V�F�C�N�p�����[�^�[
		PerlinShakeParameter* pReserveShakeParam;

		/// @brief �_���[�W�󂯂��Ƃ��ɗh�炷����
		PerlinShake acceptDamageShake;

		/// @brief ���b�V���`��
		CP_MeshRenderer* pMeshRenderer;

		/// @brief ���g���^�[�Q�b�g�ɂ��Ă����
		std::list<IObjectTargeter*> pTargeters;

		/// @brief �G�̖���
		std::string enemyName;

	public:
		CP_Enemy();
		virtual ~CP_Enemy() {}

		/// @brief �G�����擾
		/// @return �G��
		const std::string& GetEnemyName() const;

		/// @brief ���̃I�u�W�F�N�g�̃��[���h���W���擾
		/// @return ���[���h���W���擾
		const DXSimp::Vector3& GetWorldPos() const override;

		/// @brief �^�[�Q�b�g���鑤���擾����
		/// @param _targeter �^�[�Q�b�g���鑤
		void SetTargeter(IObjectTargeter& _targeter);

		/// @brief �^�[�Q�b�g���鑤���폜����
		/// @param _targeter �폜�^�[�Q�b�g���鑤
		void RemoveTargeter(IObjectTargeter& _targeter) override;

		/// @brief �q�b�g�X�g�b�v�J�n�������̏���
		void OnHitStopBegin() override;

		/// @brief �q�b�g�X�g�b�v�I���������̏���
		void OnHitStopEnd() override;

		/// @brief �^�[�Q�b�g���Ɏ��񂾂��Ƃ�ʒm
		void OnRemoveNotifyToTargeter();

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;
	protected:
		void Start() override;
		void Update() override;
		void OnDestroy() override;

		/// @brief �q�b�g�X�g�b�v���ɗh��邩�H
		void HitStopShaking();

		/// @brief �G�����Z�b�g
		/// @param _enemyName �G��
		void SetEnemyName(const std::string& _enemyName);

		/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _attackerPos �U���������̍��W
		/// @return �_���[�W���󂯂����H
		bool OnDamageBehavior(AttackInformation& _attackInfo) override;

		/// @brief �_���[�W�󂯂��Ƃ��̏���
		/// @param _attackInfo �U�����
		/// @param _contactPos �Փ˒n�_
		void OnTakeDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos) override;

		/// @brief ���񂾂Ƃ��̏���
		void OnDeathBehavior() override {};

		void ImGuiDebug() override;
	private:
		// �q�b�g���̗h��V�F�C�N�ҏW
		void ImGuiHitShake();
	};
}