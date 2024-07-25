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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	if (!isPlaying) return;
	if (pCurrentAnimation == nullptr) return;

	assert(pSkeletalMesh != nullptr && "スケルタルメッシュ非設定");

	// 再生時間を進める
	ProgressPlayTime();

	// アニメーション行列を更新
	UpdateAnimationMtx();

	// コンビネーション行列を更新
	UpdateBoneCombMtx();
}

void CP_Animation::ImGuiSetting()
{
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

	if (playingTime_s > pCurrentAnimation->GetAnimationTime())
		playingTime_s = 0.0f;
}

void CP_Animation::UpdateBoneCombMtx()
{
	const aiNode* pRootNode = pCurrentAnimation->GetRootNode();

	// ノードを辿って全体のコンビネーション行列を更新していく
	UpdateNodeHierarchy(*pRootNode, Matrix::Identity);
}

void CP_Animation::UpdateNodeHierarchy(const aiNode& _aiNode, const Matrix& _parentMtx)
{
	std::string nodeName = _aiNode.mName.C_Str();

	Bone* pBone = GetBoneByName(nodeName);

	// コンビネーション行列を求める (ボーンオフセット * アニメーション * 逆オフセット * 親までの行列)
	pBone->CreateCombMtx(_parentMtx);

	// ワールド変換の行列を更新させる
	Matrix toWorldMtx = _parentMtx * pBone->GetAnimMtx();

	// 子ノードの行列を更新（再帰的）
	for (u_int c_i = 0; c_i < _aiNode.mNumChildren; c_i++)
	{
		UpdateNodeHierarchy(*_aiNode.mChildren[c_i], toWorldMtx);
	}
}

void CP_Animation::UpdateAnimationMtx()
{
	// ボーン数分ループしてコンビネーション行列を作成
	for (unsigned int c_i = 0; c_i < pCurrentAnimation->GetChannelCount(); c_i++)
	{
		std::string boneName = pCurrentAnimation->GetBoneName(c_i);

		Bone* pBone = GetBoneByName(boneName);

		// 再生時間から各パラメータを取得
		// スケール
		Vector3 animScale = pCurrentAnimation->GetScale(c_i, playingTime_s);

		//クォータニオン
		Quaternion animQuat = pCurrentAnimation->GetQuaternion(c_i, playingTime_s);

		// 座標
		Vector3 animPos = pCurrentAnimation->GetPosition(c_i, playingTime_s);

		HASHI_DEBUG_LOG("quat" +std::to_string(animQuat.x) + " " +
			std::to_string(animQuat.y) + " " +
			std::to_string(animQuat.z) + " " + 
			std::to_string(animQuat.w));

		HASHI_DEBUG_LOG("scale" + std::to_string(animScale.x) + " " +
			std::to_string(animScale.y) + " " +
			std::to_string(animScale.z));

		HASHI_DEBUG_LOG("pos" + std::to_string(animPos.x) + " " +
			std::to_string(animPos.y) + " " +
			std::to_string(animPos.z));

		// アニメーション行列を作成
		Matrix scaleMtx = Matrix::CreateScale(animScale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(animQuat);
		Matrix transformMtx = Matrix::CreateTranslation(animPos);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// ボーンにアニメーション行列をセット
		pBone->SetAnimationMtx(animationMtx);
	}
}

Bone* CP_Animation::GetBoneByName(const std::string& _boneName)
{
	assert(pSkeletalMesh != nullptr);

	Bone* pRetBone = nullptr;

	const std::vector<BonePerMesh>& bones = pSkeletalMesh->GetBones();

	// メッシュの数を取得
	u_int meshCnt = static_cast<u_int>(bones.size());

	// 一つずつ確認していく
	// メッシュ数ループ
	for (u_int m_i = 0; m_i < meshCnt; m_i++)
	{
		// メッシュ内のボーンの数を取得
		const BonePerMesh& bonePerMesh = bones[m_i];
		u_int boneCnt = static_cast<u_int>(bonePerMesh.size());

		// ボーン数ループ
		for (u_int b_i = 0; b_i < boneCnt; b_i++)
		{
			if (bonePerMesh[b_i]->GetBoneName() != _boneName)
				continue;

			// 見つけたら
			pRetBone = bonePerMesh[b_i].get();
			break;
		}

		if (pRetBone != nullptr)
			break;
	}

	assert(pRetBone != nullptr && "ボーンが見つかりませんでした");

	return nullptr;
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

void CP_Animation::ConnectBoneId(AnimationData& _connectAnim)
{
	if (pSkeletalMesh == nullptr)
	{
		HASHI_DEBUG_LOG("先にスケルタルメッシュを設定してください");
		return;
	}



}
