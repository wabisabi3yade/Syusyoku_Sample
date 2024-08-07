#include "pch.h"
#include "BlendAnimationNode.h"

#include "AssetGetter.h"

#include "SkeletalMesh.h"

constexpr float DUPLICATE_PLUS(0.01f);	// ブレンド値が重複したときの＋値

using namespace DirectX::SimpleMath;

BlendAnimationNode::BlendAnimationNode(std::string _nodeName) : AnimationNode_Base(_nodeName, NodeType::Blend), curBlendRatio(0.0f), targetBlendRatio(0.0f), ratioMoveTime(0.0f)
{
	SetAnimationTime(1.0f);
}

void BlendAnimationNode::ImGuiPlaying()
{
	ImGui::Begin(TO_UTF8("ブレンドアニメーション"));

	AnimationNode_Base::ImGuiPlaying();

	std::string text = "現在のブレンド: " + std::to_string(curBlendRatio);
	ImGui::Text(TO_UTF8(text));

	ImGui::DragFloat(TO_UTF8("ターゲット"), &targetBlendRatio, 0.01f, 0.0f, 1.0f);

	for (auto& data : blendDatas)
	{
		std::string animName = data.pAnimation->GetAssetName();
		if (!ImGui::TreeNode(animName.c_str())) continue;
;
		ImGui::DragFloat(TO_UTF8("ブレンド"), &data.ratio, 0.01f, 0.0f, 1.0f);

		ImGui::TreePop();
	}

	ImGui::End();
}

void BlendAnimationNode::ImGuiSetting()
{
	if (!ImGui::TreeNode("Import")) return;

	static char animName[256] = "";
	ImGui::InputText("AnimName", animName, 256);

	if (ImGui::Button("Set"))
	{
		AnimationData* a = AssetGetter::GetAsset<AnimationData>(animName);
		if (a)
		{
			SetAnimationData(*a);
		}
	}

	ImGui::TreePop();
}

void BlendAnimationNode::Update(float _playingTime, BoneList& _boneList)
{
	// ブレンド値をターゲットに近づける
	MoveCurBlend();
	
	// アニメーションをブレンドする
	AnimationUpdate(_playingTime, _boneList);
}

void BlendAnimationNode::SetAnimationData(AnimationData& _animData)
{
	BlendData createData;
	createData.pAnimation = &_animData;
	createData.ratio = 0.0f;

	blendDatas.push_back(createData);
}

void BlendAnimationNode::SetTargetBlendRatio(float _ratio)
{
	SetRatio(targetBlendRatio, _ratio);
}

void BlendAnimationNode::SetAnimationRatio(float _ratio, const std::string& _animName)
{
	BlendData* findBlend = FindBlendData(_animName);
	// 見つからなかったら
	if (findBlend == nullptr) return;

	SetRatio(findBlend->ratio, _ratio);
}

void BlendAnimationNode::MoveCurBlend()
{
	// イージングなどで

	curBlendRatio = targetBlendRatio;
}

void BlendAnimationNode::AnimationUpdate(float _playingRatio, BoneList& _boneList)
{
	// ブレンドする2つのアニメーションを探す
	BlendPair blendPair = FindBlendPair();

	if (blendPair.nextData == nullptr)	// 考慮するアニメーションが一つだけ
		SingleUpdateAnimation(*blendPair.prevData, _playingRatio, _boneList);
	else
		BlendUpdateAnimation(blendPair, _playingRatio, _boneList);
}

BlendAnimationNode::BlendPair BlendAnimationNode::FindBlendPair()
{
	BlendPair blendPair{};

	// 以下の条件のとき、考慮するアニメーションは一つだけ
	auto minItr = blendDatas.begin();
	auto maxItr = std::prev(blendDatas.end());

	if (curBlendRatio <= minItr->ratio) // 最小値より小さい
	{
		blendPair.prevData = &(*minItr);
		return blendPair;
	}
	else if (curBlendRatio >= maxItr->ratio) // 最大値より大きい
	{
		blendPair.prevData = &(*maxItr);
		return blendPair;
	}


	// 現在の割合より大きい割合をデータの中から探す
	for (auto itr = blendDatas.begin(); itr != std::prev(blendDatas.end()); itr++)
	{
		auto nextItr = std::next(itr);
		if (curBlendRatio > nextItr->ratio) continue;

		// 見つけたら
		blendPair.prevData = &(*itr);
		blendPair.nextData = &(*nextItr);

		break;
	}
	return blendPair;
}

void BlendAnimationNode::SingleUpdateAnimation(BlendData& _animationData, float _playingRatio, BoneList& _boneList)
{
	const AnimationData& animData = *_animationData.pAnimation;

	//ボーン数ループ
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// 再生時間から各パラメータを取得
		// スケール
		transform.scale = animData.GetScale(b_i, _playingRatio);

		//クォータニオン
		transform.rotation = animData.GetQuaternion(b_i, _playingRatio);

		// 座標
		transform.position = animData.GetPosition(b_i, _playingRatio);

		bone.SetAnimTransform(transform);
	}
}

void BlendAnimationNode::BlendUpdateAnimation(BlendPair& _blendPair, float _playingRatio, BoneList& _boneList)
{
	float deltaRatio = _blendPair.nextData->ratio - _blendPair.prevData->ratio;

	if (deltaRatio == 0.0f)	// ブレンド値に差がない場合
	{
		deltaRatio += Mathf::smallValue; // 0徐算しないようにする
	}

	// アニメーションの重み割合
	float weightRatio = (curBlendRatio - _blendPair.prevData->ratio) / deltaRatio;

	const AnimationData* p_pAnimData = _blendPair.prevData->pAnimation;
	const AnimationData* n_pAnimData = _blendPair.nextData->pAnimation;

	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// 前のアニメーション
		p_Transform = p_pAnimData->GetTransform(b_i, _playingRatio);

		BoneTransform n_Transform;	// 後のアニメーション
		n_Transform = n_pAnimData->GetTransform(b_i, _playingRatio);

		BoneTransform blendTransform;
		// 座標
		blendTransform.position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// スケール
		blendTransform.scale = Vector3::Lerp(p_Transform.scale, n_Transform.scale, weightRatio);

		// 回転量
		blendTransform.rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);

		// トランスフォームをセット
		Bone& bone =  _boneList.GetBone(b_i);
		bone.SetAnimTransform(blendTransform);
	}
}

BlendAnimationNode::BlendData* BlendAnimationNode::FindBlendData(const std::string& _animName)
{
	// 名前から探す
	auto itr = std::find_if(blendDatas.begin(), blendDatas.end(), [&](BlendData& data)
		{
			return data.pAnimation->GetAssetName() == _animName;
		});

	if (itr == blendDatas.end())
	{
		HASHI_DEBUG_LOG("アニメーション名が見つかりませんでした");
		return nullptr;
	}

	return &(*itr);
}

void BlendAnimationNode::SetRatio(float& _ratio, float _setRatio)
{
#ifdef EDIT
	if (_ratio < 0.0f || _ratio > 1.0f)
		HASHI_DEBUG_LOG("0.0～1.0の範囲で代入してください");
#endif //  EDIT

	// 範囲内に制限
	_ratio = std::min(_ratio, 1.0f);
	_ratio = std::max(_ratio, 0.0f);
}

void BlendAnimationNode::SortBlendValue()
{
	blendDatas.sort(CompareBlendValue);
}

bool BlendAnimationNode::CompareBlendValue(const BlendData& _bd1, const BlendData& _bd2)
{
	return _bd1.ratio > _bd2.ratio;
}
