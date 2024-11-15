#pragma once
#include "CharacterActionController.h"
#include "PlayerActState_Base.h"
#include "ChangeAnimObserver.h"
#include "ITargetAccepter.h"

class PlayerChangeAnimObserver;
class CP_Player;

/// @brief プレイヤーの動きコントローラー
class PlayerActionController : public CharacterActionController, public IObjectTargeter
{
private:
	/// @brief 入力クラス
	GameInput* pInput;

	/// @brief ターゲットしているオブジェクト先
	ITargetAccepter* pTargetObject;

	/// @brief キャンセルフラグのポインタ
	const bool* pIsCanCancel;

	/// @brief 先行入力フラグのポインタ
	const bool* pIsSenkoInput;

	/// @brief ターゲット中かどうか
	bool isTargeting;
public:
	/// @brief コンストラクタ
	/// @param _player プレイヤーコンポーネント
	PlayerActionController(CP_Player& _player);
	~PlayerActionController() {}

	/// @brief 開始処理
	/// @param _animation アニメーション
	void Init(CP_Animation* _animation);

	/// @brief ターゲットの更新処理
	void UpdateTargeting();

	/// @brief 行動状態を切り替える
	/// @param _nextActionName 次の状態の名前
	bool ChangeState(const PlayerActState_Base::PlayerState& _nextActionState);

	/// @brief ターゲット中か取得する
	/// @return ターゲット中か？
	bool GetIsTargeting() const;

	/// @brief キャンセルできるか取得
	/// @return キャンセルできるか？
	bool GetIsCanCancel() const;

	/// @brief 先行入力できるかを取得
	/// @return 先行入力できるか？
	bool GetCanInput() const;

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

	/// @brief ターゲットオブジェクトを取得する
	/// @param _targetObject ターゲットオブジェクト
	void GetTargetObject(ITargetAccepter& _targetObject);

	/// @brief ターゲットが死んだときの処理
	void OnTargetDeath();
private:
	/// @brief 現在の更新処理
	void Update() override;

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
	static constexpr auto CANCEL_PARAMNAME = "canCancel";

	/// @brief 先行入力できるかを表すアニメーションパラメータ
	static constexpr auto SENKOINPUT_PARAMNAME = "canInput";
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::PlayerState _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *this);

	AddNode(static_cast<int>(_actionState), std::move(createState));
}
