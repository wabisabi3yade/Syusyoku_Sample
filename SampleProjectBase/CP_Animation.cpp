#include "pch.h"
#include "CP_Animation.h"

#include "GameObject.h"

// メッシュ
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"

// アセット取得
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

CP_Animation::CP_Animation() : pSkeletalMesh(nullptr), pAnimController(nullptr), playingRatio(0.0f), playSpeed(1.0f), isPlaying(false)
{
	isPlaying = true;
}

void CP_Animation::Init()
{
	name = "CP_Animation";

	// レンダラーに設定しているスケルタルメッシュを取得
	CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
	SkeletalMesh* pSetMesh = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());

	if (pSetMesh != nullptr)
	{
		pSkeletalMesh = pSetMesh;
	}
}

void CP_Animation::LateUpdate()
{
	if (!IsCanPlay()) return;

	// 再生時間を進める
	ProgressPlayTime();

	//// アニメーション行列を更新
	UpdateAnimationMtx();

	// コンビネーション行列を更新
	UpdateBoneCombMtx();

	// ボーン行列をバッファとして送信
	UpdateBoneBuffer();
}

void CP_Animation::ImGuiSetting()
{
	static bool isWindowOpen = true;

	if (ImGui::Button(TO_UTF8("ウィンドウ")))
		isWindowOpen = true;

	if (!isWindowOpen) return;

	std::string name = "アニメーション" + pAnimController->GetAssetName();
	ImGui::Begin(TO_UTF8(name), &isWindowOpen);

	ImGui::Text(TO_UTF8(std::string("再生割合 " + std::to_string(playingRatio))));
	if(pAnimController->IsSetAnimation())
	ImGui::Text(TO_UTF8(std::string("再生時間 " + std::to_string(playingRatio * pAnimController->GetCurrentNode()->GetAnimationTime()))));
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.1f);
	ImGui::Checkbox("Play", &isPlaying);

	pAnimController->ImGuiSetting();

	ImGui::End();
}

void CP_Animation::AddAnimations(const std::string& _animName)
{
	pAnimController->AddAnimation(_animName);
}

void CP_Animation::RemoveAnimations(const std::string& _animName)
{
	pAnimController->RemoveAnimation(_animName);
}

void CP_Animation::SetSkeletalMesh(SkeletalMesh& _skeletalMesh)
{
	pSkeletalMesh = &_skeletalMesh;
}

void CP_Animation::SetAnimationController(AnimationController& _controller)
{
	pAnimController = &_controller;
}

void CP_Animation::ProgressPlayTime()
{
	// 各アニメーションの割合を進める速度
	float progressRatioSpeed = 1.0f / pAnimController->GetCurrentNode()->GetAnimationTime();

	// 時間を進める
	playingRatio += progressRatioSpeed * playSpeed * MainApplication::DeltaTime();

	if (IsCanLoop())	// ループできるなら
		playingRatio = 0.0f;
}

void CP_Animation::UpdateAnimationMtx()
{
	// アニメーションコントローラーで更新する
	pAnimController->Update(pSkeletalMesh->GetBoneList(), playingRatio);
}

bool CP_Animation::IsCanPlay()
{
	// 再生中　かつ　現在のアニメーションがあるなら　更新する
	if (!isPlaying) return false;
	if (pAnimController == nullptr) return false;
	if (!pAnimController->IsSetAnimation()) return false;

	assert(pSkeletalMesh != nullptr && "スケルタルメッシュ非設定");

	return true;
}

bool CP_Animation::IsCanLoop()
{
	const AnimationNode_Base* pCurrentAnimation = GetCurrentNode();

	// そもそもアニメーションが設定されていないなら
	if (!pAnimController->IsSetAnimation()) return false;

	// ループ再生しないなら
	if (!pCurrentAnimation->GetIsLoop()) return false;

	// アニメーションの全体時間を超えていないなら
	if (playingRatio < 1.0f) return false;

	return true;
}

void CP_Animation::UpdateBoneCombMtx()
{
	TreeNode& pRootNode = pSkeletalMesh->GetRootNode();
	// ノードを辿って全体のコンビネーション行列を更新していく
	UpdateNodeHierarchy(pRootNode, Matrix::Identity);
}

void CP_Animation::UpdateNodeHierarchy(TreeNode& _treeNode, const Matrix& _parentMtx)
{
	Matrix nodeMatrix = Matrix::Identity;

	// 対応したボーンがあるなら
	if (_treeNode.HasBone())
	{
		Bone& pBone = _treeNode.GetBone();

		// コンビネーション行列を求める
		pBone.CreateCombMtx(_parentMtx);
		nodeMatrix = pBone.GetAnimMtx();
	}

	// ワールド変換の行列を更新させる
	Matrix toWorldMtx = nodeMatrix * _parentMtx;

	// 再帰的にボーンを更新していく
	for (u_int c_i = 0; c_i < _treeNode.GetChildNum(); c_i++)
	{
		UpdateNodeHierarchy(*_treeNode.GetChild(c_i), toWorldMtx);
	}
}

void CP_Animation::UpdateBoneBuffer()
{
	u_int boneCnt = pSkeletalMesh->GetBoneNum();

	// モデルのスケール倍率をアニメーション行列に反映する
	Matrix scaleMtx = Matrix::CreateScale(pSkeletalMesh->GetScaleTimes());

	// ボーン数ループ
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		const Bone& bone = *pSkeletalMesh->GetBone(b_i);

		// ボーンのID番目に行列を入れる
		boneComb.matrix[bone.GetIndex()] = bone.GetCombMtx() * scaleMtx;
		boneComb.matrix[bone.GetIndex()] = boneComb.matrix[bone.GetIndex()].Transpose();
	}

	// マテリアルにボーン行列を渡す
	u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
	for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
	{
		Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
		/*pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));*/
		pMaterial->GetVertexShader().UpdateSubResource(1, &boneComb);
	}
}

const AnimationNode_Base* CP_Animation::GetCurrentNode()
{
	return pAnimController->GetCurrentNode();
}
