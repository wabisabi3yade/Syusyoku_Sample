#pragma once
#include "SingletonComponent.h"
#include "ITargetAccepter.h"

class CP_Player;
class CP_Enemy;

/// @brief 戦闘のマネージャーコンポーネント
class CP_BattleManager : public SingletonComponent<CP_BattleManager>
{
public:
	// 敵リスト
	using EnemyList = std::list<CP_Enemy*>;

private:
	friend class SingletonComponent<CP_BattleManager>;

	/// @brief プレイヤーオブジェクト
	CP_Player* pPlayer{ nullptr };

	/// @brief 敵全体リスト
	EnemyList enemyList;

	/// @brief 移動範囲の中心座標
	DirectX::SimpleMath::Vector3 moveAreaCenterPos;

	/// @brief 移動範囲の半径
	float moveAreaRadius{ 0.0f };

#ifdef EDIT
	bool isDebugDisplay{ false };
#endif // EDIT

public:
	/// @brief プレイヤーオブジェクトをセット
	/// @param _playerObj プレイヤーオブジェクト
	void SetPlayer(CP_Player& _playerObj);

	/// @brief 敵オブジェクトを追加
	/// @param _enemyObj 敵オブジェクト
	void AddEnemy(CP_Enemy& _enemyObj);

	/// @brief プレイヤーを取り除く
	/// @param _playerObj プレイヤー
	void RemovePlayer(CP_Player& _playerObj);

	/// @brief 敵を取り除く
	/// @param _enemyObj 敵
	void RemoveEnemy(CP_Enemy& _enemyObj);

	/// @brief プレイヤーオブジェクトを取得する
	/// @return プレイヤー
	CP_Player* GetPlayerObject();

	/// @brief 敵リストを取得する
	/// @return 
	const EnemyList& GetEnemyList();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void LateUpdate() override;
	void Draw() override;

	/// @brief キャラクターの移動制限更新処理
	void MoveAreaUpdate();

	/// @brief 移動範囲を制限するキャラクターの移動制限を行う
	/// @param _charaTransform キャラクターのトランスフォーム
	void PositionClamp(Transform& _charaTransform);

	void ImGuiDebug() override;
};

