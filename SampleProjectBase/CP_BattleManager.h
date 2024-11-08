#pragma once
#include "SingletonComponent.h"

/// @brief 戦闘のマネージャーコンポーネント
class CP_BattleManager : public SingletonComponent<CP_BattleManager>
{
	friend class SingletonComponent<CP_BattleManager>;

	/// @brief プレイヤーオブジェクト
	GameObject* pPlayerObject;

	/// @brief 敵のオブジェクト
	std::list<GameObject*> enemyList;
public:
	
	/// @brief プレイヤーオブジェクトをセット
	/// @param _playerObj プレイヤーオブジェクト
	void SetPlayer(GameObject& _playerObj);

	/// @brief 敵オブジェクトを追加
	/// @param _enemyObj 敵オブジェクト
	void AddEnemy(GameObject& _enemyObj);

	/// @brief プレイヤーを取り除く
	/// @param _playerObj プレイヤー
	void RemovePlayer(GameObject& _playerObj);

	/// @brief 敵を取り除く
	/// @param _enemyObj 敵
	void RemoveEnemy(GameObject& _enemyObj);

	/// @brief プレイヤーオブジェクトを取得する
	/// @return プレイヤー
	GameObject* GetPlayerObject();
};

