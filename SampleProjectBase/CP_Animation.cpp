#include "pch.h"
#include "CP_Animation.h"

#include "GameObject.h"

// アニメーション
#include "AnimationData.h"

// メッシュ
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"

// アセット取得
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

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
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.1f);

	ImGui::Checkbox("Play", &isPlaying);

	// コンボボックス
	std::string currentName = "";
	if (pCurrentAnimation != nullptr)
		currentName = pCurrentAnimation->GetName();

	std::vector<std::string> animNames;
	for (auto& anim : pHaveAnimations)
	{
		animNames.push_back(anim->GetName());
	}
	bool isChange = ImGuiMethod::ComboBox(TO_UTF8("アニメーション"), currentName, animNames);
	if (isChange)
	{
		PlayAnimation(currentName);
	}

}

void CP_Animation::PlayAnimation(const std::string& _animName)
{
	// 名前からアニメーションを再生する
	AnimationData* pPlayAnim = FindAnimaton(_animName);
	if (pPlayAnim == nullptr) return;

	pCurrentAnimation = pPlayAnim;
	isPlaying = true;
}

void CP_Animation::AddAnimations(AnimationData& _addAnim)
{
	pHaveAnimations.push_back(&_addAnim);
}

void CP_Animation::AddAnimations(const std::string& _animName)
{
	AnimationData* pAnimData = AssetGetter::GetAsset<AnimationData>(_animName);

	if (pAnimData == nullptr)
		return;

	pHaveAnimations.push_back(pAnimData);
}

void CP_Animation::RemoveAnimations(const std::string& _animName)
{
	// 削除したか確認するために
	// 要素数を取得
	u_int num = static_cast<u_int>(pHaveAnimations.size());

	// 名前が同じアニメーションを探して取り除く
	pHaveAnimations.remove_if([&](AnimationData* pAnim)
		{
			return pAnim->GetName() == _animName;
		});

	if (static_cast<u_int>(pHaveAnimations.size()) != num)
		HASHI_DEBUG_LOG("正常に削除されました");
}

void CP_Animation::SetSkeletalMesh(SkeletalMesh& _skeletalMesh)
{
	pSkeletalMesh = &_skeletalMesh;
}

void CP_Animation::ProgressPlayTime()
{
	// 時間を進める
	playingTime_s += playSpeed * MainApplication::DeltaTime();

	// アニメーションの全体時間を超えたら
	if (playingTime_s > pCurrentAnimation->GetAnimationTime())
		playingTime_s = 0.0f;
}

void CP_Animation::UpdateAnimationMtx()
{
	static u_int flame = 0;

	// チャンネル数分ループしてアニメーション行列を作成
	for (unsigned int c_i = 0; c_i < pCurrentAnimation->GetChannelCount(); c_i++)
	{
		std::string boneName = pCurrentAnimation->GetBoneName(c_i);

		Bone* pBone = pSkeletalMesh->GetBoneByName(boneName);

		// 再生時間から各パラメータを取得
		// スケール
		Vector3 animScale = /*pCurrentAnimation->GetScale(c_i, playingTime_s)*/Vector3::One;

		//クォータニオン
		Quaternion animQuat = pCurrentAnimation->GetQuaternion(c_i, playingTime_s);

		// 座標
		Vector3 animPos = pCurrentAnimation->GetPosition(c_i, playingTime_s);

		// アニメーション行列を作成
		Matrix scaleMtx = Matrix::CreateScale(Vector3::One);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(animQuat);
		Matrix transformMtx = Matrix::CreateTranslation(animPos);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// ボーンにアニメーション行列をセット
		pBone->SetAnimationMtx(animationMtx);
	}

	flame++;
}

bool CP_Animation::IsCanPlay()
{
	// 再生中　かつ　現在のアニメーションがあるなら　更新する
	if (!isPlaying) return false;
	if (pCurrentAnimation == nullptr) false;

	assert(pSkeletalMesh != nullptr && "スケルタルメッシュ非設定");

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
		nodeMatrix = pBone.GetAnimMtx() ;
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

	// ボーン数ループ
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		const Bone& bone = pSkeletalMesh->GetBone(b_i);

		// ボーンのID番目に行列を入れる
		boneComb.matrix[bone.GetIndex()] = bone.GetCombMtx();
		boneComb.matrix[bone.GetIndex()] = boneComb.matrix[bone.GetIndex()].Transpose();
	}

	// マテリアルにボーン行列を渡す
	u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
	for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
	{
		Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
		//pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));
		pMaterial->GetVertexShader().UpdateSubResource(1, &boneComb);
	}
}

AnimationData* CP_Animation::FindAnimaton(const std::string& _animName)
{
	// 同じ名前のアニメーションを探索
	auto itr = std::find_if(pHaveAnimations.begin(), pHaveAnimations.end(),
		[&](AnimationData* pAnim)
		{
			return pAnim->GetName() == _animName;
		});

	if (itr == pHaveAnimations.end())
	{
		HASHI_DEBUG_LOG(_animName + "が見つかりませんでした");
		return nullptr;
	}

	return *itr;
}
