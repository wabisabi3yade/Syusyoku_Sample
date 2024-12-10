#include "pch.h"
#include "PlayerRushAttack.h"
#include "GameObject.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr float BEGIN_JUMP_ANIMRATIO(0.380f);	// ジャンプアニメーション割合
constexpr float BEGIN_SLASH_ANIMRATIO(0.465f);	// 斬り始めるアニメーション割合

PlayerRushAttack::PlayerRushAttack() :
	pTargetObj(nullptr), 
	progressLengthHori(5.0f),
	prevDistanceHori(0.0f),
	slashBeginDistance(0.7f), 
	cancelBeginRatio(0.380f),
	slashBeginAnimRatio(0.465f),
	isProgressForward(false)
{
	distanceHoriCurve.SetCurveName("Horizon");
}

nlohmann::json PlayerRushAttack::Save()
{
	auto data = PlayerAttackState::Save();

	data["slashBegin"] = slashBeginDistance;
	data["horiDis"] = progressLengthHori;
	data["horiCurve"] = distanceHoriCurve.Save();
	data["cancelAnim"] = cancelBeginRatio;
	data["slashAnim"] = slashBeginAnimRatio;

	return data;
}

void PlayerRushAttack::Load(const nlohmann::json& _data)
{
	PlayerAttackState::Load(_data);

	HashiTaku::LoadJsonFloat("slashBegin", slashBeginDistance, _data);
	HashiTaku::LoadJsonFloat("horiDis", progressLengthHori, _data);
	HashiTaku::LoadJsonFloat("cancelAnim", cancelBeginRatio, _data);
	HashiTaku::LoadJsonFloat("slashAnim", slashBeginAnimRatio, _data);
	if (HashiTaku::IsJsonContains(_data, "horiCurve"))
	{
		distanceHoriCurve.Load(_data["horiCurve"]);
	}
}

void PlayerRushAttack::OnStartBehavior()
{
	PlayerAttackState::OnStartBehavior();

	// ターゲット先を取得する
	pTargetObj = pActionController->GetTargetAccepter();

	// リセット
	prevDistanceHori = 0.0f;
	isProgressForward = true;
}

void PlayerRushAttack::UpdateBehavior()
{
	using namespace DirectX::SimpleMath;

	PlayerAttackState::UpdateBehavior();

	// 前に突進
	MoveForward();
}

void PlayerRushAttack::TransitionCheckUpdate()
{
	PlayerAttackState::TransitionCheckUpdate();
}

void PlayerRushAttack::MoveForward()
{
	// 前に進まないなら
	if (!isProgressForward) return;
	
	float deltaTime = DeltaTime();
	if (deltaTime < Mathf::epsilon) return;	// Δtが0なら

	Transform& playerTrans = GetPlayer().GetGameObject().GetTransform();

	// アニメーションの割合を取得する
	CP_Animation* pAnimation = pActionController->GetAnimation();
	if (!pAnimation) return;
	float animPlayRatio = pAnimation->GetCurrentPlayRatio();

	// カーブから進む距離を取得
	float horiDistance = progressLengthHori * distanceHoriCurve.GetValue(animPlayRatio);

	// 前回からの差分を求める
	float diffHoriDis = horiDistance - prevDistanceHori;

	// 移動速度を求める
	float horiSpeed = diffHoriDis / deltaTime;

	// 進める
	GetRB().SetVelocity(playerTrans.Forward() * horiSpeed);

	// 次フレームの為に更新
	prevDistanceHori = horiDistance;

	// 敵との一定距離に入ったら
	if (CheckDistanceToEnemy(animPlayRatio))
	{
		// 前に進まなくする
		isProgressForward = false;

		// アニメーションを斬り始めるようにする
		pAnimation->SetPlayRatio(slashBeginAnimRatio);

		GetRB().SetVelocity(DXSimp::Vector3::Zero);
	}
}

bool PlayerRushAttack::CheckDistanceToEnemy(float _currentPlayRatio)
{
	if (!pTargetObj) return false;

	// ジャンプして斬り始める間だけチェック
	if (_currentPlayRatio < cancelBeginRatio) return false;
	if (_currentPlayRatio > slashBeginAnimRatio) return false;

	DXSimp::Vector3 distance = GetMyTransform().GetPosition() -
		pTargetObj->GetWorldPos();
	distance.y = 0.0f;	// yは考慮しない

	if (distance.Length() > slashBeginDistance) return false;

	return true;
}

bool PlayerRushAttack::isExistTrgeter() const
{
	return pTargetObj != nullptr;
}

void PlayerRushAttack::ImGuiDebug()
{
	PlayerAttackState::ImGuiDebug();

	ImGui::DragFloat("SlashBeginDis", &slashBeginDistance, 0.01f, 0.0f, 1000.0f);
	ImGui::DragFloat("HoriDis", &progressLengthHori, 0.1f, 0.0f, 1000.0f);
	ImGui::DragFloat("cancelBeginRatio", &cancelBeginRatio, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("slashRatio", &slashBeginAnimRatio, 0.001f, 0.0f, 1.0f);


	distanceHoriCurve.ImGuiCall();
}
