#include "pch.h"
#include "PlayerAttackState.h"
#include "PlayerActionController.h"
#include "GameObject.h"

namespace DXSimp = DirectX::SimpleMath;

// 瞬時に見るのを反映する敵との距離
constexpr float INSTANTLOOK_DISTANCE(4.0f);

PlayerAttackState::PlayerAttackState() :
	pIsReAttack(nullptr),
	nextCombAtkState(PlayerState::None),
	curChangeAtkState(PlayerState::None),
	atkMaxDistance(10.0f),
	lookRotateSpeed(0.0f),
	curAtkProgressDis(0.0f),
	prevProgressDistance(0.0f),
	curAttackTime(1),
	attackTimeCnt(1),
	isMoveForward(false),
	isAttackCollisionBefore(false)
{

	attackInfos.resize(1);	// 攻撃情報を最低1作成しておく
	HashiTaku::AttackInformation atkInfo;
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
	DXSimp::Vector3 vecToEnemy = atkPos - GetTransform().GetPosition(); vecToEnemy.y = 0.0f;
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
	curChangeAtkState = PlayerState::None;
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

void PlayerAttackState::TransitionCheckUpdate()
{
	// 攻撃入力されたらステート遷移する
	if (GetCanCombAttack())
		ChangeState(curChangeAtkState);

	PlayerActState_Base::TransitionCheckUpdate();
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

	if (IsSpecialAtkInput(InputVector::Forward))
		curChangeAtkState = PlayerState::SpecialAtkHi;

	else if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
		curChangeAtkState = nextCombAtkState;
}

void PlayerAttackState::CalcProgressDis(const DirectX::SimpleMath::Vector3& _atkEnemyPos)
{
	if (!isMoveForward) return;

	// 最大距離を超えないに距離を求める
	DXSimp::Vector3 distance = _atkEnemyPos - GetTransform().GetPosition();
	distance.y = 0.0f;
	curAtkProgressDis = distance.Length();
	if (curAtkProgressDis > atkMaxDistance)
		curAtkProgressDis = atkMaxDistance;
}

void PlayerAttackState::ImGuiDebug()
{
	PlayerActState_Base::ImGuiDebug();

	ImGuiComboAttack("nextCombo", nextCombAtkState);

	// 攻撃情報
	ImGui::Text("AtkInfo");
	int imInt = static_cast<int>(attackTimeCnt);	// 回数指定
	if (ImGui::DragInt("attackTime", &imInt, 1, 1, 100))
	{
		attackTimeCnt = static_cast<u_int>(imInt);
		attackInfos.resize(attackTimeCnt);
	}

	for (u_int a_i = 0; a_i < attackTimeCnt; a_i++)
	{
		ImGui::PushID(a_i);
		attackInfos[a_i].ImGuiCall();
		ImGui::PopID();
	}

	// 前進カーブ
	ImGui::Text(std::to_string(curAtkProgressDis).c_str());
	ImGui::Checkbox("MoveFwd", &isMoveForward);
	if (isMoveForward)
	{
		ImGui::DragFloat("AtkMoveSpeed", &atkMaxDistance, 0.1f);
		progressDistanceCurve.ImGuiCall();
	}
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
	Transform& trans = GetTransform();
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

bool PlayerAttackState::GetCanCombAttack()
{
	if (curChangeAtkState == PlayerState::None) return false;
	if (!pActionController->GetCanAttack()) return false;

	return true;
}

DirectX::SimpleMath::Vector3 PlayerAttackState::GetAtkEnemyPos()
{
	CP_BattleManager* pBattle = CP_BattleManager::GetInstance();
	if (!pBattle) return DXSimp::Vector3::Zero;

	// 敵リストを取得する
	const auto& enemyList = pBattle->GetEnemyList();

	// 敵がいないなら
	if (static_cast<u_int>(enemyList.size()) == 0) return DXSimp::Vector3::Zero;

	DXSimp::Vector3 atkEnemyPos;
	ITargetAccepter* pAtkEnemy = nullptr;
	// ターゲット先がいるなら
	if (pAtkEnemy = pActionController->GetTargetObject())
	{
		atkEnemyPos = pAtkEnemy->GetWorldPosByTargetObj();
	}
	else if (pAtkEnemy = (*enemyList.begin()))
	{
		// 本来は一番近くの敵を取得する
		atkEnemyPos = pAtkEnemy->GetWorldPosByTargetObj();
	}

	return atkEnemyPos;
}

nlohmann::json PlayerAttackState::Save()
{
	auto data = PlayerActState_Base::Save();

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
	PlayerActState_Base::Load(_data);

	LoadJsonEnum<PlayerState>("nextCombAttack", nextCombAtkState, _data);
	LoadJsonFloat("maxDistance", atkMaxDistance, _data);
	LoadJsonUnsigned("attackCnt", attackTimeCnt, _data);
	LoadJsonBoolean("isMoveFwd", isMoveForward, _data);

	// 攻撃情報をロード
	nlohmann::json attackDatas;
	if (HashiTaku::LoadJsonDataArray("attackInfos", attackDatas, _data))
	{
		u_int arrayIdx = 0;
		attackInfos.clear();
		attackInfos.resize(attackTimeCnt);

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
