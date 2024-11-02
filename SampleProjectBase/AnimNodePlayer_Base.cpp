#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"
#include "IAnimParametersSetter.h"
#include "AnimationNotifyFactory.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), pObjectTransform(&_transform),
	curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue), animationRatio(0.0f), playerSpeedTimes(1.0f), allPlaySpeed(0.0f), isJustLoop(false), isPlay(true)
{
}

void AnimNodePlayer_Base::CopyNotifys(const std::list<std::unique_ptr<AnimationNotify_Base>>& _notifyList, AnimationParameters& _animationParameters)
{
	// コピーする
	for (auto& origin : _notifyList)
	{
		// クローン作成
		auto pCopyNotify = origin->Clone();

		// アニメーションパラメータをセットするなら
		if (IAnimParametersSetter* pAnimParamSetter = 
			dynamic_cast<IAnimParametersSetter*>(pCopyNotify.get()))
		{
			pAnimParamSetter->SetAnimationParameters(_animationParameters);
		}
	
		// 追加
		copyNotifys.push_back(std::move(pCopyNotify));
	}
}

void AnimNodePlayer_Base::UpdateCall(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed)
{
	if (!isPlay)
	{
		Update(_outTransforms);
		return;
	}

	// 再生割合を進める
	ProgressPlayRatio(_controllerPlaySpeed);

	// アニメーションの更新処理
	Update(_outTransforms);

	// ルートモーションの座標移動速度を計算
	CalcRootMotionPosSpeed(_controllerPlaySpeed);

	// アニメーションのルートモーションを適用する
	ApplyRootMotionToTransform();

	// 通知イベントを更新
	NotifyUpdate();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = _playRatio;

	// 1フレーム前の再生割合を現在の割合より前に置く
	lastPlayRatio = curPlayRatio - Mathf::smallValue;

	animationRatio = pPlayAnimNode->GetCurveValue(curPlayRatio);
}

void AnimNodePlayer_Base::SetPlaySpeedTimes(float _playSpeed)
{
	playerSpeedTimes = _playSpeed;
}

float AnimNodePlayer_Base::GetCurPlayRatio() const
{
	return curPlayRatio;
}

float AnimNodePlayer_Base::GetLastPlayRatio() const
{
	return lastPlayRatio;
}

float AnimNodePlayer_Base::GetAnimationRatio() const
{
	return animationRatio;
}

float AnimNodePlayer_Base::GetNodePlaySpeed() const
{
	return playerSpeedTimes;
}

void AnimNodePlayer_Base::GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const
{
	_outPos = GetRootMotionPos(animationRatio) - p_RootMotionPos;
}

void AnimNodePlayer_Base::GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const
{
	_outPos = GetRootMotionPos(GetAnimationRatio(), _isLoadScaling);
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
	プレイヤーの再生速度　×
	ノードの速度倍率　/
	再生時間を考慮したアニメーション速度(割合で進めているので)
	*/
	allPlaySpeed = _controllerPlaySpeed *
		playerSpeedTimes *
		pPlayAnimNode->GetPlaySpeedTimes() /
		pPlayAnimNode->GetAnimationTime();


	curPlayRatio += allPlaySpeed * MainApplication::DeltaTime();
	// アニメーション割合を計算
	animationRatio = pPlayAnimNode->GetCurveValue(curPlayRatio);

	if (IsCanLoop())
		OnPlayLoop();
}

void AnimNodePlayer_Base::OnTerminal()
{
	for (auto& pNotify : copyNotifys)
	{
		pNotify->OnTerminalCall();
	}

	copyNotifys.clear();
}

bool AnimNodePlayer_Base::IsCanLoop()
{
	// アニメーションの全体時間を超えていないなら
	if (curPlayRatio < 1.0f) return false;
	if (!pPlayAnimNode->GetIsLoop())
	{
		isPlay = false;
		return false;
	}

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
		//p_RootMotionRot = GetRootMotionRot(0.0f);
	}

	float curPlayRatio = GetAnimationRatio();

	// 移動座標
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;

	// ループ時に前回の再生割合からアニメーション最後までのルートモーションの座標移動
	Vector3 loopDeadRMDistabce;
	if (isJustLoop)
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

void AnimNodePlayer_Base::NotifyUpdate()
{
	// 全て
	for (auto& pNotify : copyNotifys)
	{
		pNotify->Update(lastPlayRatio, curPlayRatio, isJustLoop);
	}
}

void AnimNodePlayer_Base::ApplyLoadTransform(DirectX::SimpleMath::Vector3& _rootMotionPos) const
{
	using namespace DirectX::SimpleMath;

	_rootMotionPos *= pBoneList->GetLoadScale();
	_rootMotionPos = Vector3::Transform(_rootMotionPos, Matrix::CreateFromQuaternion(pBoneList->GetLoadRotation()));
}

void AnimNodePlayer_Base::ImGuiSetting()
{
	ImGui::SliderFloat("Play", &curPlayRatio, 0.0f, 1.0f);
	ImGui::Text("AnimRatio:%lf", animationRatio);
	ImGui::DragFloat("Speed", &playerSpeedTimes, 0.01f, 0.0f, 50.0f);
}
