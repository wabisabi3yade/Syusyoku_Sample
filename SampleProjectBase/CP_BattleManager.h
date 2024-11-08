#pragma once
#include "SingletonComponent.h"

/// @brief �퓬�̃}�l�[�W���[�R���|�[�l���g
class CP_BattleManager : public SingletonComponent<CP_BattleManager>
{
	friend class SingletonComponent<CP_BattleManager>;

	/// @brief �v���C���[�I�u�W�F�N�g
	GameObject* pPlayerObject;

	/// @brief �G�̃I�u�W�F�N�g
	std::list<GameObject*> enemyList;
public:
	
	/// @brief �v���C���[�I�u�W�F�N�g���Z�b�g
	/// @param _playerObj �v���C���[�I�u�W�F�N�g
	void SetPlayer(GameObject& _playerObj);

	/// @brief �G�I�u�W�F�N�g��ǉ�
	/// @param _enemyObj �G�I�u�W�F�N�g
	void AddEnemy(GameObject& _enemyObj);

	/// @brief �v���C���[����菜��
	/// @param _playerObj �v���C���[
	void RemovePlayer(GameObject& _playerObj);

	/// @brief �G����菜��
	/// @param _enemyObj �G
	void RemoveEnemy(GameObject& _enemyObj);

	/// @brief �v���C���[�I�u�W�F�N�g���擾����
	/// @return �v���C���[
	GameObject* GetPlayerObject();
};

