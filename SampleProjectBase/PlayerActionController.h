#pragma once
#include "StateMachine.h"
#include "PlayerActState_Base.h"
#include "PlayerActChangeObserver.h"

class CP_Animation;

/// @brief プレイヤーの動きコントローラー
class PlayerActionController : public HashiTaku::StateMachine_Base<PlayerActState_Base::StateType>, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
private:
	/// @brief ステートから変更した時に通知を受け取るオブザーバー
	std::unique_ptr<PlayerActChangeObserver> pStateChangeObserver;

	/// @brief アニメーションのコントローラー
	CP_Animation* pAnimation;

	/// @brief プレイヤーのオブジェクト
	GameObject* pPlayerObject;
public:
	PlayerActionController(GameObject& _pPlayerObject);
	~PlayerActionController() {}

	/// @brief 開始処理
	/// @param _animationController アニメーション
	void Begin(CP_Animation& _animation);

	/// @brief 現在の更新処理
	void Update();

	/// @brief 行動状態を切り替える
	/// @param _nextActionName 次の状態の名前
	void ChangeNode(const PlayerActState_Base::StateType& _nextActionState) override;

	
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 新しくStateを生成
	/// @tparam T 対応している行動クラス
	/// @param _actionName アクション名
	template <class T> void CreateState(PlayerActState_Base::StateType _actionState);

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
inline void PlayerActionController::CreateState(PlayerActState_Base::StateType _actionState)
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(_actionState, *pPlayerObject, *pStateChangeObserver);

	AddNode(_actionState, std::move(createState));
}
