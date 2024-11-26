#pragma once
#include "EnemyActionController.h"
#include "BossActState_Base.h"

class CP_Player;
class CP_Boss;

/// @brief ボスのアクションコントローラー
class BossActionController : public EnemyActionController
{
public:
	// 行動の距離タイプ
	enum class ActDistance
	{
		Short,	// 近距離
		Mid,	// 中距離
		Far,	// 遠距離
		MaxNum
	};

private:
	/// @brief プレイヤーオブジェクト
	CP_Player* pPlayerObject;

	/// @brief のけぞることはできるのか？(アニメーションパラメータ)
	const bool* pCanKnock;

	/// @brief デフォルト状態設定
	BossActState_Base::BossState defaultState;

	/// @brief それぞれの距離を指定する
	std::array<float, static_cast<u_int>(ActDistance::MaxNum)> disLengthList;

	/// @brief 近距離行動のアクションリスト
	std::vector<BossActState_Base::BossState> shortRangeActions;
	
	/// @brief 中距離行動のアクションリスト
	std::vector<BossActState_Base::BossState> midRangeActions;

	/// @brief 遠距離行動のアクションリスト
	std::vector<BossActState_Base::BossState> farRangeActions;
public:
	BossActionController(CP_Boss& _boss);
	~BossActionController() {}

	/// @brief 初期化処理
	/// @param _pAnimation アニメーション
	/// @param _pRigidBody リジッドボディ
	void Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody) override;

	/// @brief ステートを変更
	/// @param _nextState 次のステート
	/// @param _isForce　強制切り替え
	/// @return 成功したか？
	bool ChangeState(BossActState_Base::BossState _nextState, bool _isForce = false);

	/// @brief ダメージ時処理
	void OnDamage(const HashiTaku::AttackInformation& _atkInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos) override;

	/// @brief デバッグ描画
	void DebugDisplay() override;

	/// @brief 乱数で距離のステートを取得
	/// @param _actDistance 距離
	/// @return ボスのステート
	BossActState_Base::BossState RandState(ActDistance _actDistance);

	/// @brief プレイヤーオブジェクトをセット
	/// @param _playerObj プレイヤーオブジェクト
	void SetPlayer(CP_Player& _playerObj);

	/// @brief 攻撃情報をセット
	/// @param _atkInfo 攻撃情報
	void SetAttackInfo(const HashiTaku::AttackInformation& _atkInfo);

	/// @brief ボスコンポーネントを取得
	CP_Boss& GetBoss();

	/// @brief プレイヤーを取得する
	/// @return プレイヤーオブジェクト
	CP_Player& GetPlayer();

	/// @brief のけぞることができるか取得
	/// @return のけぞりできるか？
	const bool GetCanKnock() const;

	/// @brief 行動の距離を取得
	/// @param _actDistance 取得したい距離の種類
	/// @return 距離んお長さ
	float GetActDistanceLength(ActDistance _actDistance);

	/// @brief どの距離か判断する
	/// @param _disLength 距離の長さ
	/// @return どの距離か？
	ActDistance JudgeActDistance(float _disLength) const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 更新処理
	void Update() override;

	/// @brief ボスのステートを作成
	/// @tparam T ボスの行動クラス
	/// @param _createState 作成したいステート
	template<class T>
	void CreateState(BossActState_Base::BossState _createState,
		std::vector<ActDistance> _actDistance);

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
inline void BossActionController::CreateState(BossActState_Base::BossState _createState, std::vector<ActDistance> _actDistance)
{
	// ステートを作成して、リストに追加する
	std::unique_ptr<BossActState_Base> createState = std::make_unique<T>();
	createState->Init(_createState, *this);
	AddNode(static_cast<int>(_createState), std::move(createState));

	// 距離のリストにも追加する
	std::vector<ActDistance> dupplicateCheck;
	u_int disCnt = static_cast<u_int>(_actDistance.size());
	for (u_int d_i = 0; d_i < disCnt; d_i++)
	{
		ActDistance actDis = _actDistance[d_i];

		// 重複チェック
		bool isDupplicate = false;
		for (auto& dupp : dupplicateCheck)
		{
			if (actDis != dupp) continue;

			HASHI_DEBUG_LOG("重複している要素があります");
			isDupplicate = true; break;
		}
		if (isDupplicate) break;

		// 距離のリストに追加する
		switch (actDis)
		{
		case ActDistance::Short:
			shortRangeActions.push_back(_createState);
			break;

		case ActDistance::Mid:
			midRangeActions.push_back(_createState);
			break;

		case ActDistance::Far:
			farRangeActions.push_back(_createState);
			break;

		default:
			break;
		}

		// 重複チェックリストに入れる
		dupplicateCheck.push_back(actDis);
	}
}
