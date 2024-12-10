#pragma once
#include "PlayerActionController_Base.h"
#include "PlayerAirState.h"

/// @brief プレイヤーの空中行動を継承しているかコンセプト
template<typename T>
concept PlayerAirConcept =
std::is_base_of_v<PlayerAirState, T>;

/// @brief プレイヤー空中アクションコントローラー
class PlayerAirActionController : public PlayerActionController_Base
{
public:
	using AirState = PlayerAirState::PlayerState;

private:
	/// @brief 下に力を加えるか？
	bool isDownForce;

	/// @brief 下向きにかける力（重力だと、横方向の大きさも）
	float downForcePower;

public:
	/// @brief コンストラクタ
	/// @param _pAction アクション
	/// @param _player プレイヤー
	PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player);
	~PlayerAirActionController() {}

	/// @brief 更新処理
	void Update() override;

	/// @brief 行動状態を切り替える
	/// @param _nextActionName 次の状態の名前
	/// @param _isForce 強制で切り替えするか？
	bool ChangeAirState(const  AirState& _nextActionState, bool _isForce = false);

	/// @brief 下に力を加えるかセットする
	/// @param _isDown 力を加えるか？
	void SetIsDownForce(bool _isDown);

	/// @brief 各Stateの文字列を取得する
	/// @param _stateId 状態のID
	/// @return 文字列
	std::string GetStateStr(int _stateId) override;

	/// @brief 各StateのIDを取得する
	/// @param _stateName　状態名
	/// @return 状態のID
	int GetStateId(const std::string& _stateName) override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	template<PlayerAirConcept T>
	void CreateState(AirState _actionState);

	/// @brief 理想の落下にするために下向きの力を加える
	void AddDownForce();

	void ImGuiDebug() override;
};

template<PlayerAirConcept T>
inline void PlayerAirActionController::CreateState(AirState _actionState)
{
	PlayerActionController_Base::CreateState<T>(static_cast<int>(_actionState));
}