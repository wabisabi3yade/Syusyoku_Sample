#pragma once
#include "StateMachine.h"
#include "PlayerActState_Base.h"
#include "PlayerActChangeObserver.h"
#include "ChangeAnimObserver.h"

class CP_Animation;
class PlayerChangeAnimObserver;

/// @brief プレイヤーの動きコントローラー
class PlayerActionController : public HashiTaku::StateMachine_Base<PlayerActState_Base::PlayerState>, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
private:
	/// @brief ステートから変更した時に通知を受け取るオブザーバー
	std::unique_ptr<PlayerActChangeObserver> pStateChangeObserver;

	/// @brief アニメーション変更したときのオブザーバー
	std::unique_ptr<PlayerChangeAnimObserver> pChangeAnimObserver;

	/// @brief アニメーションのコントローラー
	CP_Animation* pAnimation;

	/// @brief プレイヤーコンポーネント
	CP_Player* pPlayer;

	/// @brief プレイヤーのオブジェクト
	GameObject* pPlayerObject;
public:
	PlayerActionController(CP_Player& _player);
	~PlayerActionController() {}

	/// @brief 開始処理
	/// @param _animationController アニメーション
	void Begin(CP_Animation& _animation);

	/// @brief 現在の更新処理
	void Update();

	/// @brief 行動状態を切り替える
	/// @param _nextActionName 次の状態の名前
	bool ChangeNode(const PlayerActState_Base::PlayerState& _nextActionState) override;

	/// @brief 現在のアクションを取得
	/// @return アクションステート
	PlayerActState_Base* GetCurrentAction();

	/// @brief アニメーション変更オブザーバーを取得
	/// @return アニメーション変更オブザーバー
	PlayerChangeAnimObserver& GetChangeAnimObserver();
	
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 新しくStateを生成
	/// @tparam T 対応している行動クラス
	/// @param _actionName アクション名
	template <class T> void CreateState(PlayerActState_Base::PlayerState _actionState);

	/// @brief PlayerActState_Baseにキャスト
	/// @param _stateNodeBase ステートノード基底変数
	/// @return キャストした変数
	PlayerActState_Base& CastPlayerAct(HashiTaku::StateNode_Base& _stateNodeBase);

	void ImGuiSetting() override;
private:
	// アニメーションコントローラ内のプレイヤー名
	constexpr static auto STATEANIM_PARAMNAME = "state";	// アニメーションの状態変数
};

template<class T>
inline void PlayerActionController::CreateState(PlayerActState_Base::PlayerState _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *pPlayer, *pStateChangeObserver);

	AddNode(_actionState, std::move(createState));
}

/// @brief プレイヤーアクションでアニメーション遷移したときのオブザーバー
class PlayerChangeAnimObserver : public HashiTaku::ChangeAnimObserver
{
	PlayerActionController* pActionController;

public:
	PlayerChangeAnimObserver(PlayerActionController& _playerActCon);
	~PlayerChangeAnimObserver() {}

	/// @brief 通知がきたときの処理
	void ObserverUpdate(const HashiTaku::ChangeAnimationInfo& _value);
};
