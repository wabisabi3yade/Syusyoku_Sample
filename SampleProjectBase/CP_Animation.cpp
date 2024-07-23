#include "pch.h"
#include "CP_Animation.h"

#include "GameObject.h"

// アニメーション
#include "AnimationData.h"

// メッシュ
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"

#include "AssetGetter.h"

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
	// アニメーションを更新する
	UpdateAnimation();
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
	bool isChange = ImGuiMethod::ComboBox("アニメーション", currentName, animNames);
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


void CP_Animation::UpdateAnimation()
{
	if (!isPlaying) return;
	if (pCurrentAnimation == nullptr) return;

	assert(pSkeletalMesh != nullptr && "スケルタルメッシュ非設定");

	// ボーン数分ループしてボーン行列を作成
	for (unsigned int c_i = 0; c_i < pCurrentAnimation->GetChannelCount(); c_i++)
	{
		std::string boneName = pCurrentAnimation->GetBoneName(c_i);
		HASHI_DEBUG_LOG(boneName);
	}
}

const Bone* CP_Animation::GetBoneByName(const std::string& _boneName)
{
	assert(pSkeletalMesh != nullptr);

	const Bone* pRetBone = nullptr;

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

	return pRetBone;
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
