#include "pch.h"
#include "AnimationController.h"

// ノード種類
#include "SingleAnimationNode.h"
#include "BlendAnimationNode.h"

// アセット
#include "AssetGetter.h"
// ボーンリスト取得するため
#include "SkeletalMesh.h"
// 慣性補間
#include "InertInterpAnimation.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

AnimationController::AnimationController()
	: pCurrentNodeInfo(nullptr), pPrevAnimNode(nullptr), pDefaultNodeInfo(nullptr), pCurTransArrow(nullptr), playSpeed(1.0f), pBoneList(nullptr), isPlay(true), isTransitioning(false)
{
	pAnimParameters = std::make_unique<AnimationParameters>();
	pCrossFadeInterp = std::make_unique<CrossFadeAnimation>();
	pInertInterp = std::make_unique<InertInterpAnimation>();
}

AnimationController::AnimationController(const AnimationController& _other)
{
	Copy(_other);
}

AnimationController& AnimationController::operator=(const AnimationController& _other)
{
	Copy(_other);
	return *this;
}

void AnimationController::Begin(BoneList& _boneList)
{
	// デフォルトノードを現在のノードのセット
	pCurrentNodeInfo = pDefaultNodeInfo;

	// ボーンリストをセット
	pBoneList = &_boneList;
}

void AnimationController::Update(BoneList& _boneList)
{
	if (!IsCanPlay()) return;

	// アニメーション更新
	AnimatioUpdate();

	// キャッシュ更新
	CacheUpdate();

	// 遷移の条件を達成しているか確認
	TranstionCheck();
}

void AnimationController::NormalUpdate()
{
	pCurrentNodeInfo->pAnimNode->ProgressPlayRatio(playSpeed);
	if (pCurrentNodeInfo->pAnimNode->GetIsFinish())
		OnAnimationFinish();

	pCurrentNodeInfo->pAnimNode->UpdateCall(*pBoneList);
}

void AnimationController::CrossFadeUpdate()
{
	pCrossFadeInterp->Update(*pBoneList, playSpeed);

	if (pCrossFadeInterp->GetIsTransitionEnd())
	{
		OnTransitionEnd();
	}
}

void AnimationController::InertInterpUpdate()
{
	// 遷移の時間を進める
	float transElapsedTime = pInertInterp->ProgressTransitionTime(playSpeed);

	if (pInertInterp->GetIsTransitionEnd())	// ブレンド時間過ぎたら
	{
		// 遷移終了の処理
		OnTransitionEnd();
		return;
	}

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		Bone& bone = pBoneList->GetBone(b_i);

		BoneTransform transform;

		transform.position = pInertInterp->CalcBlendPos(b_i, transElapsedTime);
		transform.scale = pInertInterp->CalcBlendScale(b_i, transElapsedTime);
		transform.rotation = pInertInterp->CalcBlendRot(b_i, transElapsedTime);

		bone.SetAnimTransform(transform);
	}
}

void AnimationController::ChangeAnimation(const std::string& _animName)
{
	AnimNodeInfo* pNextNodeInfo = GetNodeInfo(_animName);
	if (!pNextNodeInfo)	// アニメーションがなかったら
	{
		HASHI_DEBUG_LOG(_animName + "は取得できませんでした");
		return;
	}

	// 再生開始する
	isPlay = true;

	// 更新する
	pPrevAnimNode = pCurrentNodeInfo->pAnimNode.get();
	pCurrentNodeInfo = GetNodeInfo(_animName);

	// 開始処理
	pCurrentNodeInfo->pAnimNode->Begin();
	pCurrentNodeInfo->pAnimNode->SetCurPlayRatio(0.0f);

	OnChangeAnimComplete();
}

void AnimationController::ChangeAnimation(const AnimTransitionArrow& _transitionArrow)
{
	// 再生開始する
	isPlay = true;

	// 更新する
	pPrevAnimNode = pPrevAnimNode = pCurrentNodeInfo->pAnimNode.get();
	pCurrentNodeInfo = GetNodeInfo(_transitionArrow.GetToNode());

	// 開始処理
	pCurrentNodeInfo->pAnimNode->Begin();

	// 遷移矢印をセットする
	pCurTransArrow = &_transitionArrow;
	float targetAnimRatio = pCurTransArrow->GetTargetRatio();
	float transitionTime = pCurTransArrow->GetTransitionTime();
	EaseKind ease = pCurTransArrow->GetEaseKind();

	// 補間開始
	switch (pCurTransArrow->GetInterpolateKind())
	{
	case AnimInterpolateKind::CrossFade:
		CrossFadeStart(targetAnimRatio, transitionTime, ease);
		break;

	case AnimInterpolateKind::Inertialization:
		InterpTransitionStart(targetAnimRatio, transitionTime);
		break;

	default:
		assert(!"アニメーション間の補間の種類指定が正常でないです");
		break;
	}
}


void AnimationController::SetBlendRatio(float _ratio)
{
	using enum AnimationNode_Base::NodeType;

	// 現在のノードがブレンドでないなら
	AnimationNode_Base& currentNode = *pCurrentNodeInfo->pAnimNode;
	if (currentNode.GetNodeType() != Blend) return;

	BlendAnimationNode& pBlend = static_cast<BlendAnimationNode&>(currentNode);

	pBlend.SetTargetBlendRatio(_ratio);
}

void AnimationController::CreateSingleNode(const std::string& _nodeName, const std::string& _animName)
{
	// 同じアニメーションがあったら
	if (IsHaveNode(_animName))	return;

	// アニメーションをセットし。ノードを配列に入れる
	std::unique_ptr<AnimNodeInfo> pCreateInfo = std::make_unique<AnimNodeInfo>();
	pCreateInfo->pAnimNode = std::make_unique<SingleAnimationNode>(_nodeName);
	pCreateInfo->pAnimNode->SetAnimationData(_animName);

	animNodeInfos.push_back(std::move(pCreateInfo));
}

void AnimationController::CreateBlendNode(const std::vector<std::string>& _animNames,
	const std::vector<float>& _ratios, const std::string& _nodeName)
{
	assert(_animNames.size() == _ratios.size() && "名前と割合の要素数が違います");

	std::unique_ptr<BlendAnimationNode> pCreateBlend = std::make_unique<BlendAnimationNode>(*pAnimParameters, _nodeName);

	u_int maxNum = static_cast<u_int>(_animNames.size());

	// ブレンドアニメーションにアニメーションを追加する
	for (u_int n_i = 0; n_i < maxNum; n_i++)
	{
		pCreateBlend->SetAnimationData(_animNames[n_i]);

		// ブレンド割合をセット
		pCreateBlend->SetAnimationRatio(_ratios[n_i], _animNames[n_i]);
	}

	std::unique_ptr<AnimNodeInfo> createInfo = std::make_unique<AnimNodeInfo>();
	createInfo->pAnimNode = std::move(pCreateBlend);
	animNodeInfos.push_back(std::move(createInfo));
}

AnimTransitionArrow* AnimationController::CreateTransitionArrow(const std::string& _fromNodeName, const std::string& _toNodeName)
{
	AnimNodeInfo* fromNodeInfo = GetNodeInfo(_fromNodeName);
	if (!fromNodeInfo)
	{
		HASHI_DEBUG_LOG(_fromNodeName + " 遷移元ノードが見つかりませんでした");
		return nullptr;
	}

	AnimNodeInfo* toNodeInfo = GetNodeInfo(_toNodeName);
	if (!toNodeInfo)
	{
		HASHI_DEBUG_LOG(_toNodeName + " 遷移先ノードが見つかりませんでした");
		return nullptr;
	}

	if (fromNodeInfo == toNodeInfo)	// 自分自身につなげていたら
	{
		HASHI_DEBUG_LOG("遷移元と先が同じノードです");
		return nullptr;
	}

	// 矢印作成して追加
	AnimationNode_Base* pFromNode = fromNodeInfo->pAnimNode.get();
	AnimationNode_Base* pToNode = toNodeInfo->pAnimNode.get();
	std::unique_ptr<AnimTransitionArrow> pTransitionArrow = std::make_unique<AnimTransitionArrow>(*pFromNode, *pToNode, *pAnimParameters);

	AnimTransitionArrow* pRetArrow = pTransitionArrow.get();
	fromNodeInfo->pTransArrows.push_back(std::move(pTransitionArrow));

	return pRetArrow;
}

void AnimationController::RemoveNode(const std::string& _nodeName)
{
	if (!GetNodeInfo(_nodeName)) return;	// なかったら処理しない

	AnimNodeInfo* pDelete = GetNodeInfo(_nodeName);
	if (!pDelete) return;

	auto itr = std::find_if(animNodeInfos.begin(), animNodeInfos.end(),
		[&](const std::unique_ptr<AnimNodeInfo>& _nodeInfo)
		{
			return pDelete == _nodeInfo.get();
		});

	if (itr != animNodeInfos.end())
		animNodeInfos.remove(*itr);
}

bool AnimationController::IsSetAnimation()
{
	return pCurrentNodeInfo != nullptr;
}

AnimationNode_Base* AnimationController::GetCurrentNode()
{
	return pCurrentNodeInfo->pAnimNode.get();
}

AnimationController::AnimNodeInfo* AnimationController::GetNodeInfo(const std::string& _name)
{
	auto itr = std::find_if(animNodeInfos.begin(), animNodeInfos.end(),
		[&](const std::unique_ptr<AnimNodeInfo>& _nodeInfo)
		{
			return _name == _nodeInfo->pAnimNode->GetNodeName();
		});

	if (itr == animNodeInfos.end())
	{
		HASHI_DEBUG_LOG(_name + "が見つかりませんでした");
		return nullptr;
	}

	return (*itr).get();
}

AnimationController::AnimNodeInfo* AnimationController::GetNodeInfo(const AnimationNode_Base& _node)
{
	auto itr = std::find_if(animNodeInfos.begin(), animNodeInfos.end(),
		[&](const std::unique_ptr<AnimNodeInfo>& _animInfo)
		{
			return &_node == _animInfo->pAnimNode.get();
		});

	if (itr == animNodeInfos.end()) return nullptr;

	return (*itr).get();
}

float AnimationController::GetPlayingRatio() const
{
	return pCurrentNodeInfo->pAnimNode->GetCurPlayRatio();
}

void AnimationController::GetNodeArray(std::list<const AnimationNode_Base*>& _animNodeArray) const
{
	for (auto& n : animNodeInfos)
	{
		_animNodeArray.push_back(n->pAnimNode.get());
	}
}

bool AnimationController::GetIsPlay() const
{
	return isPlay;
}

nlohmann::json AnimationController::Save()
{
	auto data = Asset_Base::Save();
	data["playSpeed"] = playSpeed;

	nlohmann::json nodeInfoData;
	for (auto& nodeInfo : animNodeInfos)
	{
		nodeInfoData.push_back(SaveNodeInfo(*nodeInfo));
	}
	data["nodeInfoData"] = nodeInfoData;

	if (pDefaultNodeInfo)
		data["defaultNode"] = pDefaultNodeInfo->pAnimNode->GetNodeName();

	data["animParameter"] = pAnimParameters->Save();	// パラメーター
	return data;
}

void AnimationController::Load(const nlohmann::json& _data)
{
	Asset_Base::Load(_data);
	LoadJsonFloat("playSpeed", playSpeed, _data);

	// パラメーター
	nlohmann::json paramsData;	
	if (LoadJsonDataArray("animParameter", paramsData, _data))
	{
		pAnimParameters->Load(paramsData);
	}

	// ノード情報をロードする
	nlohmann::json nodeInfoDatas;
	if (LoadJsonDataArray("nodeInfoData", nodeInfoDatas, _data))
	{
		for (auto& nd : nodeInfoDatas)
		{
			LoadNodeInfo(nd);
		}

		for (auto& nd : nodeInfoDatas)
		{
			LoadTransArrow(nd);
		}
	}

	std::string defaultNodeName = "";
	if (LoadJsonString("defaultNode", defaultNodeName, _data))
	{
		pDefaultNodeInfo = GetNodeInfo(defaultNodeName);
	}
}

bool AnimationController::IsCanPlay()
{
	if (!isPlay) return false;
	if (pCurrentNodeInfo == nullptr) return false;

	return true;
}

void AnimationController::AnimatioUpdate()
{
	if (isTransitioning)	// アニメーション遷移中なら
	{
		switch (pCurTransArrow->GetInterpolateKind())
		{
		case AnimInterpolateKind::CrossFade:
			CrossFadeUpdate();
			break;

		case AnimInterpolateKind::Inertialization:
			InertInterpUpdate();
			break;
		}
	}
	else
		NormalUpdate();
}

bool AnimationController::IsCanLoop()
{
	return false;
}

void AnimationController::CacheUpdate()
{
	// ボーンのキャッシュ取得のためパラメータを取得する
	std::vector<BoneTransform> cacheTransforms;

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(pBoneList->GetBone(b_i).GetAnimationTransform());
	}

	// 慣性補間用のキャッシュを更新する
	pInertInterp->UpdateBoneCache(cacheTransforms);
}

void AnimationController::TranstionCheck()
{
	AnimTransitionArrow* pTransArrow = nullptr;
	float curPlayRatio = pCurrentNodeInfo->pAnimNode->GetCurPlayRatio();
	float lastPlayRatio = pCurrentNodeInfo->pAnimNode->GetLastPlayRatio();

	for (auto& pArrow : pCurrentNodeInfo->pTransArrows)
	{
		if (!pArrow->CheckTransition(curPlayRatio, lastPlayRatio)) return;
		pTransArrow = pArrow.get();
	}

	// トリガー変数をリセットする
	pAnimParameters->ResetTrigger();

	if (!pTransArrow) return;	// 遷移しないなら終わる

	ChangeAnimation(*pTransArrow);
}

void AnimationController::OnChangeAnimComplete()
{
	HASHI_DEBUG_LOG(pCurrentNodeInfo->pAnimNode->GetNodeName() + "へ移行");
}

void AnimationController::OnTransitionStart()
{
	isTransitioning = true;
}

bool AnimationController::IsHaveNode(const std::string& _nodeName)
{
	auto itr = std::find_if(animNodeInfos.begin(), animNodeInfos.end(),
		[&](const std::unique_ptr<AnimNodeInfo>& _nodeInfo)
		{
			return _nodeName == _nodeInfo->pAnimNode->GetNodeName();
		});

	if (itr == animNodeInfos.end()) return false;
	return true;
}

void AnimationController::NotDuplicateNodeName(std::string& _nodename)
{
	u_int loopCnt = 1;
	std::string startName = _nodename;
	while (IsHaveNode(_nodename))	// ノードがある限りループ
	{
		_nodename = startName + std::to_string(loopCnt);
	}
}

void AnimationController::CrossFadeStart(float _targetAnimRatio, float _transitionTime, HashiTaku::EaseKind _easeKind)
{
	// 開始時の割合をセット
	AnimationNode_Base* pCurNode = pCurrentNodeInfo->pAnimNode.get();
	pCurNode->SetCurPlayRatio(_targetAnimRatio);

	pCrossFadeInterp->Begin(*pPrevAnimNode, *pCurNode, _transitionTime, _easeKind);

	OnTransitionStart();
}

void AnimationController::InterpTransitionStart(float _targetAnimRatio, float _transitionTime)
{
	u_int boneCnt = pBoneList->GetBoneCnt();
	std::vector<BoneTransform> requestPose(boneCnt);

	// 遷移終了時に再生されるのでターゲットの割合をセットしておく
	AnimationNode_Base* pCurNode = pCurrentNodeInfo->pAnimNode.get();
	pCurNode->SetCurPlayRatio(_targetAnimRatio);

	// 次のアニメーションの姿勢を取得する
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		pCurNode->GetAnimTransform(requestPose[b_i], b_i, _targetAnimRatio);
	}

	// 慣性補間の初期処理
	bool isSuccess = pInertInterp->Calculate(requestPose, _transitionTime);

	if (!isSuccess) return;	// 失敗したら

	OnTransitionStart();
}

void AnimationController::OnTransitionEnd()
{
	OnChangeAnimComplete();

	isTransitioning = false;	// 遷移終了
}

void AnimationController::SetBoneList(BoneList& _boneList)
{
	pBoneList = &_boneList;
}

void AnimationController::SetDefaultNode(const std::string& _nodeName)
{
	pDefaultNodeInfo = GetNodeInfo(_nodeName);
}

void AnimationController::SetBool(const std::string& _paramName, bool _isBool)
{
	pAnimParameters->SetBool(_paramName, _isBool);
}

void AnimationController::SetInt(const std::string& _paramName, int _intVal)
{
	pAnimParameters->SetInt(_paramName, _intVal);
}

void AnimationController::SetFloat(const std::string& _paramName, float _floatVal)
{
	pAnimParameters->SetFloat(_paramName, _floatVal);
}

void AnimationController::SetTrigger(const std::string& _paramName)
{
	pAnimParameters->SetTrigger(_paramName);
}

bool AnimationController::GetBool(const std::string& _paramName)
{
	return pAnimParameters->GetBool(_paramName);
}

int AnimationController::GetInt(const std::string& _paramName)
{
	return pAnimParameters->GetInt(_paramName);
}

float AnimationController::GetFloat(const std::string& _paramName)
{
	return pAnimParameters->GetFloat(_paramName);
}

bool AnimationController::GetTrigger(const std::string& _paramName)
{
	return pAnimParameters->GetTrigger(_paramName);
}

void AnimationController::OnAnimationFinish()
{
	isPlay = false;
}

nlohmann::json AnimationController::SaveNodeInfo(AnimNodeInfo& _nodeInfo)
{
	nlohmann::json nodeInfoData;
	nodeInfoData["nodeName"] = _nodeInfo.pAnimNode->GetNodeName();
	nodeInfoData["nodeType"] = _nodeInfo.pAnimNode->GetNodeType();
	nodeInfoData["animNode"] = _nodeInfo.pAnimNode->Save();

	auto& arrowDataList = nodeInfoData["arrowDataList"];
	for (auto& pArrow : _nodeInfo.pTransArrows)
	{
		nlohmann::json arrowData;
		arrowData["arrowParam"] = pArrow->Save();
		arrowData["toName"] = pArrow->GetToNode().GetNodeName();
		arrowDataList.push_back(arrowData);
	}

	return nodeInfoData;
}

void AnimationController::LoadNodeInfo(const nlohmann::json& _nodeInfoData)
{
	std::string nodeName;	// ノード名
	LoadJsonString("nodeName", nodeName, _nodeInfoData);
	AnimationNode_Base::NodeType createType;	// ノードタイプ
	LoadJsonEnum<AnimationNode_Base::NodeType>("nodeType", createType, _nodeInfoData);
	auto pCreateNodeInfo = CreateNodeInfoByType(createType, nodeName);	// ノード情報作成

	nlohmann::json nodeData;	// ノード種ごとのロード
	LoadJsonData("animNode", nodeData, _nodeInfoData);
	pCreateNodeInfo->pAnimNode->Load(nodeData);
}

void AnimationController::LoadTransArrow(const nlohmann::json& _nodeInfoData)
{
	// 矢印作成
	nlohmann::json arrowDataList;
	HashiTaku::LoadJsonDataArray("arrowDataList", arrowDataList, _nodeInfoData);
	for (auto& arrowData : arrowDataList)
	{
		std::string toNodeName;	// 遷移先
		HashiTaku::LoadJsonString("toName", toNodeName, arrowData);

		std::string fromName;	// 遷移元
		HashiTaku::LoadJsonString("nodeName", fromName, _nodeInfoData);

		// 矢印を作成
		if (AnimTransitionArrow* pTransArrow = CreateTransitionArrow(fromName, toNodeName))
		{
			nlohmann::json arrowParamData;	// 矢印のパラメータ
			if (HashiTaku::LoadJsonData("arrowParam", arrowParamData, arrowData))
				pTransArrow->Load(arrowParamData);
		}
	}
}


AnimationController::AnimNodeInfo* AnimationController::CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName)
{
	if (_nodeName.empty()) return nullptr;	// 名前入力してないなら終了

	std::string nodeName = _nodeName;
	NotDuplicateNodeName(nodeName);	// コントローラー内で重複しないようなノード名を作成する

	std::unique_ptr<AnimNodeInfo> pAnimNodeInfo = std::make_unique<AnimNodeInfo>();

	pAnimNodeInfo->pAnimNode = std::make_unique<SingleAnimationNode>(_nodeName);
	using enum  AnimationNode_Base::NodeType;
	switch (_nodeType)
	{
	case Single:
		pAnimNodeInfo->pAnimNode = std::make_unique<SingleAnimationNode>(_nodeName);
		break;

	case Blend:
		pAnimNodeInfo->pAnimNode = std::make_unique<BlendAnimationNode>(*pAnimParameters, _nodeName);
		break;

	default:
		assert(!"不正なノードタイプです");
		break;
	}

	AnimNodeInfo* pRetNode = pAnimNodeInfo.get();

	animNodeInfos.push_back(std::move(pAnimNodeInfo));
	return pRetNode;
}

void AnimationController::Copy(const AnimationController& _other)
{
	if (this == &_other) return;

	CopyNodes(_other);	// ノードをコピー
}

void AnimationController::CopyNodes(const AnimationController& _other)
{
	for (auto& nodeInfo : _other.animNodeInfos)
	{

	}
}

void AnimationController::ImGuiSetting()
{
	ImGui::Checkbox("Play", &isPlay);
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.1f);

	if (IsSetAnimation() && ImGuiMethod::TreeNode("Play Node"))
	{
		pCurrentNodeInfo->pAnimNode->ImGuiPlaying();
		ImGui::TreePop();
	}

	std::vector<std::string> nodeNames;	// 全ノード名を取得しておく
	for (auto& ni : animNodeInfos)
		nodeNames.push_back(ni->pAnimNode->GetNodeName());

	if (ImGuiMethod::TreeNode("All Node"))
	{
		ImGuiNode(nodeNames);	// 所持ノード
		ImGuiCreateNode();	// ノード追加
		ImGui::TreePop();
	}

	// パラメータ
	if (ImGuiMethod::TreeNode("Parameter"))
	{
		pAnimParameters->ImGuiCall();
		ImGui::TreePop();
	}
}


void AnimationController::ImGuiNode(const std::vector<std::string>& _nodeNames)
{
#ifdef EDIT
	auto nodeItr = animNodeInfos.begin();
	while (nodeItr != animNodeInfos.end())	// 全ノード
	{
		AnimationNode_Base& animNode = *(*nodeItr)->pAnimNode;
		bool isDelete = false;

		if (ImGuiMethod::TreeNode(animNode.GetNodeName()))
		{
			isDelete = ImGui::Button("Delete");	// 削除
			animNode.ImGuiCall();	// ノード
			ImGuiMethod::LineSpaceMid();

			// 矢印
			ImGui::Text("Arrow");
			ImGuiTransArrow(*(*nodeItr), _nodeNames);
			ImGui::TreePop();
		}

		// ノード情報削除
		if (isDelete)
			nodeItr = animNodeInfos.erase(nodeItr);
		else
			nodeItr++;
	}
#endif // EDIT
}

void AnimationController::ImGuiSetDefaultNode(const std::vector<std::string>& _nodeNames)
{
#ifdef EDIT
	// コンボボックスでデフォルトノード設定
	if (animNodeInfos.empty()) return;

	std::string curDefaultName;
	if (pDefaultNodeInfo)
		curDefaultName = pDefaultNodeInfo->pAnimNode->GetNodeName();

	if (ImGuiMethod::ComboBox("DefaultNode", curDefaultName, _nodeNames))
	{
		pDefaultNodeInfo = GetNodeInfo(curDefaultName);
	}
#endif // EDIT
}

void AnimationController::ImGuiTransition()
{
#ifdef EDIT
	AnimationNode_Base* pCurNode = pCurrentNodeInfo->pAnimNode.get();
	IAnimInterpolate* interpolate = pCrossFadeInterp.get();
	if (pCurTransArrow->GetInterpolateKind() == AnimInterpolateKind::Inertialization)
		interpolate = pInertInterp.get();

	std::string text = pPrevAnimNode->GetNodeName();
	text += " ---> ";
	text += pCurNode->GetNodeName();
	ImGui::Text(TO_UTF8(text));

	text = "経過：" + std::to_string(interpolate->GetElapsedTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "ブレンド時間" + std::to_string(interpolate->GetTransitionTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));
#endif // EDIT
}

void AnimationController::ImGuiCreateNode()
{
#ifdef EDIT
	ImGui::Text("CreateNode");

	// ノード名
	static char input[IM_INPUT_BUF] = "";
	ImGui::InputText("NodeName", input, IM_INPUT_BUF);

	// ノードタイプ
	static u_int selectId = 0;

	std::vector<std::string> typeNames =
	{
		"Single",
		"Blend"
	};
	ImGuiMethod::ComboBox("NodeType", selectId, typeNames);
	ImGui::SameLine();

	if (ImGui::Button("+"))
	{
		CreateNodeInfoByType(static_cast<AnimationNode_Base::NodeType>(selectId), input);
	}
#endif // EDIT
}

void AnimationController::ImGuiTransArrow(AnimNodeInfo& _nodeInfo, const std::vector<std::string>& _nodeNames)
{
#ifdef EDIT
	static u_int selectId = 0;

	// 所持している矢印パラメータ
	auto itr = _nodeInfo.pTransArrows.begin();
	while (itr != _nodeInfo.pTransArrows.end())
	{
		bool isDelete = false;
		std::string toNodeName = (*itr)->GetToNode().GetNodeName();

		std::string treeCaption = "To " + toNodeName;
		if (ImGuiMethod::TreeNode(treeCaption.c_str()))
		{
			(*itr)->ImGuiCall();
			isDelete = ImGui::Button("X");	// 削除ボタン
			ImGui::TreePop();
		}

		if (isDelete)
			itr = _nodeInfo.pTransArrows.erase(itr);
		else
			itr++;
	}

	// 新たに矢印を作成する
	ImGui::Text("CreateArrow");
	ImGuiMethod::ComboBox("ToNode", selectId, _nodeNames);
	ImGui::SameLine();
	if (ImGui::Button("+"))	// 作成
	{
		std::string fromName = _nodeInfo.pAnimNode->GetNodeName();
		CreateTransitionArrow(fromName, _nodeNames[selectId]);
	}
#endif // EDIT
}