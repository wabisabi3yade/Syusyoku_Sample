#include "pch.h"
#include "BossJumpAttack.h"

#include "Geometory.h"

namespace DXSimp = DirectX::SimpleMath;

// 正面ベクトルとプレイヤーとの角度がこの値以下なら瞬時に向く
constexpr float LOOKMOMENT_DEGREE_RANGE{ 20.0f };

// 横移動カーブの値がこれ以上だと瞬時に振り向くことができる
constexpr float CAN_LOOKMOMENT_HORICURVE_VAL{ 0.5f };

BossJumpAttack::BossJumpAttack() :
	maxHoriSpeed(12.0f), 
	maxJumpHeight(5.0f),
	prevJumpHeight(0.0f),
	horiSpeedCofficient(2.0f),
	fallPosOffset(0.0f)
{
}

void BossJumpAttack::OnStartBehavior()
{
	BossAttackState::OnStartBehavior();

	prevJumpHeight = 0.0f;
}

void BossJumpAttack::UpdateBehavior()
{
	BossAttackState::UpdateBehavior();

	// ジャンプの動き
	JumpMove();
}

void BossJumpAttack::DebugDisplay()
{
#ifdef EDIT
	const Transform& t = GetBossTransform();
	Geometory::SetPosition(t.GetPosition() + t.Forward() * fallPosOffset);
	Geometory::SetScale(DXSimp::Vector3::One * 0.1f);
	Geometory::DrawSphere();
#endif
}

nlohmann::json BossJumpAttack::Save()
{
	auto data = BossAttackState::Save();

	data["jumpHeight"] = maxJumpHeight;
	data["jumpHeightCurve"] = jumpHeightDisCurve.Save();

	data["fallOffset"] = fallPosOffset;
	data["maxHoriSpeed"] = maxHoriSpeed;
	data["speedCof"] = horiSpeedCofficient;
	data["jumpHoriCurve"] = horiSpeedCofficientCurve.Save();

	return data;
}

void BossJumpAttack::Load(const nlohmann::json& _data)
{
	BossAttackState::Load(_data);

	HashiTaku::LoadJsonFloat("fallOffset", fallPosOffset, _data);
	HashiTaku::LoadJsonFloat("jumpHeight", maxJumpHeight, _data);
	HashiTaku::LoadJsonFloat("maxHoriSpeed", maxHoriSpeed, _data);
	HashiTaku::LoadJsonFloat("speedCof", horiSpeedCofficient, _data);

	nlohmann::json curveData;
	if (HashiTaku::LoadJsonData("jumpHeightCurve", curveData, _data))
	{
		jumpHeightDisCurve.Load(curveData);
	}
	if (HashiTaku::LoadJsonData("jumpHoriCurve", curveData, _data))
	{
		horiSpeedCofficientCurve.Load(curveData);
	}
}

void BossJumpAttack::JumpMove()
{
	// 速度をリセット
	curMoveSpeed = DXSimp::Vector3::Zero;

	// 横の移動
	HorizonMove();

	// 縦の移動
	VertivalMove();

	// 速度をRbに渡す
	GetRB().SetVelocity(curMoveSpeed);
}

void BossJumpAttack::HorizonMove()
{
	// プレイヤーとの距離を見て速度を変更しながら、近づいてくる
	Transform& bossTrans = GetBossTransform();
	const DXSimp::Vector3& playerPos = GetPlayerTransform().GetPosition();
	float deltaTime = DeltaTime();
	float animRatio = GetAnimation()->GetCurrentPlayRatio();

	// 速度係数を取得する
	float curSpeedCofficient = horiSpeedCofficientCurve.GetValue(animRatio);

	// プレイヤーに合わせる座標を取得
	DXSimp::Vector3 playerMatchPos = bossTrans.GetPosition() + bossTrans.Forward() * fallPosOffset;

	// プレイヤーとの距離を取得する
	DXSimp::Vector3 toPlayerDistance = playerPos - playerMatchPos;
	toPlayerDistance.y = 0.0f;	// 横だけの距離

	// ボスをプレイヤーへ向けていく
	DXSimp::Vector3 toPlayerVec;
	toPlayerDistance.Normalize(toPlayerVec);

	// プレイヤーとの距離の長さ
	float toPlayerLength = toPlayerDistance.Length();

	// 距離から速度を求める(最大速度を制限)
	float speed = toPlayerLength * curSpeedCofficient * horiSpeedCofficient;
	if (speed > maxHoriSpeed)
		speed = maxHoriSpeed;

	// 移動速度を求める（XZ成分だけセット）
	DXSimp::Vector3 moveSpeedXZ = toPlayerVec * speed;
	curMoveSpeed.x = moveSpeedXZ.x; curMoveSpeed.z = moveSpeedXZ.z;
}

void BossJumpAttack::VertivalMove()
{
	if (DeltaTime() < Mathf::epsilon) return;

	Transform& bossTrans = GetBossTransform();
	float deltaTime = DeltaTime();
	float animRatio = GetAnimation()->GetCurrentPlayRatio();

	// 高さの移動
	// 速度を計算
	float curJumpHeight = jumpHeightDisCurve.GetValue(animRatio) * maxJumpHeight;
	float jumpSpeed = (curJumpHeight - prevJumpHeight) / deltaTime;

	// 速度代入
	curMoveSpeed.y = jumpSpeed;

	// 次フレームの為に更新
	prevJumpHeight = curJumpHeight;
}

void BossJumpAttack::ImGuiDebug()
{
	BossAttackState::ImGuiDebug();

	// Hori
	ImGui::DragFloat("FallOffset", &fallPosOffset, 0.01f);
	ImGui::DragFloat("MaxHoriSpd", &maxHoriSpeed, 1.0f, 0.0f, 1000.0f);
	ImGui::DragFloat("HoriSpeed", &horiSpeedCofficient, 1.0f, 0.0f, 1000.0f);
	horiSpeedCofficientCurve.ImGuiCall();

	// Vert
	ImGui::DragFloat("Height", &maxJumpHeight, 0.01f, 0.0f, 1000.0f);
	jumpHeightDisCurve.ImGuiCall();
}
