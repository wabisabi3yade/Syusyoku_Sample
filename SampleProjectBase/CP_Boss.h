#pragma once
#include "CP_Enemy.h"
#include "CP_Weapon.h"
#include "BossActionController.h"


/// @brief �{�X�̃R���|�[�l���g
class CP_Boss : public CP_Enemy
{
	/// @brief ���햼
	std::string weaponObjName;

	/// @brief �A�N�V�����Ǘ�
	std::unique_ptr<BossActionController> pActionController;

	/// @brief �A�j���[�V����
	CP_Animation* pAnimation;

	/// @brief ����I�u�W�F�N�g
	CP_Weapon* pWeapon;
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

	/// @brief ������Z�b�g����
	void SetupWeapon();

	/// @brief �X�V�ł��邩�H
	/// @return �X�V�ł��邩�H
	bool GetCanUpdate();

	/// @brief �q�b�g�X�g�b�v�J�n����
	void OnHitStopBegin() override;

	/// @brief �q�b�g�X�g�b�v�I������
	void OnHitStopEnd() override;

	/// @brief �e�N���X�̃_���[�W���󂯂��Ƃ��̏���
	/// @param _attackInfo �U�����
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo) override;

	void ImGuiDebug() override;
};

