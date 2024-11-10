#pragma once

class GameObject;

/// @brief �G���Ƃ��ăo�g������C���^�[�t�F�[�X
class IBattleEnemy
{
public:
	IBattleEnemy() {}
	virtual ~IBattleEnemy() {}

	/// @brief �v���C���[�I�u�W�F�N�g���󂯎��
	/// @param _playerObject �v���C���[�I�u�W�F�N�g
	virtual void AcceptPlayerObject(GameObject& _playerObject) = 0;

	/// @brief �G�I�u�W�F�N�g���擾����
	virtual GameObject& GetBatttleEnemyObject() = 0;
};

