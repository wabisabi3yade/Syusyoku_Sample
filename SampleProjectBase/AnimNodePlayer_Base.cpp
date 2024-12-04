#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"
#include "IAnimParametersSetter.h"
#include "AnimationNotifyFactory.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform) :
	pPlayAnimNode(&_playNode),
	pAssetBoneList(&_boneList),
	pObjectTransform(&_transform),
	curPlayRatio(0.0f),
	lastPlayRatio(-Mathf::smallValue),
	playerSpeedTimes(1.0f),
	allPlaySpeed(0.0f),
	deltaTime(0.0f),
	isJustLoop(false),
	isPlaying(true)
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

		// 初期処理
		pCopyNotify->OnInitCall();

		// 追加
		copyNotifys.push_back(std::move(pCopyNotify));
	}
}

void AnimNodePlayer_Base::UpdateCall(std::vector<BoneTransform>& _outTransforms, float _deltaTime, float _controllerSpeed)
{
	deltaTime = _deltaTime;
	lastPlayRatio = curPlayRatio;
	if (!isPlaying)
	{
		Update(_outTransforms);
		return;
	}

	// 再生割合を進める
	ProgressPlayRatio(_controllerSpeed);

	// アニメーションの更新処理
	Update(_outTransforms);

	// ルートモーションの座標移動速度を計算
	CalcRootMotionPosSpeed();

	// アニメーションのルートモーションを適用する
	ApplyRootMotion(CalcRootMotionToTransform());

	// 通知イベントを更新
	NotifyUpdate();

	
}

void AnimNodePlayer_Base::ApplyRootMotion(const DirectX::SimpleMath::Vector3& _rootMovement)
{
	pObjectTransform->SetPosition(pObjectTransform->GetPosition() + _rootMovement);
}

void AnimNodePlayer_Base::OnInterpolateUpdate(std::vector<BoneTransform>& _outTransforms, float _deltaTime, float _controllerSpeed)
{
	deltaTime = _deltaTime;
	lastPlayRatio = curPlayRatio;

	if (!isPlaying)
	{
		Update(_outTransforms);
		return;
	}

	// 再生割合を進める
	ProgressPlayRatio(_controllerSpeed);

	// アニメーションの更新処理
	Update(_outTransforms);

	// ルートモーションの座標移動速度を計算
	CalcRootMotionPosSpeed();

	// 通知イベントを更新
	NotifyUpdate();
}

void AnimNodePlayer_Base::SetCurPlayRatio(float _playRatio)
{
	curPlayRatio = std::clamp(_playRatio, 0.0f, 1.0f);

	// 1フレーム前の割合を求めておく
	lastPlayRatio = curPlayRatio - Mathf::smallValue;

	// ルートモーションも求めておく
	p_RootMotionPos = GetRootMotionPos(lastPlayRatio);
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

float AnimNodePlayer_Base::GetNodePlaySpeed() const
{
	return playerSpeedTimes;
}

void AnimNodePlayer_Base::GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const
{
	_outPos = GetRootMotionPos(curPlayRatio) - p_RootMotionPos;
}

void AnimNodePlayer_Base::GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const
{
	_outPos = GetRootMotionPos(curPlayRatio, _isLoadScaling);
}

const std::string& AnimNodePlayer_Base::GetNodeName() const
{
	return pPlayAnimNode->GetNodeName();
}

const DirectX::SimpleMath::Vector3& AnimNodePlayer_Base::GetRootMotionSpeed() const
{
	return rootMotionPosSpeedPerSec;
}

void AnimNodePlayer_Base::ProgressPlayRatio(float _controllerSpeed)
{
	isJustLoop = false;

	// カーブ上での速度
	float animCurveSpeed = pPlayAnimNode->GetCurveValue(curPlayRatio);

	/*
	コントローラー速度 ×
	プレイヤーの再生速度　×
	アニメーションカーブでの速度　×
	ノードの速度倍率
	*/
	allPlaySpeed = _controllerSpeed * playerSpeedTimes * 
		pPlayAnimNode->GetPlaySpeedTimes() * animCurveSpeed;

	// 0徐算防止
	float divideVal = _controllerSpeed;
	if (divideVal < Mathf::epsilon)
		divideVal = Mathf::epsilon;

	// deltaTimeにcontrollerSpeedが掛けてあるので
	float deltaRemoveConSpeed = deltaTime / divideVal;
	curPlayRatio += allPlaySpeed * deltaRemoveConSpeed / pPlayAnimNode->GetAnimationTime();

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
		isPlaying = false;
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

DirectX::SimpleMath::Vector3 AnimNodePlayer_Base::CalcRootMotionToTransform()
{
	using namespace DirectX::SimpleMath;

	if (isJustLoop)	// ループしたなら
	{
		// 前回のルートモーションを初期化する
		p_RootMotionPos = GetRootMotionPos(0.0f);
	}

	// 移動座標
	Vector3 curPos = GetRootMotionPos(curPlayRatio);
	Vector3 posRootMovemrnt = curPos - p_RootMotionPos;

	// ループ時に前回の再生割合からアニメーション1.0までのルートモーションの座標移動を足す
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

	p_RootMotionPos = curPos;

	return worldMovement;
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

	_rootMotionPos *= pAssetBoneList->GetLoadScale();
	_rootMotionPos = Vector3::Transform(_rootMotionPos, Matrix::CreateFromQuaternion(pAssetBoneList->GetLoadRotation()));
}

void AnimNodePlayer_Base::ImGuiDebug()
{
	ImGui::Checkbox("IsPlay", &isPlaying);
	ImGui::DragFloat("Speed", &playerSpeedTimes, 0.01f, 0.0f, 50.0f);
	float curveSpeed = pPlayAnimNode->GetCurveValue(curPlayRatio);
	ImGui::Text("curveSpeed:%f", curveSpeed);
	ImGui::SliderFloat("Ratio", &curPlayRatio, 0.0f, 1.0f);
	ImGui::Text("LastRatio:%f", lastPlayRatio);
}
