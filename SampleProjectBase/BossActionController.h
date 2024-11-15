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

	/// @brief のけぞることはできるのか？(アニメーションパラメータ)
	const bool* pCanKnock;

	/// @brief デフォルト状態設定
	BossActState_Base::BossState defaultState;

public:
	BossActionController(CP_Boss& _boss);
	~BossActionController() {}

	/// @brief 初期化処理
	/// @param _pAnimation アニメーション
	void Init(CP_Animation* _pAnimation) override;

	/// @brief ステートを変更
	/// @param _nextState 次のステート
	/// @param _isForce　強制切り替え
	/// @return 成功したか？
	bool ChangeState(BossActState_Base::BossState _nextState, bool _isForce = false);

	/// @brief ダメージ時処理
	void OnDamage();

	/// @brief プレイヤーオブジェクトをセット
	/// @param _playerObj プレイヤーオブジェクト
	void SetPlayer(CP_Player& _playerObj);

	/// @brief ボスコンポーネントを取得
	CP_Boss& GetBoss();

	/// @brief プレイヤーを取得する
	/// @return プレイヤーオブジェクト
	CP_Player& GetPlayer();

	/// @brief のけぞることができるか取得
	/// @return のけぞりできるか？
	const bool GetCanKnock() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:	
	/// @brief 更新処理
	void Update() override;

	/// @brief ボスのステートを作成
	/// @tparam T ボスの行動クラス
	/// @param _createState 作成したいステート
	template<class T>
	void CreateState(BossActState_Base::BossState _createState);

	/// @brief 最初にアニメーションパラメータから取得
	void GetAnimationParam();

	/// @brief ボスの更新処理が行えるか
	/// @return 行えるか?
	bool IsCanBossUpdate();

	/// @brief ボス行動に変換
	/// @param _pBaseNode ベースノードクラス
	/// @return ボス講堂
	BossActState_Base* CastBossAct(HashiTaku::StateNode_Base* _pBaseNode);

	/// @brief デフォルト状態をセット
	/// @param _defaultState デフォルト状態
	void SetDefaultState(BossActState_Base::BossState _defaultState);

	/// @brief 各Stateの文字列を取得する
	/// @param _stateId 状態のID
	/// @return 文字列
	std::string GetStateStr(int _stateId) override;

	/// @brief 各StateのIDを取得する
	/// @param _stateName　状態名
	/// @return 状態のID
	int GetStateId(const std::string& _stateName) override;

	void ImGuiDebug() override;
};

template<class T>
inline void BossActionController::CreateState(BossActState_Base::BossState _createState)
{
	std::unique_ptr<BossActState_Base> createState = std::make_unique<T>();
	createState->Init(_createState, *this);

	AddNode(static_cast<int>(_createState), std::move(createState));
}
