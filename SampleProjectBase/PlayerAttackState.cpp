#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerGroundActionController.h"
#include "GameObject.h"

namespace HashiTaku
{
	// 瞬時に見るのを反映する敵との距離
	constexpr float INSTANTLOOK_DISTANCE(8.0f);

	PlayerAttackState::PlayerAttackState() :
		pIsReAttack(nullptr),
		nextCombAtkState(PlayerState::None),
		atkMaxDistance(10.0f),
		curAtkProgressDis(0.0f),
		prevProgressDistance(0.0f),
		curAttackTime(1),
		attackTimeCnt(1),
		isMoveForward(false)
	{

		attackInfos.resize(1);	// 攻撃情報を最低1作成しておく
		PlayerAttackInformation atkInfo;
		attackInfos.push_back(atkInfo);

		// カーブ名をセット
		progressDistanceCurve.SetCurveName("AtkMoveSpd");
	}

	void PlayerAttackState::OnStartBehavior()
	{
		if (!pIsReAttack)
			pIsReAttack = GetAnimation()->GetParameterPointer<bool>(REATTACK_PARAMNAME);

		// 敵の座標を取得
		DXSimp::Vector3 atkPos = GetAtkEnemyPos();

		// 進む距離
		curAtkProgressDis = atkMaxDistance;

		// 距離を見て敵へ向かわせるか判断
		DXSimp::Vector3 vecToEnemy = atkPos - GetMyTransform().GetPosition(); vecToEnemy.y = 0.0f;
		if (pActionController->GetIsTargeting() || vecToEnemy.Length() < INSTANTLOOK_DISTANCE)
		{
			// 進む距離を求める
			CalcProgressDis(atkPos);

			// 敵の方向を見る
			LookAtEnemyInstant(atkPos);
		}

		// 攻撃情報を更新
		UpdateAttackInfo();

		// 攻撃フラグを立てる
		pActionController->SetAnimationTrigger(ATTACKTRIGGER_PARAMNAME);

		// パラメータリセット
		prevProgressDistance = 0.0f;
	}

	void PlayerAttackState::UpdateBehavior()
	{
		// 攻撃時に前進する
		ForwardProgressMove();

		// 攻撃情報を更新するか確認
		UpdateReAttack();

		// コンビネーション攻撃の入力
		UpdateCombInput();
	}

	void PlayerAttackState::OnEndBehavior()
	{
		ClearVelocity(false);
	}

	void PlayerAttackState::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{
		// アニメーションの遷移先が待機状態なら待機に戻す
		if (_toAnimNodeName == IDLE_ANIM_NAME)
			ChangeState(PlayerState::Idle);
	}

	void PlayerAttackState::SetAttackTimes(u_int _attackTimes)
	{
		attackTimeCnt = _attackTimes;
		OnChangeAttackTimes();
	}

	void PlayerAttackState::UpdateAttackInfo()
	{
		assert(curAttackTime <= attackTimeCnt && "攻撃回数が終えています");
		assert(curAttackTime <= static_cast<u_int>(attackInfos.size()) && "攻撃回数が終えています");

		// 現在の攻撃回数目の情報を送る
		GetPlayer().SetAttackInfo(attackInfos[curAttackTime - 1]);
	}

	void PlayerAttackState::UpdateReAttack()
	{
		if (!pIsReAttack) return;	// リアタック変数が取得できていない
		if (!(*pIsReAttack)) return;	// リアタックしないなら処理しない

		// 現在の攻撃回数を次へ進める
		curAttackTime++;
		if (curAttackTime > attackTimeCnt)
		{
			HASHI_DEBUG_LOG("攻撃回数が決められた回数を超えました");
			curAttackTime = attackTimeCnt;
		}

		UpdateAttackInfo();	// 攻撃情報を更新
		GetAnimation()->SetBool(REATTACK_PARAMNAME, false);	// リアタックフラグを降ろす
	}

	void PlayerAttackState::UpdateCombInput()
	{
		if (!pActionController->GetCanInput()) return;	// 入力受け付けていないなら

		if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
			pActionController->SetReserveState(static_cast<int>(nextCombAtkState));
	}

	void PlayerAttackState::CalcProgressDis(const DirectX::SimpleMath::Vector3& _atkEnemyPos)
	{
		if (!isMoveForward) return;

		// 最大距離を超えないに距離を求める
		DXSimp::Vector3 distance = _atkEnemyPos - GetMyTransform().GetPosition();
		distance.y = 0.0f;
		curAtkProgressDis = distance.Length();
		if (curAtkProgressDis > atkMaxDistance)
			curAtkProgressDis = atkMaxDistance;
	}

	void PlayerAttackState::OnChangeAttackTimes()
	{
		// 攻撃情報を更新
		attackInfos.resize(attackTimeCnt);
	}

	PlayerAttackInformation* PlayerAttackState::GetPlayerAtkInfo(u_int _atkIdx)
	{
		// 範囲外なら
		if (static_cast<u_int>(attackInfos.size()) <= _atkIdx)
		{
			HASHI_DEBUG_LOG("エラー：攻撃情報の範囲外を取得しています");
			return nullptr;
		}
			
		return &attackInfos[_atkIdx];
	}

	u_int PlayerAttackState::GetAttackTimes() const
	{
		return attackTimeCnt;
	}

	void PlayerAttackState::ImGuiDebug()
	{
		PlayerGroundState::ImGuiDebug();

		ImGuiCombAttack();

		// 攻撃情報
		ImGui::Text("AtkInfo");
		int imInt = static_cast<int>(attackTimeCnt);	// 回数指定
		if (ImGui::DragInt("attackTime", &imInt, 1, 1, 100))
			SetAttackTimes(attackTimeCnt);

		ImGuiAttackInfo();

		// 前進カーブ
		ImGui::Text(std::to_string(curAtkProgressDis).c_str());
		ImGui::Checkbox("MoveFwd", &isMoveForward);
		if (isMoveForward)
		{
			ImGui::DragFloat("AtkMoveSpeed", &atkMaxDistance, 0.1f);
			progressDistanceCurve.ImGuiCall();
		}
	}

	void PlayerAttackState::ImGuiAttackInfo()
	{
		if (!ImGuiMethod::TreeNode("Attack")) return;

		for (u_int a_i = 0; a_i < attackTimeCnt; a_i++)
		{
			std::string caption = "Step:" + std::to_string(a_i);
			if(!ImGuiMethod::TreeNode(caption)) continue;
			attackInfos[a_i].ImGuiCall();
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	void PlayerAttackState::InitParameter()
	{
		// リアタック変数を取っていないなら
		if (!pIsReAttack)
			pIsReAttack = GetAnimation()->GetParameterPointer<bool>(REATTACK_PARAMNAME);

		curAttackTime = 1;	// 1個目の攻撃情報から
	}

	void PlayerAttackState::LookAtEnemyInstant(DirectX::SimpleMath::Vector3 _atkEnemyPos)
	{
		// 敵の方向を見る
		Transform& trans = GetMyTransform();
		_atkEnemyPos.y = trans.GetPosition().y;	// y軸回転のみする

		trans.LookAt(_atkEnemyPos);
	}

	void PlayerAttackState::ForwardProgressMove()
	{
		using namespace DirectX::SimpleMath;

		float deltaTime = DeltaTime();
		if (deltaTime < Mathf::epsilon) return;

		if (!isMoveForward) return;

		CP_Animation* pAnimation = pActionController->GetAnimation();
		if (!pAnimation) return;

		// カーブから進む料を取得
		float animPlayRatio = pAnimation->GetCurrentPlayRatio();
		float curveVal = progressDistanceCurve.GetValue(animPlayRatio);
		float curDis = curveVal * curAtkProgressDis;
		float curSpeed = (curDis - prevProgressDistance) / deltaTime;

		Transform& transform = GetPlayer().GetTransform();

		// 座標に反映
		GetRB().SetVelocity(transform.Forward() * curSpeed);

		prevProgressDistance = curDis;
	}

	void PlayerAttackState::ImGuiCombAttack()
	{
#ifdef EDIT
		// コンポボックスで変更
		std::string curStateStr = std::string(magic_enum::enum_name(nextCombAtkState));
		bool isChange = ImGuiMethod::ComboBox("NextCombAtk", curStateStr, combAtkState);

		if (isChange)
		{
			// 文字列から列挙型
			auto changeState = magic_enum::enum_cast<PlayerState>(curStateStr);
			if (changeState.has_value())
			{
				nextCombAtkState = changeState.value();
				return;
			}
		}
#endif EDIT
	}

	nlohmann::json PlayerAttackState::Save()
	{
		auto data = PlayerGroundState::Save();

		data["nextCombAttack"] = nextCombAtkState;
		data["maxDistance"] = atkMaxDistance;
		data["attackCnt"] = attackTimeCnt;
		data["isMoveFwd"] = isMoveForward;
		data["forwardCurve"] = progressDistanceCurve.Save();

		// 攻撃情報をセーブ
		auto& attackInfoDatas = data["attackInfos"];
		for (u_int i = 0; i < attackTimeCnt; i++)
		{
			attackInfoDatas.push_back(attackInfos[i].Save());
		}

		return data;
	}

	void PlayerAttackState::Load(const nlohmann::json& _data)
	{
		using namespace HashiTaku;
		PlayerGroundState::Load(_data);

		LoadJsonEnum<PlayerState>("nextCombAttack", nextCombAtkState, _data);
		LoadJsonFloat("maxDistance", atkMaxDistance, _data);
		LoadJsonBoolean("isMoveFwd", isMoveForward, _data);

		// 攻撃回数
		if (LoadJsonUnsigned("attackCnt", attackTimeCnt, _data))
			SetAttackTimes(attackTimeCnt);

		// 攻撃情報をロード
		nlohmann::json attackDatas;
		if (LoadJsonDataArray("attackInfos", attackDatas, _data))
		{
			u_int arrayIdx = 0;

			for (auto& atkInfoData : attackDatas)
			{
				attackInfos[arrayIdx].Load(atkInfoData);
				arrayIdx++;
			}
		}

		nlohmann::json loadData;
		if (LoadJsonData("forwardCurve", loadData, _data))
		{
			progressDistanceCurve.Load(loadData);
		}
	}
}