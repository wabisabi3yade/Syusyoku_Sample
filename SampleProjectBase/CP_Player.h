#pragma once
#include "CP_Character.h"
#include "PlayerActionController.h"
#include "AttackInformation.h"

class CP_Weapon;

class CP_Player : public CP_Character
{
	/// @brief �A�N�V�����R���g���[���[
	std::unique_ptr<PlayerActionController> pActionController;

	/// @brief �A�j���[�V����
	CP_Animation* pAnimation;

	/// @brief ���픻��
	CP_Weapon* pWeapon;

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

	/// @brief ����R���|�[�l���g���Z�b�g
	/// @param _setWeapon ����R���|�[�l���g
	void SetWeapon(CP_Weapon& _setWeapon);

	/// @brief �U�������Z�b�g����
	/// @param _setAttackInfo �U�����
	void SetAttackInfo(const HashiTaku::AttackInformation& _setAttackInfo);

	/// @brief �U���t���O���擾
	/// @return �U���t���O
	bool GetIsAttackFlag() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	/// @brief ����̍U���t���O���Z�b�g
	void SetWeaponAttackFlag();

	/// @brief �X�V�ł��邩�擾����
	/// @return �X�V�ł��邩�H
	bool GetCanUpdate() const;

	/// @brief �v���C���[�̃_���[�W����
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo) override;
	void OnDeathBehavior() override;	

	void ImGuiDebug() override;
	void ImGuiSetWeapon();
};


