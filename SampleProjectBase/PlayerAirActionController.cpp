#include "pch.h"
#include "PlayerAirActionController.h"
#include "PlayerAction.h"
#include "PlayerAirMove.h"
#include "PlayerAirSlashHigh.h"
#include "PlayerAirAttack.h"

namespace HashiTaku
{
	PlayerAirActionController::PlayerAirActionController(PlayerAction& _pAction, CP_Player& _player) :
		PlayerActionController_Base(_pAction, _player, "playerAirController"),
		downForcePower(0.0f),
		isDownForce(true)
	{
		place = ActionPlace::Air;

		// 行動クラスを生成
		using enum AirState;
		CreateState<PlayerAirMove>(Move, CancelType::Move);
		CreateState<PlayerAirSlashHigh>(SlashHigh_Air, CancelType::None);
		//CreateState<PlayerAirAttack>(Attack11);

		// デフォルト状態をセット
		SetDefaultNode(static_cast<int>(Move));
	}

	void PlayerAirActionController::Update()
	{
		// 下向きに力を加える
		AddDownForce();

		PlayerActionController_Base::Update();
	}

	bool PlayerAirActionController::ChangeAirState(const AirState& _nextActionState, bool _isForce)
	{
		int _nextId = static_cast<int>(_nextActionState);
		// 遷移する
		// 失敗したら処理終了
		if (!ChangeState(static_cast<int>(_nextActionState), _isForce))
			return false;

		// 下に落ちるようにするか？
		PlayerAirState* pNextState = GetAirState(_nextActionState);
		isDownForce = pNextState->GetIsApplyDownForce();

		return true;
	}

	std::string PlayerAirActionController::GetStateStr(int _stateId)
	{
		AirState state = static_cast<AirState>(_stateId);
		std::string str = std::string(magic_enum::enum_name<AirState>(state));

		return str;
	}

	int PlayerAirActionController::GetStateId(const std::string& _stateName)
	{
		auto state = magic_enum::enum_cast<AirState>(_stateName);
		if (!state.has_value())
		{
			assert(!"正常に状態を取得できません");
			return -1;
		}

		return static_cast<int>(state.value());
	}

	PlayerAirState* PlayerAirActionController::GetAirState(AirState _getState)
	{
		PlayerAirState* pGetState = nullptr;
		int stateId = static_cast<int>(_getState);
#ifdef EDIT
		pGetState = dynamic_cast<PlayerAirState*>(GetNode(stateId));
		assert(pGetState && "ステートが正常に取得できませんでした");
#else
		pGetState = static_cast<PlayerAirState*>(GetNode(stateId));
#endif // EDIT

		return pGetState;
	}

	json PlayerAirActionController::Save()
	{
		auto data = PlayerActionController_Base::Save();

		data["downForcePower"] = downForcePower;

		return data;
	}

	void PlayerAirActionController::Load(const json& _data)
	{
		PlayerActionController_Base::Load(_data);
		LoadJsonFloat("downForcePower", downForcePower, _data);
	}

	void PlayerAirActionController::AddDownForce()
	{
		if (!pRigidBody) return;


		// 下向きに力を加える
		pRigidBody->AddForce(Vec3::Up * downForcePower);
	}

	void PlayerAirActionController::ImGuiDebug()
	{
		PlayerActionController_Base::ImGuiDebug();

		ImGui::DragFloat("DownPower", &downForcePower, 0.01f);
	}
}