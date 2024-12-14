#include "pch.h"
#include "BossCombAttack.h"
#include "BossActionController.h"

//BossCombAttack::BossCombAttack() : 
//	pIsReAttack(nullptr), attackTimeCnt(1), curAttackTime(1)
//{
//}
//
//void BossCombAttack::OnStartBehavior()
//{
//	BossAttackState::OnStartBehavior();
//}
//
//void BossCombAttack::UpdateBehavior()
//{
//	BossAttackState::UpdateBehavior();
//
//	UpdateReAttack();
//}
//
//nlohmann::json BossCombAttack::Save()
//{
//	auto data = BossAttackState::Save();
//
//	data["atkTimeCnt"] = attackTimeCnt;
//
//	return data;
//}
//
//void BossCombAttack::Load(const nlohmann::json& _data)
//{
//	BossAttackState::Load(_data);
//
//	HashiTaku::LoadJsonUnsigned("atkTimeCnt", attackTimeCnt, _data);
//}
//
//void BossCombAttack::UpdateReAttack()
//{
//	// リアタックのタイミングでないなら
//	if (!(*pIsReAttack)) return;
//
//	// リアタックフラグを降ろす
//	GetAnimation()->SetBool(REATTACK_PARAMNAME, false);
//
//	// リアタック更新
//	curAttackTime++;
//
//	// 総攻撃回数を超えるなら
//	if(curAttackTime > attackTimeCnt)	
//	{
//		HASHI_DEBUG_LOG("エラー：総攻撃回数を超えています");
//		curAttackTime = attackTimeCnt;
//	}
//
//	assert(static_cast<u_int>(attackInfos.size()) >= curAttackTime && 
//		"攻撃情報が攻撃回数以下です");
//
//	// 対応した攻撃情報をセットする
//	pActionController->SetAttackInfo(attackInfos[curAttackTime - 1]);
//}
//
//void BossCombAttack::SetAttackTimeCnt(u_int _attackTime)
//{
//#ifdef EDIT
//	if (_attackTime == 0)
//	{
//		HASHI_DEBUG_LOG("1以上を設定してください");
//		return;
//	}
//#endif // EDIT
//
//	attackTimeCnt = _attackTime;
//	attackInfos.resize(_attackTime);	// 攻撃情報も合わせる
//}
//
//void BossCombAttack::ImGuiDebug()
//{
//	BossAttackState::ImGuiDebug();
//	// 攻撃回数
//	int imInt = static_cast<u_int>(attackTimeCnt);
//	if (ImGui::DragInt("attackCnt", &imInt, 1, 1, 100))
//	{
//		SetAttackTimeCnt(imInt);
//	}
//}
