#include "pch.h"
#include "PlayerAirAttack.h"
#include "CP_Player.h"

namespace DXSimp = DirectX::SimpleMath;

// 瞬時に見るのを反映する敵との距離
constexpr float INSTANTLOOK_DISTANCE(8.0f);

PlayerAirAttack::PlayerAirAttack() :
	nextCombAtkState(PlayerState::None),
	attackTimeCnt(1)
{
	attackInfos.resize(1);	// 攻撃情報を最低1作成しておく
	HashiTaku::AttackInformation atkInfo;
	attackInfos.push_back(atkInfo);
}

void PlayerAirAttack::OnStartBehavior()
{
	// 範囲内なら敵を見る
	LookAtEnemy();

	// 攻撃情報を更新
	UpdateAttackInfo();

	// 攻撃フラグを立てる
	pActionController->SetAnimationTrigger(ATTACKTRIGGER_PARAMNAME);

	// 下に落ちなくする
	CastAirController().SetIsDownForce(false);
	ClearVelocity(true);
}

void PlayerAirAttack::UpdateBehavior()
{
	// コンビネーション攻撃の入力
	UpdateCombInput();

	// y軸速度だけ0に
	ClearVelocityY();
}

void PlayerAirAttack::OnEndBehavior()
{
	// 落ちるようにする
	CastAirController().SetIsDownForce(true);
}

void PlayerAirAttack::OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
{
	if (_toAnimNodeName == FALLLOOP_ANIM_NAME)
		ChangeState(PlayerState::Move);
}

void PlayerAirAttack::UpdateCombInput()
{
	if (!pActionController->GetCanInput()) return;	// 入力受け付けていないなら

	// 予約状態にセット
	if (pPlayerInput->GetButtonDown(GameInput::ButtonType::Player_Attack))
		 pActionController->SetReserveState(
			 static_cast<int>(nextCombAtkState), 
			 PlayerActionController_Base::CancelType::Attack
		 );
}

void PlayerAirAttack::LookAtEnemy()
{
	Transform& playerTrans = GetMyTransform();

	// 敵座標とのベクトルを取得
	DXSimp::Vector3 atkPos = GetAtkEnemyPos();
	DXSimp::Vector3 vecToEnemy = atkPos - playerTrans.GetPosition();

	// 向く範囲内でないなら処理しない
	if (vecToEnemy.Length() > INSTANTLOOK_DISTANCE)
		return;

	// 敵の方向を見る(y軸回転のみする)
	atkPos.y = playerTrans.GetPosition().y;	
	playerTrans.LookAt(atkPos);
}

void PlayerAirAttack::UpdateAttackInfo()
{
	// 現在の攻撃回数目の情報を送る
	GetPlayer().SetAttackInfo(attackInfos[0]);
}

void PlayerAirAttack::ClearVelocityY()
{
	CP_RigidBody& rb = GetRB();
	rb.ClearVelocity();
}
