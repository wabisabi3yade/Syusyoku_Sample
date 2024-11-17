#include "pch.h"
#include "PlayerRushAttack.h"
#include "GameObject.h"

namespace DXSimp = DirectX::SimpleMath;

constexpr float BEGIN_JUMP_ANIMRATIO(0.380f);	// ジャンプアニメーション割合
constexpr float BEGIN_SLASH_ANIMRATIO(0.465f);	// 斬り始めるアニメーション割合

PlayerRushAttack::PlayerRushAttack() :
	pTargetObj(nullptr), progressLengthHori(5.0f), prevDistanceHori(0.0f),
	slashBeginDistance(0.7f), isProgressForward(false)
{
	distanceHoriCurve.SetCurveName("Horizon");
}

nlohmann::json PlayerRushAttack::Save()
{
	auto data = PlayerGroundAttack::Save();

	data["slashBegin"] = slashBeginDistance;
	data["horiDis"] = progressLengthHori;
	data["horiCurve"] = distanceHoriCurve.Save();

	return data;
}

void PlayerRushAttack::Load(const nlohmann::json& _data)
{
	PlayerGroundAttack::Load(_data);

	HashiTaku::LoadJsonFloat("slashBegin", slashBeginDistance, _data);
	HashiTaku::LoadJsonFloat("horiDis", progressLengthHori, _data);
	if (HashiTaku::IsJsonContains(_data, "horiCurve"))
	{
		distanceHoriCurve.Load(_data["horiCurve"]);
	}
}

void PlayerRushAttack::OnStartBehavior()
{
	PlayerGroundAttack::OnStartBehavior();

	// ターゲット先を取得する
	pTargetObj = pActionController->GetTargetObject();

	// リセット
	prevDistanceHori = 0.0f;
	isProgressForward = true;
}

void PlayerRushAttack::UpdateBehavior()
{
	using namespace DirectX::SimpleMath;

	PlayerGroundAttack::UpdateBehavior();

	// 前に突進
	MoveForward();
}

void PlayerRushAttack::TransitionCheckUpdate()
{
	PlayerGroundAttack::TransitionCheckUpdate();
}

void PlayerRushAttack::MoveForward()
{
	// 前に進まないなら
	if (!isProgressForward) return;
	
	float deltaTime = DeltaTime();
	if (deltaTime < Mathf::epsilon) return;	// Δtが0なら

	Transform& playerTrans = pActionController->GetPlayer().GetGameObject().GetTransform();

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
		pAnimation->SetPlayRatio(BEGIN_SLASH_ANIMRATIO);

		GetRB().SetVelocity(DXSimp::Vector3::Zero);
	}
}

bool PlayerRushAttack::CheckDistanceToEnemy(float _currentPlayRatio)
{
	if (!pTargetObj) return false;

	// ジャンプして斬り始める間だけチェック
	if (_currentPlayRatio < BEGIN_JUMP_ANIMRATIO) return false;
	if (_currentPlayRatio > BEGIN_SLASH_ANIMRATIO) return false;

	DXSimp::Vector3 distance = GetTransform().GetPosition() -
		pTargetObj->GetWorldPosByTargetObj();
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
	PlayerGroundAttack::ImGuiDebug();

	ImGui::DragFloat("SlashBeginDis", &slashBeginDistance, 0.01f, 0.0f, 1000.0f);

	ImGui::DragFloat("HoriDis", &progressLengthHori, 0.1f, 0.0f, 1000.0f);
	distanceHoriCurve.ImGuiCall();
}
