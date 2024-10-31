#pragma once
#include "Component.h"
#include "AttackInformation.h"
#include "IDamageable.h"
#include "Tag.h"

/// @brief ����R���|�[�l���g
class CP_Weapon : public Component
{
	/// @brief �U�����
	HashiTaku::AttackInformation atkInfomation;

	/// @brief �U���ɂ�����^�O
	std::list<HashiTaku::Tag::Type> attackableTags;

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
	void OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo);

	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation);

	/// @brief ����̍U������R���W����
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �U������
	/// @param _damager �U���^����Ώ�
	void OnAttack(HashiTaku::IDamageable& _damager);

	/// @brief �U���ł��邩�m�F
	/// @param _targetObject �Ώۂ̃I�u�W�F�N�g
	/// @return �U���ł��邩�H
	bool CheckAttackable(GameObject& _targetObject);

	// �f�o�b�O�p�U���t���O�������Ă��邩���邽�߂̕`��
	void DebugAttackFlag();

	/// @brief �U���ł���^�O��ǉ�
	/// @param _addTag �^�O
	void AddAttackableTag(HashiTaku::Tag::Type _addTag);

	void ImGuiSetting() override;
};



