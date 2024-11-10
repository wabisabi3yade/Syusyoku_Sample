#pragma once
#include "EnemyActionController.h"
#include "BossActState_Base.h"

class CP_Player;
class CP_Boss;

/// @brief �{�X�̃A�N�V�����R���g���[���[
class BossActionController : public EnemyActionController
{
	/// @brief �v���C���[�I�u�W�F�N�g
	CP_Player* pPlayerObject;

public:
	BossActionController(CP_Boss& _boss);
	~BossActionController() {}

	/// @brief ����������
	/// @param _animationController 
	void Init(CP_Animation& _animationController) override;

	/// @brief �X�V����
	void Update() override;

	/// @brief �X�e�[�g��ύX
	/// @param _nextState ���̃X�e�[�g
	/// @return �����������H
	bool ChangeState(BossActState_Base::BossState _nextState);

	/// @brief �v���C���[�I�u�W�F�N�g���Z�b�g
	/// @param _playerObj �v���C���[�I�u�W�F�N�g
	void SetPlayer(CP_Player& _playerObj);

	/// @brief �{�X�R���|�[�l���g���擾
	CP_Boss& GetBoss();

	/// @brief �v���C���[���擾����
	/// @return �v���C���[�I�u�W�F�N�g
	CP_Player& GetPlayer();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:	
	/// @brief �{�X�̃X�e�[�g���쐬
	/// @tparam T �{�X�̍s���N���X
	/// @param _createState �쐬�������X�e�[�g
	template<class T>
	void CreateState(BossActState_Base::BossState _createState);

	/// @brief �{�X�̍X�V�������s���邩
	/// @return �s���邩?
	bool IsCanBossUpdate();

	void ImGuiDebug() override;
};

template<class T>
inline void BossActionController::CreateState(BossActState_Base::BossState _createState)
{
	std::unique_ptr<BossActState_Base> createState = std::make_unique<T>();
	createState->Init(_createState, *this);

	AddNode(static_cast<int>(_createState), std::move(createState));
}
