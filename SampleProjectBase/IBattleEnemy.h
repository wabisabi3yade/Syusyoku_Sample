#pragma once

class GameObject;

/// @brief 敵側としてバトルするインターフェース
class IBattleEnemy
{
public:
	IBattleEnemy() {}
	virtual ~IBattleEnemy() {}

	/// @brief プレイヤーオブジェクトを受け取る
	/// @param _playerObject プレイヤーオブジェクト
	virtual void AcceptPlayerObject(GameObject& _playerObject) = 0;

	/// @brief 敵オブジェクトを取得する
	virtual GameObject& GetBatttleEnemyObject() = 0;
};

