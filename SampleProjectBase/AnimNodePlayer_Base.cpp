#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), pObjectTransform(&_transform), curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue), isJustLoop(false)
{
	// ノードのみの再生速度を求める
	playNodeSpeedTimes = 1.0f / pPlayAnimNode->GetAnimationTime();
}

void AnimNodePlayer_Base::UpdateCall(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed)
{
	// 再生割合を進める
	ProgressPlayRatio(_controllerPlaySpeed);

	// アニメーションの更新処理
	Update(_outTransforms);

	// ルートモーションの座標移動速度を計算
	CalcRootMotionPosSpeed(_controllerPlaySpeed);

	// アニメーションのルートモーションを適用する
	ApplyRootMotionToTransform();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = _playRatio;

	// 1フレーム前の再生割合を現在の割合より前に置く
	lastPlayRatio = curPlayRatio - Mathf::smallValue;
}
void AnimNodePlayer_Base::SetPlaySpeedTimes(float _playSpeed)
{
	playNodeSpeedTimes = _playSpeed;
}

float AnimNodePlayer_Base::CalcPlaySpeed(float _controllerSpeed) const
{
	return _controllerSpeed * playNodeSpeedTimes * pPlayAnimNode->GetPlaySpeedTimes();
}

float AnimNodePlayer_Base::GetCurPlayRatio() const
{
	return curPlayRatio;
}

float AnimNodePlayer_Base::GetLastPlayRatio() const
{
	return lastPlayRatio;
}

float AnimNodePlayer_Base::GetNodePlaySpeed() const
{
	return playNodeSpeedTimes;
}

void AnimNodePlayer_Base::GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const
{
	_outPos = GetRootMotionPos(curPlayRatio) - p_RootMotionPos;
}

void AnimNodePlayer_Base::GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const
{
	_outPos = GetRootMotionPos(GetCurPlayRatio(), _isLoadScaling);
}

const std::string& AnimNodePlayer_Base::GetNodeName() const
{
	return pPlayAnimNode->GetNodeName();
}

const DirectX::SimpleMath::Vector3& AnimNodePlayer_Base::GetRootMotionSpeed() const
{
	return rootMotionPosSpeedPerSec;
}

void AnimNodePlayer_Base::ProgressPlayRatio(float _controllerPlaySpeed)
{
	isJustLoop = false;

	// 1フレーム前の再生割合を更新
	lastPlayRatio = curPlayRatio;

	/*
	コントローラ全体の再生速度 ×
	ノードの再生速度　×
	ノードの速度倍率　×
	DeletTime
	*/
	curPlayRatio += CalcPlaySpeed(_controllerPlaySpeed) * MainApplication::DeltaTime();

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

	isJustLoop = true;
	curPlayRatio = Mathf::Repeat(curPlayRatio, 1.0f);	// 再生割合を戻す
}

void AnimNodePlayer_Base::ApplyRootMotionToTransform()
{
	using namespace DirectX::SimpleMath;

	if (isJustLoop)	// ループしたなら
	{
		// 前回のルートモーションを初期化する
		p_RootMotionPos = GetRootMotionPos(0.0f);
		p_RootMotionRot = GetRootMotionRot(0.0f);
	}

	float curPlayRatio = GetCurPlayRatio();

	// 移動座標
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;

	// ループ時に前回の再生割合からアニメーション最後までのルートモーションの座標移動
	Vector3 loopDeadRMDistabce;
	if (curPlayRatio < lastPlayRatio)
	{
		Vector3 endRootMotionPos = GetRootMotionPos(1.0f);
		loopDeadRMDistabce = endRootMotionPos - GetRootMotionPos(lastPlayRatio);
	}

	// オブジェクトの向きに反映する
	Vector3 worldMovement;
	if (pPlayAnimNode->GetIsRootMotionXZ())
	{
		worldMovement += pObjectTransform->Right() * (posRootMovemrnt.x + loopDeadRMDistabce.x);
		worldMovement += pObjectTransform->Forward() * (posRootMovemrnt.z + loopDeadRMDistabce.z);
	}
	if (pPlayAnimNode->GetIsRootMotionY())
		worldMovement += pObjectTransform->Up() * (posRootMovemrnt.y + loopDeadRMDistabce.y);

	// オブジェクトの座標更新
	pObjectTransform->SetPosition(pObjectTransform->GetPosition() + worldMovement);
	p_RootMotionPos = curPos;
}

void AnimNodePlayer_Base::ApplyLoadTransform(DirectX::SimpleMath::Vector3& _rootMotionPos) const
{
	using namespace DirectX::SimpleMath;

	_rootMotionPos *= pBoneList->GetLoadScale();
	_rootMotionPos = Vector3::Transform(_rootMotionPos, Matrix::CreateFromQuaternion(pBoneList->GetLoadRotation()));
}

void AnimNodePlayer_Base::ImGuiSetting()
{
	ImGui::SliderFloat("play", &curPlayRatio, 0.0f, 1.0f);
	ImGui::DragFloat("speed", &playNodeSpeedTimes, 0.01f, 0.0f, 50.0f);
}
