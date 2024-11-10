#pragma once
#include "EnemyActionController.h"
#include "BossActState_Base.h"

class CP_Player;
class CP_Boss;

/// @brief ボスのアクションコントローラー
class BossActionController : public EnemyActionController
{
	/// @brief プレイヤーオブジェクト
	CP_Player* pPlayerObject;

public:
	BossActionController(CP_Boss& _boss);
	~BossActionController() {}

	/// @brief 初期化処理
	/// @param _animationController 
	void Init(CP_Animation& _animationController) override;

	/// @brief 更新処理
	void Update() override;

	/// @brief ステートを変更
	/// @param _nextState 次のステート
	/// @return 成功したか？
	bool ChangeState(BossActState_Base::BossState _nextState);

	/// @brief プレイヤーオブジェクトをセット
	/// @param _playerObj プレイヤーオブジェクト
	void SetPlayer(CP_Player& _playerObj);

	/// @brief ボスコンポーネントを取得
	CP_Boss& GetBoss();

	/// @brief プレイヤーを取得する
	/// @return プレイヤーオブジェクト
	CP_Player& GetPlayer();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:	
	/// @brief ボスのステートを作成
	/// @tparam T ボスの行動クラス
	/// @param _createState 作成したいステート
	template<class T>
	void CreateState(BossActState_Base::BossState _createState);

	/// @brief ボスの更新処理が行えるか
	/// @return 行えるか?
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
