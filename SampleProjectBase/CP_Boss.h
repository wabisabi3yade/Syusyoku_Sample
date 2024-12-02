#pragma once
#include "CP_Enemy.h"
#include "CP_Weapon.h"
#include "BossActionController.h"
#include "IUISlider.h"

/// @brief �{�X�̃R���|�[�l���g
class CP_Boss : public CP_Enemy
{
	/// @brief ���햼
	std::string weaponObjName;

	/// @brief �̗̓o�[�I�u�W�F�N�g��
	std::string hpBarObjName;

	/// @brief �A�N�V�����Ǘ�
	std::unique_ptr<BossActionController> pActionController;

	/// @brief �A�j���[�V����
	CP_Animation* pAnimation;

	/// @brief ����I�u�W�F�N�g
	CP_Weapon* pWeapon;

	/// @brief �̗̓o�[
	IUISlider* pHpBar;

	/// @brief �U������
	const bool* pCanAttack;
public:
	CP_Boss();
	~CP_Boss() {}

	/// @brief ��������
	void Init() override;

	/// @brief �U�������Z�b�g
	/// @param _attackInfo �U�����
	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInfo);

	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
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

	/// @brief �K�v�ȃI�u�W�F�N�g��T��
	void FindRequaireObject();

	
	/// @brief �̗͂��Z�b�g
	/// @param _setHp �̗�
	void SetCurrentHP(float _setHp)override;

	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
	/// @param _attackInfo �U�����
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos) override;

	/// @brief ���񂾂Ƃ��̏���
	void OnDeathBehavior() override;

	void ImGuiDebug() override;
};

