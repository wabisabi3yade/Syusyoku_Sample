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

	/// @brief �ړ��͈͂̒��S���W
	DirectX::SimpleMath::Vector3 moveAreaCenterPos;

	/// @brief �ړ��͈͂̔��a
	float moveAreaRadius{ 0.0f };

#ifdef EDIT
	bool isDebugDisplay{ false };
#endif // EDIT

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

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void LateUpdate() override;
	void Draw() override;

	/// @brief �L�����N�^�[�̈ړ������X�V����
	void MoveAreaUpdate();

	/// @brief �ړ��͈͂𐧌�����L�����N�^�[�̈ړ��������s��
	/// @param _charaTransform �L�����N�^�[�̃g�����X�t�H�[��
	void PositionClamp(Transform& _charaTransform);

	void ImGuiDebug() override;
};

