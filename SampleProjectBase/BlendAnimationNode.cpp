#include "pch.h"
#include "BlendAnimationNode.h"
#include "AnimationParameters.h"

#include "AssetGetter.h"
#include "SkeletalMesh.h"

constexpr float DUPLICATE_PLUS(0.01f);	// ブレンド値が重複したときの＋値

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

BlendAnimationNode::BlendAnimationNode(const AnimationParameters& _animParams, std::string _nodeName)
	: AnimationNode_Base(_nodeName, NodeType::Blend), pAnimParameters(&_animParams)
{
	// 初期化
	for (u_int a_i = 0; a_i < axisCnt; a_i++)
		targetBlendRatios[a_i] = nullptr;

	SetAnimationTime(1.0f);
}
void BlendAnimationNode::ImGuiPlaying()
{
	AnimationNode_Base::ImGuiPlaying();

	std::string text = "現在のブレンド: " + std::to_string(curBlendRatio);
	ImGui::Text(TO_UTF8(text));

	// ターゲット
	float target = targetBlendRatio;
	if (ImGui::SliderFloat("target", &target, 0.0f, 1.0f))
		SetTargetBlendRatio(target);
}

void BlendAnimationNode::Begin()
{
	AnimationNode_Base::Begin();

	targetBlendRatio = 0.0f;
	curBlendRatio = 0.0f;
	changeBlendRatio = 0.0f;
}

void BlendAnimationNode::Update(BoneList& _boneList)
{
	if (!IsCanUpdate()) return;

	// ブレンド値をターゲットに近づける
	MoveCurBlend();

	// アニメーションをブレンドする
	AnimationUpdate(_boneList);
}

void BlendAnimationNode::SetAnimationData(const std::string& _animName)
{
	BlendData createData;

	AnimationData* pData = AssetGetter::GetAsset<AnimationData>(_animName);
	if (!pData) return;

	createData.pAnimation = pData;
	createData.ratioX = 0.0f;
	blendDatas.push_back(createData);
}

void BlendAnimationNode::SetTargetBlendRatio(float _ratio)
{
	// 同じ割合をセットするなら処理しない
	if (abs(_ratio - targetBlendRatio) <= Mathf::epsilon)
		return;

	SetRatio(targetBlendRatio, _ratio);

	// 移動時間をリセットする
	curRatioSmoothTime = 0.0f;
	// 変更時の求める
	changeBlendRatio = curBlendRatio;
}

void BlendAnimationNode::SetAnimationRatio(float _ratio, const std::string& _animName)
{
	BlendData* findBlend = FindBlendData(_animName);
	// 見つからなかったら
	if (findBlend == nullptr) return;

	SetRatio(findBlend->ratioX, _ratio);
}

nlohmann::json BlendAnimationNode::Save()
{
	auto data = AnimationNode_Base::Save();

	for (auto& b : blendDatas)	// 各ブレンドデータをセーブ
		data["blendData"].push_back(SaveBlendData(b));
	data["smoothTime"] = ratioSmoothTime;
	data["moveEase"] = ratioMoveEase;
	return data;
}

void BlendAnimationNode::Load(const nlohmann::json& _data)
{
	AnimationNode_Base::Load(_data);

	nlohmann::json blendSaveDatas;
	LoadJsonDataArray("blendData", blendSaveDatas, _data);
	for (auto& d : blendSaveDatas)	// ブレンドデータをロードする
		LoadBlendData(d);
}

void BlendAnimationNode::MoveCurBlend()
{
	// 割合移動時間
	if (ratioSmoothTime < Mathf::epsilon)
	{
		curBlendRatio = targetBlendRatio;
		return;
	}

	curRatioSmoothTime += MainApplication::DeltaTime();
	curRatioSmoothTime = std::min(curRatioSmoothTime, ratioSmoothTime);

	float easeValue = HashiTaku::Easing::EaseValue(curRatioSmoothTime / ratioSmoothTime, ratioMoveEase);

	float subRatio = targetBlendRatio - changeBlendRatio;

	// イージングで割合を移動する
	curBlendRatio = changeBlendRatio + subRatio * easeValue;
}

void BlendAnimationNode::AnimationUpdate(BoneList& _boneList)
{
	// ブレンドする2つのアニメーションを探す
	BlendPair blendPair = FindBlendPair();

	if (blendPair.nextData == nullptr)	// 考慮するアニメーションが一つだけ
		SingleUpdateAnimation(*blendPair.prevData, _boneList);
	else
		BlendUpdateAnimation(blendPair, _boneList);
}

bool BlendAnimationNode::IsCanUpdate()
{
	if (static_cast<u_int>(blendDatas.size()) == 0) return false;

	return true;
}

BlendAnimationNode::BlendPair BlendAnimationNode::FindBlendPair()
{
	BlendPair blendPair{};

	// 以下の条件のとき、考慮するアニメーションは一つだけ
	auto minItr = blendDatas.begin();
	auto maxItr = std::prev(blendDatas.end());

	if (curBlendRatio <= minItr->ratioX) // 最小値より小さい
	{
		blendPair.prevData = &(*minItr);
		return blendPair;
	}
	else if (curBlendRatio >= maxItr->ratioX) // 最大値より大きい
	{
		blendPair.prevData = &(*maxItr);
		return blendPair;
	}


	// 現在の割合より大きい割合をデータの中から探す
	for (auto itr = blendDatas.begin(); itr != std::prev(blendDatas.end()); itr++)
	{
		auto nextItr = std::next(itr);
		if (curBlendRatio > nextItr->ratioX) continue;

		// 見つけたら
		blendPair.prevData = &(*itr);
		blendPair.nextData = &(*nextItr);

		break;
	}
	return blendPair;
}

void BlendAnimationNode::SingleUpdateAnimation(BlendData& _animationData, BoneList& _boneList)
{
	const AnimationData& animData = *_animationData.pAnimation;

	float playingRatio = GetCurPlayRatio();

	//ボーン数ループ
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// 再生時間から各パラメータを取得
		// スケール
		transform.scale = animData.GetScaleByRatio(b_i, playingRatio);

		//クォータニオン
		transform.rotation = animData.GetQuaternionByRatio(b_i, playingRatio);

		// 座標
		transform.position = animData.GetPositionByRatio(b_i, playingRatio);

		bone.SetAnimTransform(transform);
	}
}

void BlendAnimationNode::BlendUpdateAnimation(BlendPair& _blendPair, BoneList& _boneList)
{
	float deltaRatio = _blendPair.nextData->ratioX - _blendPair.prevData->ratioX;
	float playingRatio = GetCurPlayRatio();

	if (abs(deltaRatio) < Mathf::epsilon)	// ブレンド値に差がない場合
	{
		deltaRatio = Mathf::smallValue; // 0徐算しないようにする
	}

	// アニメーションの重み割合
	float weightRatio = (curBlendRatio - _blendPair.prevData->ratioX) / deltaRatio;

	const AnimationData* p_pAnimData = _blendPair.prevData->pAnimation;
	const AnimationData* n_pAnimData = _blendPair.nextData->pAnimation;

	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		BoneTransform p_Transform;	// 前のアニメーション
		p_Transform = p_pAnimData->GetTransformByRatio(b_i, playingRatio);

		BoneTransform n_Transform;	// 後のアニメーション
		n_Transform = n_pAnimData->GetTransformByRatio(b_i, playingRatio);

		BoneTransform blendTransform;
		// 座標
		blendTransform.position = Vector3::Lerp(p_Transform.position, n_Transform.position, weightRatio);

		// スケール
		blendTransform.scale = Vector3::Lerp(p_Transform.scale, n_Transform.scale, weightRatio);

		// 回転量
		blendTransform.rotation = Quaternion::Slerp(p_Transform.rotation, n_Transform.rotation, weightRatio);

		// トランスフォームをセット
		Bone& bone = _boneList.GetBone(b_i);
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
	if (_setRatio < 0.0f || _setRatio > 1.0f)
		HASHI_DEBUG_LOG("0.0～1.0の範囲で代入してください");
#endif //  EDIT

	// 範囲内に制限
	_ratio = std::min(_setRatio, 1.0f);
	_ratio = std::max(_ratio, 0.0f);
}

void BlendAnimationNode::SortBlendValue()
{
	blendDatas.sort(CompareBlendValue);
}

bool BlendAnimationNode::CompareBlendValue(const BlendData& _bd1, const BlendData& _bd2)
{
	return _bd1.ratioX > _bd2.ratioX;
}

void BlendAnimationNode::GetCurAnimTransform(BoneTransform& _outTransform, u_int _boneId) const
{
	GetAnimTransform(_outTransform, _boneId, GetCurPlayRatio());
}

void BlendAnimationNode::GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const
{
	// 現段階は最初の割合だけ
	const AnimationData* pAnimationData = blendDatas.begin()->pAnimation;
	_outTransform = pAnimationData->GetTransformByRatio(_boneId, _requestRatio);

}

void BlendAnimationNode::ImGuiSetting()
{
	AnimationNode_Base::ImGuiSetting();

	ImGui::DragFloat("RatioSmoothTime", &ratioSmoothTime, 0.01f, 0.0f, 10.0f);
	HashiTaku::Easing::ImGuiSelect(ratioMoveEase);
	for (auto& data : blendDatas)
	{
		std::string animName = data.pAnimation->GetAssetName();
		if (!ImGuiMethod::TreeNode(animName.c_str())) continue;
		ImGui::SliderFloat("Blend", &data.ratioX, 0.0f, 1.0f);
		ImGui::TreePop();
	}

	static std::string str;
	AssetGetter::ImGuiGetCombobox<AnimationData>("AddAnimation", str);
	if (ImGui::Button("Add Blend"))
		SetAnimationData(str);
}

void BlendAnimationNode::ImGuiChangeTargetParam()
{
	// float型変数のみのパラメータ名を取得
	std::vector<const std::string*> pAnimParams;
	pAnimParameters->GetFloatNameList(pAnimParams);


}

nlohmann::json BlendAnimationNode::SaveBlendData(BlendData& _blendData)
{
	nlohmann::json saveData;
	saveData["animName"] = _blendData.pAnimation->GetAssetName();
	saveData["ratio"] = _blendData.ratioX;
	return saveData;
}

void BlendAnimationNode::LoadBlendData(const nlohmann::json& _blendData)
{
	BlendData loadBlend;

	loadBlend.pAnimation = LoadJsonAsset<AnimationData>("animName", _blendData);
	if (!loadBlend.pAnimation) return;

	LoadJsonFloat("ratio", loadBlend.ratioX, _blendData);
	blendDatas.push_back(std::move(loadBlend));
}

