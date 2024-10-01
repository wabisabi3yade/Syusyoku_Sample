#include "pch.h"
#include "AnimNodePlayer_Base.h"
#include "SkeletalMesh.h"
#include "AnimationNode_Base.h"

AnimNodePlayer_Base::AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList)
	: pPlayAnimNode(&_playNode), pBoneList(&_boneList), curPlayRatio(0.0f), lastPlayRatio(-Mathf::smallValue)
{
	// ノードのみの再生速度を求める
	progressNodeSpeed = 1.0f / pPlayAnimNode->GetAnimationTime();
}

void AnimNodePlayer_Base::UpdateCall(float _controllerPlaySpeed)
{
	// 再生割合を進める
	ProgressPlayRatio(_controllerPlaySpeed);

	Update();
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
	curPlayRatio += _controllerPlaySpeed * progressNodeSpeed *  nodePlayTimeSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())	// ループできるなら
		curPlayRatio -= 1.0f;
}

bool AnimNodePlayer_Base::IsCanLoop() const
{
	// アニメーションの全体時間を超えていないなら
	if (curPlayRatio < 1.0f) return false;
	if (!pPlayAnimNode->GetIsLoop()) return false;

	return true;
}
