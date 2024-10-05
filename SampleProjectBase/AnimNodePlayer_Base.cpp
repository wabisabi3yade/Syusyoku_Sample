#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), pObjectTransform(&_transform), curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue)
{
	// ノードのみの再生速度を求める
	progressNodeSpeed = 1.0f / pPlayAnimNode->GetAnimationTime();
}

void AnimNodePlayer_Base::UpdateCall(float _controllerPlaySpeed)
{
	// 再生割合を進める
	ProgressPlayRatio(_controllerPlaySpeed);

	// アニメーションの更新処理
	Update();

	// アニメーションのルートモーションを適用する
	ApplyRootMotionToTransform();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = _playRatio;
}
float AnimNodePlayer_Base::GetCurPlayRatio() const
{
	return curPlayRatio;
}

void AnimNodePlayer_Base::ProgressPlayRatio(float _controllerPlaySpeed)
{
	// 1フレーム前の再生割合を更新
	lastPlayRatio = curPlayRatio;

	// 時間を進める
	float nodePlayTimeSpeed = pPlayAnimNode->GetPlaySpeedTimes();

	/*
	コントローラ全体の再生速度 ×
	ノードの再生速度　×
	ノードの速度倍率　×
	DeletTime
	*/
	curPlayRatio += _controllerPlaySpeed * progressNodeSpeed * nodePlayTimeSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())
		OnPlayLoop();
}

bool AnimNodePlayer_Base::IsCanLoop() const
{
	// アニメーションの全体時間を超えていないなら
	if (curPlayRatio < 1.0f) return false;
	if (!pPlayAnimNode->GetIsLoop()) return false;

	return true;
}

void AnimNodePlayer_Base::OnPlayLoop()
{
	using namespace DirectX::SimpleMath;

	curPlayRatio -= 1.0f;	// 再生割合を戻す

	// 前回のルートモーションを初期化する
	p_RootMotionPos = Vector3::Zero;
	p_RootMotionRot = Quaternion::Identity;
}

void AnimNodePlayer_Base::ApplyRootMotionToTransform()
{
	using namespace DirectX::SimpleMath;

	float curPlayRatio = GetCurPlayRatio();

	// 移動座標
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;
	if (pPlayAnimNode->GetIsRootMotionXZ())	// XZ方向に移動させないなら
	{
		posRootMovemrnt.x = 0.0f;
		posRootMovemrnt.z = 0.0f;
	}
	if (!pPlayAnimNode->GetIsRootMotionY())	// Y方向に移動させないなら
	{
		posRootMovemrnt.y = 0.0f;
	}

	// オブジェクトの向きに反映する
	Vector3 worldMovement;
	worldMovement = pObjectTransform->Right() * posRootMovemrnt.x;
	worldMovement += pObjectTransform->Up() * posRootMovemrnt.y;
	worldMovement += pObjectTransform->Forward() * posRootMovemrnt.z;

	// オブジェクトの座標更新
	pObjectTransform->SetPosition(pObjectTransform->GetPosition() + worldMovement);
	p_RootMotionPos = curPos;
}
