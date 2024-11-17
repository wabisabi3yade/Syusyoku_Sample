#pragma once
#include "SingletonComponent.h"
#include "ITargetAccepter.h"

class CP_Player;
class CP_Enemy;

/// @brief �퓬�̃}�l�[�W���[�R���|�[�l���g
class CP_BattleManager : public SingletonComponent<CP_BattleManager>
{
public:
	// �G���X�g
	using EnemyList = std::list<CP_Enemy*>;

private:
	friend class SingletonComponent<CP_BattleManager>;

	/// @brief �v���C���[�I�u�W�F�N�g
	CP_Player* pPlayer{ nullptr };

	/// @brief �G�S�̃��X�g
	EnemyList enemyList;
public:
	/// @brief �v���C���[�I�u�W�F�N�g���Z�b�g
	/// @param _playerObj �v���C���[�I�u�W�F�N�g
	void SetPlayer(CP_Player& _playerObj);

	/// @brief �G�I�u�W�F�N�g��ǉ�
	/// @param _enemyObj �G�I�u�W�F�N�g
	void AddEnemy(CP_Enemy& _enemyObj);

	/// @brief �v���C���[����菜��
	/// @param _playerObj �v���C���[
	void RemovePlayer(CP_Player& _playerObj);

	/// @brief �G����菜��
	/// @param _enemyObj �G
	void RemoveEnemy(CP_Enemy& _enemyObj);

	/// @brief �v���C���[�I�u�W�F�N�g���擾����
	/// @return �v���C���[
	CP_Player* GetPlayerObject();

	/// @brief �G���X�g���擾����
	/// @return 
	const EnemyList& GetEnemyList();

private:
	void ImGuiDebug() override;
};

