#include "pch.h"
#include "AnimationController.h"
// アセット
#include "AssetGetter.h"
// ボーンリスト取得するため
#include "SkeletalMesh.h"
// 慣性補間
#include "InertInterpAnimation.h"

using namespace DirectX::SimpleMath;

AnimationController::AnimationController() : pCurrentAnimNode(nullptr), pNextAnimNode(nullptr), isTransitioning(false)
{
	inertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList, float _playingTime)
{
	// ボーンのキャッシュ取得のためパラメータを取得する
	std::vector<BoneTransform> boneTransforms(_boneList.GetBoneCnt());

	if (isTransitioning)	// アニメーション遷移中なら
		TransitionUpdate(_boneList, _playingTime, boneTransforms);
	else
		NormalUpdate(_boneList, _playingTime, boneTransforms);

	// 慣性補間用のキャッシュを更新する
	inertInterp->UpdateBoneCache(boneTransforms);
}

void AnimationController::NormalUpdate(BoneList& _boneList, float& _playingTime, std::vector<BoneTransform>& _cacheTransform)
{
	const AnimationData& currentAnimData = pCurrentAnimNode->GetAnimationData();

	//ボーン数ループ
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// 再生時間から各パラメータを取得
		// スケール
		transform.scale = currentAnimData.GetScale(b_i, _playingTime);

		//クォータニオン
		transform.rotation = currentAnimData.GetQuaternion(b_i, _playingTime);

		// 座標
		transform.position = currentAnimData.GetPosition(b_i, _playingTime);

		// アニメーション行列を作成
		Matrix scaleMtx = Matrix::CreateScale(transform.scale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(transform.rotation);
		Matrix transformMtx = Matrix::CreateTranslation(transform.position);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// ボーンにアニメーション行列をセット
		bone.SetAnimationMtx(animationMtx);

		// キャッシュ配列に追加する
		_cacheTransform[b_i] = std::move(transform);
	}
}

void AnimationController::TransitionUpdate(BoneList& _boneList, float& _playingTime, std::vector<BoneTransform>& _cacheTransform)
{
	blendElapsedTime += MainApplication::DeltaTime();

	if (blendElapsedTime > blendTime)	// ブレンド時間過ぎたら
	{
		// 遷移終了の処理
		TransitionEnd(_playingTime);
	}

	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		transform.position = inertInterp->CalcBlendPos(b_i, blendElapsedTime);
		transform.scale = inertInterp->CalcBlendScale(b_i, blendElapsedTime);
		transform.rotation = inertInterp->CalcBlendRot(b_i, blendElapsedTime);

		// アニメーション行列を作成
		Matrix scaleMtx = Matrix::CreateScale(transform.scale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(transform.rotation);
		Matrix transformMtx = Matrix::CreateTranslation(transform.position);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// ボーンにアニメーション行列をセット
		bone.SetAnimationMtx(animationMtx);

		// キャッシュ配列に追加する
		_cacheTransform[b_i] = std::move(transform);
	}
}

void AnimationController::ImGuiSetting()
{
	//if (!ImGui::TreeNode(TO_UTF8("アニメーションコントローラー"))) return;

	if (IsSetAnimation())
	{
		pCurrentAnimNode->ImGuiSetting();
	}

	ImGui::Text(TO_UTF8("アニメーション遷移"));
	for (auto& a : pAnimationNodes)	// ボタンでアニメーション変える
	{
		std::string animName = a.second->GetAnimationData().GetAssetName();
		if(ImGui::Button(animName.c_str()))
		{
			ChangeAnimation(animName);	// アニメーション変更
		}
	}

	ImGui::Text(TO_UTF8("遷移情報"));

	if (isTransitioning)
		ImGuiTransition();

	/*ImGui::TreePop();*/
}

void AnimationController::ChangeAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName))
	{
		HASHI_DEBUG_LOG(_animName + "は取得できませんでした");
		return;
	}
	

	pNextAnimNode = pAnimationNodes[_animName].get();

	// 慣性補間の初期処理
	bool isSuccess = inertInterp->Calculate(*pNextAnimNode, blendTime);

	if (isSuccess)	// 成功していたら
	{
		isTransitioning = true;
		blendElapsedTime = 0.0f;
	}


}

void AnimationController::AddAnimation(const std::string& _animName)
{
	// 同じアニメーションがあったら
	if (IsHaveAnim(_animName))	return;

	AnimationData* pGetAnim = AssetGetter::GetAsset<AnimationData>(_animName);

	// アニメーションがなかったら
	if (pGetAnim == nullptr) return;

	// ノードを作成する
	std::unique_ptr<AnimStateNode> pAnimNode = std::make_unique<AnimStateNode>(_animName);
	pAnimNode->SetAnimationData(*pGetAnim);

	pAnimationNodes[_animName] = std::move(pAnimNode);

	if (!IsSetAnimation())	// 再生中のアニメーションがないならセットする
		pCurrentAnimNode = pAnimationNodes[_animName].get();
}

void AnimationController::RemoveAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName)) return;

	pAnimationNodes.erase(_animName);
}

bool AnimationController::IsSetAnimation()
{
	return pCurrentAnimNode != nullptr;
}

AnimStateNode& AnimationController::GetCurrentNode()
{
	return *pCurrentAnimNode;
}

void AnimationController::ImGuiTransition()
{
	std::string text = pCurrentAnimNode->GetNodeName();
	text += " ---> ";
	text += pNextAnimNode->GetNodeName();
	ImGui::Text(TO_UTF8(text));

	text = "経過：" + std::to_string(blendElapsedTime) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "ブレンド時間" + std::to_string(blendTime) + "(s)";
	ImGui::Text(TO_UTF8(text));
}

bool AnimationController::IsHaveAnim(const std::string& _animName)
{
	// 配列内に同じ名前があるか
	for (auto& a : pAnimationNodes)
	{
		if (_animName == a.first)
			return true;
	}

	return false;
}

void AnimationController::TransitionEnd(float& _playTime)
{
	_playTime = 0.0f;
	
	pCurrentAnimNode = pNextAnimNode;
	pNextAnimNode = nullptr;
	isTransitioning = false;
}
