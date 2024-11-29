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
	: pDefaultNodeInfo(nullptr), playSpeed(1.0f), pAssetBoneList(nullptr)
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

	// 矢印作成して追加
	AnimationNode_Base* pFromNode = fromNodeInfo->pAnimNode.get();
	std::unique_ptr<AnimTransitionArrow> pTransitionArrow = CreateTransitionArrow(_toNodeName);
	if (!pTransitionArrow) return nullptr;

	AnimTransitionArrow* pRetArrow = pTransitionArrow.get();
	fromNodeInfo->transitionArrows.push_back(std::move(pTransitionArrow));

	return pRetArrow;
}

std::unique_ptr<AnimTransitionArrow> AnimationController::CreateTransitionArrow(const std::string& _toNodeName)
{
	const AnimNodeInfo* toNodeInfo = GetNodeInfo(_toNodeName);
	if (!toNodeInfo)
	{
		HASHI_DEBUG_LOG(_toNodeName + " 遷移先ノードが見つかりませんでした");
		return nullptr;
	}

	// 矢印作成して追加
	AnimationNode_Base* pToNode = toNodeInfo->pAnimNode.get();
	std::unique_ptr<AnimTransitionArrow> pTransitionArrow = std::make_unique<AnimTransitionArrow>(*pToNode, *pAnimParameters);

	return std::move(pTransitionArrow);
}

void AnimationController::RemoveNode(const std::string& _nodeName)
{
	const AnimNodeInfo* pDelete = GetNodeInfo(_nodeName);
	if (!pDelete) return;

	// デフォルトノードなら取り除く
	if (pDefaultNodeInfo == pDelete)
		pDefaultNodeInfo = nullptr;

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

float AnimationController::GetPlaySpeed() const
{
	return playSpeed;
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

const TransArrowList* AnimationController::GetGroupArrows(const std::string& _groupName) const
{
	auto itr = groupArrows.find(_groupName);
	if (itr == groupArrows.end()) return nullptr;	// 無かったら

	return &itr->second;
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

	data["groupArrows"] = SaveGroupArrows();

	data["animParameter"] = pAnimParameters->Save();	// パラメーター
	return data;
}

void AnimationController::Load(const nlohmann::json& _data)
{
	Asset_Base::Load(_data);
	LoadJsonFloat("playSpeed", playSpeed, _data);

	// パラメーター
	nlohmann::json loadData;
	if (LoadJsonDataArray("animParameter", loadData, _data))
	{
		pAnimParameters->Load(loadData);
	}

	// ノード情報をロードする
	if (LoadJsonDataArray("nodeInfoData", loadData, _data))
	{
		// 先にノードを作成
		for (auto& nd : loadData)
		{
			LoadNodeInfo(nd);
		}

		// 後でイベント・矢印を作成
		for (auto& nd : loadData)
		{
			LoadNotify(nd);
			LoadTransArrow(nd);
		}
	}

	// グループ矢印
	if (LoadJsonDataArray("groupArrows", loadData, _data))
	{
		LoadGroupArrow(loadData);
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
	if (pNotifyFactory->ImGuiCombo(pNotify))
	{
		pNotify->SetAnimationFrameCnt(_nodeInfo.pAnimNode->GetAllKeyFrame());

		_nodeInfo.notifyList.push_back(std::move(pNotify));
	}


#endif //  EDIT
}

void AnimationController::ImGuiGroupArrows(const std::vector<std::string>& _nodeNames)
{
	// グループごと
	auto groupItr = groupArrows.begin();
	while (groupItr != groupArrows.end())
	{
		bool isGroupDelete = false;
		bool isChangePri = false;
		const std::string& groupName = (*groupItr).first;
		if (ImGuiMethod::TreeNode(groupName))
		{
			isGroupDelete = ImGui::Button("X##Group");

			// 矢印
			TransArrowList& arrowList = (*groupItr).second;
			auto arrowItr = arrowList.begin();
			while (arrowItr != arrowList.end())
			{
				bool isaArrowDelete = false;
				AnimTransitionArrow& arrow = *(*arrowItr);
				if (ImGuiMethod::TreeNode(arrow.GetToNode().GetNodeName()))
				{
					isaArrowDelete = ImGui::Button("X");
					arrow.ImGuiCall();

					// 優先順位
					int pri = arrow.GetPriority();
					if (ImGui::DragInt("Priority", &pri))
					{
						isChangePri = true;
						arrow.SetPriority(pri);
					}

					ImGui::TreePop();
				}

				if (isaArrowDelete)
					arrowItr = arrowList.erase(arrowItr);
				else
					arrowItr++;
			}

			if (isChangePri)	// 優先度を変えたなら
				arrowList.sort(SortArrowPriority);	// ソート

			// 追加
			static std::string toNodeNmae;
			ImGuiMethod::ComboBox("ToNode", toNodeNmae, _nodeNames);
			ImGui::SameLine();
			if (ImGui::Button("+##CreateArrow"))
			{
				AddArrowToGroup(groupName, CreateTransitionArrow(toNodeNmae));
			}

			ImGui::TreePop();
		}

		if (isGroupDelete)
			groupItr = groupArrows.erase(groupItr);
		else
			++groupItr;
	}

	// 新しくグループを作成
	static char inputText[IM_INPUT_BUF] = "\0";
	ImGui::InputText("groupName", inputText, IM_INPUT_BUF);
	if (ImGui::Button("+"))
	{
		CreateGroupArrows(inputText);
	}
}

nlohmann::json AnimationController::SaveNodeInfo(AnimNodeInfo& _nodeInfo)
{
	nlohmann::json nodeInfoData;
	nodeInfoData["nodeName"] = _nodeInfo.pAnimNode->GetNodeName();
	nodeInfoData["nodeType"] = _nodeInfo.pAnimNode->GetNodeType();
	nodeInfoData["animNode"] = _nodeInfo.pAnimNode->Save();
	nodeInfoData["groupArrowName"] = _nodeInfo.groupArrowsName;

	// 通知イベント
	auto& notifyDataList = nodeInfoData["notifyList"];
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

nlohmann::json AnimationController::SaveGroupArrows()
{
	nlohmann::json groupDatas;
	for (auto& group : groupArrows)
	{
		nlohmann::json groupData;
		groupData["name"] = group.first;	// 名前

		auto& arrowDatas = groupData["arrows"];
		for (auto& arrow : group.second)
		{
			nlohmann::json arrowData;
			arrowData["toNode"] = arrow->GetToNode().GetNodeName();
			arrowData["param"] = arrow->Save();
			arrowDatas.push_back(arrowData);	// 矢印のデータ
		}

		groupDatas.push_back(groupData);
	}

	return groupDatas;
}

void AnimationController::LoadNodeInfo(const nlohmann::json& _nodeInfoData)
{
	std::string nodeName;	// ノード名
	LoadJsonString("nodeName", nodeName, _nodeInfoData);

	AnimationNode_Base::NodeType createType;	// ノードタイプ
	LoadJsonEnum<AnimationNode_Base::NodeType>("nodeType", createType, _nodeInfoData);
	auto pCreateNodeInfo = CreateNodeInfoByType(createType, nodeName);	// ノード情報作成

	// グループ遷移矢印名
	LoadJsonString("groupArrowName", pCreateNodeInfo->groupArrowsName, _nodeInfoData);

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
		std::unique_ptr<AnimationNotify_Base> pLoadNotify = pNotifyFactory->Create(loadType);
		pLoadNotify->SetAnimationFrameCnt(pNodeInfo->pAnimNode->GetAllKeyFrame());
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

TransArrowList* AnimationController::CreateGroupArrows(const std::string& _groupName)
{
	if (_groupName.empty()) return nullptr;
	if (groupArrows.contains(_groupName)) return nullptr;		// 重複しているなら

	// 追加
	TransArrowList createArrows;
	groupArrows[_groupName] = std::move(createArrows);

	return &groupArrows[_groupName];
}

void AnimationController::AddArrowToGroup(const std::string& _groupName, std::unique_ptr<AnimTransitionArrow> _addArrow)
{
	if (!_addArrow) return;	// そもそもないなら
	if (!groupArrows.contains(_groupName)) return;	// あるか確認

	// 追加
	groupArrows[_groupName].push_back(std::move(_addArrow));
}

void AnimationController::GetGroupNamelist(std::vector<const std::string*>& _outList)
{
	// 名前を追加していく
	for (auto& arrowList : groupArrows)
	{
		_outList.push_back(&arrowList.first);
	}
}

bool AnimationController::SortArrowPriority(const std::unique_ptr<AnimTransitionArrow>& _a1, const std::unique_ptr<AnimTransitionArrow>& _a2)
{
	return _a1->GetPriority() > _a2->GetPriority();
}

void AnimationController::LoadGroupArrow(const nlohmann::json& _groupArrowData)
{
	for (auto& groupData : _groupArrowData)
	{
		std::string groupName;	// グループ名
		if (!HashiTaku::LoadJsonString("name", groupName, groupData)) continue;

		TransArrowList* pTransArrow = CreateGroupArrows(groupName);
		if (!pTransArrow) continue;	// 作成失敗したなら

		nlohmann::json arrowDatas;
		if (!HashiTaku::LoadJsonDataArray("arrows", arrowDatas, groupData)) continue;

		for (auto& arrowData : arrowDatas)	// 矢印
		{
			std::string toNodeName;	// 遷移先ノード名
			if (!HashiTaku::LoadJsonString("toNode", toNodeName, arrowData)) continue;

			AnimNodeInfo* pToNodeInfo = GetNodeInfo(toNodeName);
			if (!pToNodeInfo) continue;

			// 作成
			std::unique_ptr<AnimTransitionArrow> pCreateArrow =
				std::make_unique<AnimTransitionArrow>(*pToNodeInfo->pAnimNode, *pAnimParameters);

			// パラメータ
			nlohmann::json paramData;
			if (!LoadJsonData("param", paramData, arrowData)) continue;
			pCreateArrow->Load(paramData);

			pTransArrow->push_back(std::move(pCreateArrow));	// 追加
		}
	}
}

void AnimationController::ImGuiDebug()
{
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.01f, 0.0f, 1000.0f);

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

	if (ImGuiMethod::TreeNode("Group Arrow"))
	{
		ImGuiGroupArrows(nodeNames);
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
	// 遷移条件のグループ名を取得
	std::vector<const std::string*> groupNames;
	GetGroupNamelist(groupNames);

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

			// グループ名をセットする
			ImGuiMethod::ComboBox("groupArrow", (*nodeItr)->groupArrowsName, groupNames);
			ImGui::SameLine();
			if (ImGui::Button("Clear")) (*nodeItr)->groupArrowsName = "";

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
	static std::string selectToNode = "";
	ImGuiMethod::ComboBox("ToNode", selectToNode, _nodeNames);
	ImGui::SameLine();
	if (ImGui::Button("+##TransArrow"))	// 作成
	{
		std::string fromName = _nodeInfo.pAnimNode->GetNodeName();
		CreateTransitionArrow(fromName, selectToNode);
	}
#endif // EDIT
}