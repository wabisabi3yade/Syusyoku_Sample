#pragma once
#include "CP_Character.h"
#include "ITargetAccepter.h"

class CP_Enemy : public CP_Character, public ITargetAccepter
{
	/// @brief �G�̖���
	std::string enemyName;

	/// @brief ���g���^�[�Q�b�g�ɂ��Ă����
	std::list<IObjectTargeter*> pTargeters;

public:
	CP_Enemy();
	virtual ~CP_Enemy() {}

	/// @brief �G�����擾
	/// @return �G��
	const std::string& GetEnemyName() const;

	/// @brief ���̃I�u�W�F�N�g�̃��[���h���W���擾
	/// @return ���[���h���W���擾
	const DirectX::SimpleMath::Vector3& GetWorldPosByTargetObj() const override;

	/// @brief �^�[�Q�b�g���鑤���擾����
	/// @param _targeter �^�[�Q�b�g���鑤
	void SetTargeter(IObjectTargeter& _targeter);

	/// @brief �^�[�Q�b�g���鑤���폜����
	/// @param _targeter �폜�^�[�Q�b�g���鑤
	void RemoveTargeter(IObjectTargeter& _targeter) override;

	/// @brief �^�[�Q�b�g���Ɏ��񂾂��Ƃ�ʒm
	void OnRemoveNotifyToTargeter();
protected:
	void Awake() override;
	void OnDestroy() override;

	/// @brief �G�����Z�b�g
	/// @param _enemyName �G��
	void SetEnemyName(const std::string& _enemyName);

	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
	/// @param _attackInfo �U�����
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos) override;

	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
	/// @param _attackInfo �U�����
	void OnDeathBehavior() override;
};

