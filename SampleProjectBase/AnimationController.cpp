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
	: pDefaultNodeInfo(nullptr), playSpeed(1.0f), pBoneList(nullptr)
{
	pAnimParameters = std::make_unique<AnimationParameters>();
	pNotifyFactory = std::make_unique<AnimationNotifyFactory>(*pAnimParameters);
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

AnimTransitionArrow* AnimationController::CreateTransitionArrow(const std::string& _fromNodeName, const std::string& _toNodeName)
{
	AnimNodeInfo* fromNodeInfo = GetNodeInfo(_fromNodeName);
	if (!fromNodeInfo)
	{
		HASHI_DEBUG_LOG(_fromNodeName + " 遷移元ノードが見つかりませんでした");
		return nullptr;
	}

	const AnimNodeInfo* toNodeInfo = GetNodeInfo(_toNodeName);
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
	fromNodeInfo->transitionArrows.push_back(std::move(pTransitionArrow));

	return pRetArrow;
}

void AnimationController::RemoveNode(const std::string& _nodeName)
{
	const AnimNodeInfo* pDelete = GetNodeInfo(_nodeName);
	if (!pDelete) return;

	// 遷移先が削除ノードならその矢印も消す
	for (auto& pNodeInfo : animNodeInfos)
	{
		std::erase_if(pNodeInfo->transitionArrows, [&](const std::unique_ptr<AnimTransitionArrow>& arrow)
			{
				return  pDelete->pAnimNode.get() == &arrow->GetToNode();
			});
	}

	// ノードを削除する
	auto itr = std::erase_if(animNodeInfos, [&](const std::unique_ptr<AnimNodeInfo>& _nodeInfo)
		{
			return pDelete == _nodeInfo.get();
		});
}

AnimNodeInfo* AnimationController::GetDefaultNode() const
{
	return pDefaultNodeInfo;
}

AnimNodeInfo* AnimationController::GetNodeInfo(const std::string& _name) const
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

const AnimNodeInfo* AnimationController::GetNodeInfo(const AnimationNode_Base& _node) const
{
	auto itr = std::find_if(animNodeInfos.begin(), animNodeInfos.end(),
		[&](const std::unique_ptr<AnimNodeInfo>& _animInfo)
		{
			return &_node == _animInfo->pAnimNode.get();
		});

	if (itr == animNodeInfos.end()) return nullptr;

	return (*itr).get();
}

void AnimationController::GetNodeArray(std::list<const AnimationNode_Base*>& _animNodeArray) const
{
	for (auto& n : animNodeInfos)
	{
		_animNodeArray.push_back(n->pAnimNode.get());
	}
}

const AnimationParameters& AnimationController::GetAnimationParameters() const
{
	return *pAnimParameters;
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
		// 先にノードを作成
		for (auto& nd : nodeInfoDatas)
		{
			LoadNodeInfo(nd);
		}

		// 後でイベント・矢印を作成
		for (auto& nd : nodeInfoDatas)
		{
			LoadNotify(nd);
			LoadTransArrow(nd);
		}
	}

	// デフォルトノードをロード
	std::string defaultNodeName = "";
	if (LoadJsonString("defaultNode", defaultNodeName, _data))
	{
		pDefaultNodeInfo = GetNodeInfo(defaultNodeName);
	}
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

void AnimationController::ImGuiAnimNotify(AnimNodeInfo& _nodeInfo)
{
#ifdef  EDIT
	// 編集
	for (auto& notify : _nodeInfo.notifyList)
	{
		if (ImGuiMethod::TreeNode(notify->GetNotifyName()))
		{
			if (ImGui::Button("X"))	// 削除
			{
				_nodeInfo.notifyList.remove(notify);
				ImGui::TreePop();
				return;
			}

			// 編集
			notify->ImGuiCall();
			ImGui::TreePop();
		}
	}

	// 追加
	// 生成クラス側でイベント作成
	std::unique_ptr<AnimationNotify_Base> pNotify;
	if(pNotifyFactory->ImGuiCombo(pNotify))
		_nodeInfo.notifyList.push_back(std::move(pNotify));

#endif //  EDIT
}

nlohmann::json AnimationController::SaveNodeInfo(AnimNodeInfo& _nodeInfo)
{
	nlohmann::json nodeInfoData;
	nodeInfoData["nodeName"] = _nodeInfo.pAnimNode->GetNodeName();
	nodeInfoData["nodeType"] = _nodeInfo.pAnimNode->GetNodeType();
	nodeInfoData["animNode"] = _nodeInfo.pAnimNode->Save();

	// 通知イベント
	auto& notifyDataList =  nodeInfoData["notifyList"];
	for (auto& pNotify : _nodeInfo.notifyList)
	{
		nlohmann::json notifyData;
		notifyData["notifyParam"] = pNotify->Save();
		notifyData["notifyType"] = pNotify->GetNotifyType();
		notifyDataList.push_back(notifyData);
	}

	// 遷移矢印
	auto& arrowDataList = nodeInfoData["arrowDataList"];
	for (auto& pArrow : _nodeInfo.transitionArrows)
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

void AnimationController::LoadNotify(const nlohmann::json& _nodeInfoData)
{
	// 通知イベントリストを取得
	nlohmann::json notifyDataList;
	if (!LoadJsonDataArray("notifyList", notifyDataList, _nodeInfoData))
		return;

	// ノード情報取得
	std::string nodeName;
	if (!LoadJsonString("nodeName", nodeName, _nodeInfoData))
		return;

	AnimNodeInfo* pNodeInfo = GetNodeInfo(nodeName);
	if (!pNodeInfo) return;

	for (auto& notifyData : notifyDataList)
	{
		// 通知イベントの種類
		AnimationNotify_Base::NotifyType loadType;
		if (!LoadJsonEnum<AnimationNotify_Base::NotifyType>("notifyType", loadType, notifyData))
			continue;

		// 通知イベントのパラメータ
		nlohmann::json notifyParamData;
		if (!LoadJsonData("notifyParam", notifyParamData, notifyData))
			continue;

		// 作成
		std::unique_ptr<AnimationNotify_Base> pLoadNotify =  pNotifyFactory->Create(loadType);
		pLoadNotify->Load(notifyParamData);

		// 追加
		pNodeInfo->notifyList.push_back(std::move(pLoadNotify));
	}
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


AnimNodeInfo* AnimationController::CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName)
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

bool AnimationController::SortArrowPriority(const std::unique_ptr<AnimTransitionArrow>& _a1, const std::unique_ptr<AnimTransitionArrow>& _a2)
{
	return _a1->GetPriority() > _a2->GetPriority();
}

void AnimationController::ImGuiSetting()
{
	std::vector<std::string> nodeNames;	// 全ノード名を取得しておく
	for (auto& ni : animNodeInfos)
		nodeNames.push_back(ni->pAnimNode->GetNodeName());

	// デフォルトノード設定
	ImGuiSetDefaultNode(nodeNames);

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

		std::string nodeName = animNode.GetNodeName();
		if (ImGuiMethod::TreeNode(nodeName))
		{
			isDelete = ImGui::Button("Delete");	// 削除
			animNode.ImGuiCall();	// ノード
			ImGuiMethod::LineSpaceMid();

			// 通知イベント
			ImGui::Text("Notify");
			ImGuiAnimNotify(*(*nodeItr));
			ImGuiMethod::LineSpaceMid();

			// 矢印
			ImGui::Text("Arrow");
			ImGuiTransArrow(*(*nodeItr), _nodeNames);
			ImGui::TreePop();
		}

		// ノード情報削除
		if (isDelete)
		{
			nodeItr = std::next(nodeItr);
			RemoveNode(nodeName);
		}
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
	bool isChangePriority = false;
	// 所持している矢印パラメータ
	auto itr = _nodeInfo.transitionArrows.begin();
	while (itr != _nodeInfo.transitionArrows.end())
	{
		bool isDelete = false;
		std::string toNodeName = (*itr)->GetToNode().GetNodeName();

		std::string treeCaption = "To " + toNodeName;
		if (ImGuiMethod::TreeNode(treeCaption.c_str()))
		{
			isDelete = ImGui::Button("X");	// 削除ボタン
			(*itr)->ImGuiCall();

			// 優先順位
			int pri = (*itr)->GetPriority();
			if (ImGui::DragInt("Priority", &pri))
			{
				isChangePriority = true;
				(*itr)->SetPriority(pri);
			}

			ImGui::TreePop();
		}

		if (isDelete)
			itr = _nodeInfo.transitionArrows.erase(itr);
		else
			itr++;
	}

	// 優先順位変えたなら
	if (isChangePriority)
		_nodeInfo.transitionArrows.sort(SortArrowPriority);

	// 新たに矢印を作成する
	ImGui::Text("CreateArrow");
	ImGuiMethod::ComboBox("ToNode", selectId, _nodeNames);
	ImGui::SameLine();
	if (ImGui::Button("+##TransArrow"))	// 作成
	{
		std::string fromName = _nodeInfo.pAnimNode->GetNodeName();
		CreateTransitionArrow(fromName, _nodeNames[selectId]);
	}
#endif // EDIT
}