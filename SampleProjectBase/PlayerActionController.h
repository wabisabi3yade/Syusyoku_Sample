#pragma once
#include "PlayerActState_Base.h"
#include "PlayerActChangeObserver.h"

class CP_Animation;

/// @brief プレイヤーの動きコントローラー
class PlayerActionController : HashiTaku::IImGuiUser, HashiTaku::ISaveLoad
{
private:
	/// @brief プレイヤーの行動状態リスト
	std::unordered_map<PlayerActState_Base::StateType, std::unique_ptr<PlayerActState_Base>> actionList;

	/// @brief ステートから変更した時に通知を受け取るオブザーバー
	std::unique_ptr<PlayerActChangeObserver> pStateChangeObserver;

	/// @brief 現在の行動クラス
	PlayerActState_Base* pCurrentState;

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
	/// @param _nextState 次の状態
	void ChangeState(PlayerActState_Base::StateType _nextState);

	/// @brief 初期の状態をセット
	/// @param _defaultState 初期状態のステート
	void DefaultState(PlayerActState_Base::StateType _defaultState);

	void ImGuiSetting() override;
	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 新しくStateを生成
	/// @tparam T 対応している行動クラス
	template <class T> void CreateState();

private:
	// アニメーションコントローラ内のプレイヤー名
	constexpr static auto STATE_PARAMNAME = "state";	// アニメーションの状態変数
};

template<class T>
inline void PlayerActionController::CreateState()
{
	std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
	createState->Init(*pPlayerObject, *pStateChangeObserver);

	PlayerActState_Base::StateType state = createState->GetActStateType();
	actionList[state] = std::move(createState);
}
