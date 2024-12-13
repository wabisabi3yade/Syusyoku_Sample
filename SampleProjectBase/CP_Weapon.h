#pragma once
#include "Component.h"
#include "AttackInformation.h"
#include "IDamageable.h"
#include "Tag.h"

class CP_RigidBody;

/// @brief ����R���|�[�l���g
class CP_Weapon : public Component
{
	/// @brief �U���Ώۃ^�O�̏��
	static constexpr u_int ATTACK_TAG_MAX{ 3 };

	/// @brief �U�����
	const HashiTaku::AttackInformation* pAtkInfomation;

	/// @brief �U���ɂ�����^�O
	std::array<HashiTaku::Tag::Type, ATTACK_TAG_MAX> attackableTags;

	/// @brief ���̍U������ŏd�����Ȃ��悤�ɋL�^����p���X�g
	std::vector<const CP_RigidBody*> attackedRbs;

	/// @brief ���̕�������I�u�W�F�N�g�̍��W
	const DirectX::SimpleMath::Vector3* pHaveObjectPos;

	/// @brief �U���^�O�̐�
	u_int attackTagCnt;

	/// @brief �U������R���W����
	bool isAttackCollision;

#ifdef EDIT
	/// @brief �U���t���O�`��
	bool isDebugAttackDisplay{ true };
#endif // EDIT

public:
	CP_Weapon();
	virtual ~CP_Weapon() {}

	void Draw() override;

	// �R���|�[�l���g����
	void OnCollisionStay(const HashiTaku::CollisionInfo& _otherColInfo);

	/// @brief �U�������Z�b�g����
	/// @param _attackInformation �U�����
	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation);

	/// @brief ����̍U������R���W����
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);

	/// @brief ���L�I�u�W�F�N�g�̍��W�̃|�C���^
	/// @param _pWorldPos ���[���h���W
	void SetHaveObjPosPointer(const DirectX::SimpleMath::Vector3* _pWorldPos);

	/// @brief �U���ς݃R���W���������Z�b�g
	void ClearAttackedRb();

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �U���ł��邩�擾
	/// @return �U���ł��邩�H
	bool CanAttack() const;

	/// @brief �U������
	/// @param _damager �U���^����Ώ�
	/// @param _haveObjPos ���L�I�u�W�F�N�g�̍��W
	void OnAttack(HashiTaku::IDamageable& _damager, 
		const DirectX::SimpleMath::Vector3& _haveObjPos);

	/// @brief ���ɍU�����������X�g�ɒǉ�
	/// @param _rb �U���ς݂�Rb
	void AddAttackedRb(const CP_RigidBody& _rb);

	/// @brief �U���ł��邩�m�F
	/// @param _targetObject �Ώۂ̃I�u�W�F�N�g
	/// @return �U���ł��邩�H
	bool CheckAttackableTag(GameObject& _targetObject) const;

	/// @brief �U���ł��邩�m�F
	/// @param  _targetRb �Ώۂ̃I�u�W�F�N�g
	/// @return �U���ł��邩�H
	bool CheckAttackedRb(const CP_RigidBody& _targetRb) const;

	/// @brief �q�b�g�G�t�F�N�g���o��
	/// @param �����蔻��̏Փ˒n�_
	void CreateHitVfx(const DirectX::SimpleMath::Vector3& _contactPos);

	// �f�o�b�O�p�U���t���O�������Ă��邩���邽�߂̕`��
	void DebugAttackFlag();

	/// @brief �U���ł���^�O��ǉ�
	/// @param _addTag �^�O
	void AddAttackableTag(HashiTaku::Tag::Type _addTag);

	/// @brief �Ō���̍U���ł���^�O���폜
	/// @param _addTag �^�O
	void RemoveBackAttackableTag();

	void ImGuiDebug() override;
};



