#pragma once
#include "CharacterActionController.h"
#include "PlayerActState_Base.h"
#include "ChangeAnimObserver.h"
#include "ITargetAccepter.h"

class PlayerChangeAnimObserver;
class CP_Player;
class CP_BattleManager;

/// @brief プレイヤーの動きコントローラー
class PlayerActionController : public CharacterActionController, public IObjectTargeter
{
private:
	/// @brief 入力クラス
	GameInput* pInput;

	/// @brief バトルマネジャー
	CP_BattleManager* pBattleManager;

	/// @brief ターゲットしているオブジェクト先
	ITargetAccepter* pTargetObject;

	/// @brief キャンセルフラグのポインタ
	const bool* pIsCanCancel;

	/// @brief 入力可能フラグのポインタ
	const bool* pIsCanInput;

	/// @brief コンビネーション攻撃可能のポインタ
	const bool* pIsCanCombAtk;

	/// @brief ターゲット中かどうか
	bool isTargeting;

	/// @brief 1フレーム前のターゲット状態
	bool prevIsTargeting;
public:
	/// @brief コンストラクタ
	/// @param _player プレイヤーコンポーネント
	PlayerActionController(CP_Player& _player);
	~PlayerActionController() {}

	/// @brief 開始処理
	/// @param _animation アニメーション
	/// @param _pRigidBody リジッドボディ
	void Init(CP_Animation* _animation, CP_RigidBody* _pRigidBody);

	/// @brief ターゲットの更新処理
	void UpdateTargeting();

	/// @brief 行動状態を切り替える
	/// @param _nextActionName 次の状態の名前
	bool ChangeState(const PlayerActState_Base::PlayerState& _nextActionState);

	/// @brief ターゲット中か取得する
	/// @return ターゲット中か？
	bool GetIsTargeting() const;

	/// @brief 1フレーム前のターゲット中か取得する
	/// @return ターゲット中か？
	bool GetIsPrevTargeting() const;

	/// @brief キャンセルできるか取得
	/// @return キャンセルできるか？
	bool GetIsCanCancel() const;

	/// @brief 先行入力できるかを取得
	/// @return 先行入力できるか？
	bool GetCanInput() const;

	/// @brief コンビネーション攻撃できるかを取得
	/// @return コンビネーション攻撃できるか？
	bool GetCanCombAtk() const;

	/// @brief プレイヤーコンポーネント取得
	/// @return プレイヤーコンポーネント
	CP_Player& GetPlayer();

	/// @brief 現在のアクションを取得
	/// @return アクションステート
	PlayerActState_Base* GetCurrentAction();

	/// @brief 各Stateの文字列を取得する
	/// @param _stateId 状態のID
	/// @return 文字列
	std::string GetStateStr(int _stateId) override;

	/// @brief 各StateのIDを取得する
	/// @param _stateName　状態名
	/// @return 状態のID
	int GetStateId(const std::string& _stateName);

	/// @brief ターゲットにしているオブジェクトを取得
	/// @return ターゲットにしているオブジェクト
	ITargetAccepter* GetTargetObject();

	/// @brief ターゲットオブジェクトを取得する
	/// @param _targetObject ターゲットオブジェクト
	void SetTargetObject(ITargetAccepter& _targetObject) override;

	/// @brief ターゲットオブジェクトが死んだときの更新処理
	/// @param _deathTargetObj 死んだターゲットオブジェクト
	void UpdateDeathNotify(const ITargetAccepter& _deathTargetObj) override;
private:
	/// @brief 更新できるか取得
	/// @return 更新できるか？
	bool GetCanUpdate();

	/// @brief 現在の更新処理
	void Update() override;

	/// @brief  ターゲット開始時のお処理
	void OnBeginTargeting();

	/// @brief ターゲット終了時の処理
	void OnEndTargeting();

	/// @brief 新しくStateを生成
	/// @tparam T 対応している行動クラス
	/// @param _actionName アクション名
	template <class T> void CreateState(PlayerActState_Base::PlayerState _actionState);

	/// @brief PlayerActState_Baseにキャスト
	/// @param _stateNodeBase ステートノード基底変数
	/// @return キャストした変数
	PlayerActState_Base& CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase);

	void ImGuiDebug() override;
private:
	/// @brief アニメーションコントローラ内のプレイヤー名
	static constexpr  auto STATEANIM_PARAMNAME{ "state" };

	/// @brief ターゲット中を表すアニメーションパラメータ
	static constexpr auto TARGET_PARAMNAME{ "targeting" };

	/// @brief キャンセルできるかを表すアニメーションパラメータ
	static constexpr auto CANCEL_PARAMNAME{ "canCancel" };

	/// @brief 先行入力できるかを表すアニメーションパラメータ
	static constexpr auto INPUT_PARAMNAME{ "canInput" };

	/// @brief コンビネーション攻撃できるかを表すアニメーションパラメータ
	static constexpr auto COMBATK_PARAMNAME{ "canCombAttack" };
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::PlayerState _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *this);

	AddNode(static_cast<int>(_actionState), std::move(createState));
}
