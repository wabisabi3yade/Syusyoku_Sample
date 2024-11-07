#include "pch.h"
#include "AnimationController.h"

// �m�[�h���
#include "SingleAnimationNode.h"
#include "BlendAnimationNode.h"

// �A�Z�b�g
#include "AssetGetter.h"
// �{�[�����X�g�擾���邽��
#include "SkeletalMesh.h"
// �������
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
	// �����A�j���[�V��������������
	if (IsHaveNode(_animName))	return;

	// �A�j���[�V�������Z�b�g���B�m�[�h��z��ɓ����
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
		HASHI_DEBUG_LOG(_fromNodeName + " �J�ڌ��m�[�h��������܂���ł���");
		return nullptr;
	}

	const AnimNodeInfo* toNodeInfo = GetNodeInfo(_toNodeName);
	if (!toNodeInfo)
	{
		HASHI_DEBUG_LOG(_toNodeName + " �J�ڐ�m�[�h��������܂���ł���");
		return nullptr;
	}

	if (fromNodeInfo == toNodeInfo)	// �������g�ɂȂ��Ă�����
	{
		HASHI_DEBUG_LOG("�J�ڌ��Ɛ悪�����m�[�h�ł�");
		return nullptr;
	}

	// ���쐬���Ēǉ�
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

	// �J�ڐ悪�폜�m�[�h�Ȃ炻�̖�������
	for (auto& pNodeInfo : animNodeInfos)
	{
		std::erase_if(pNodeInfo->transitionArrows, [&](const std::unique_ptr<AnimTransitionArrow>& arrow)
			{
				return  pDelete->pAnimNode.get() == &arrow->GetToNode();
			});
	}

	// �m�[�h���폜����
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
		HASHI_DEBUG_LOG(_name + "��������܂���ł���");
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

	data["animParameter"] = pAnimParameters->Save();	// �p�����[�^�[
	return data;
}

void AnimationController::Load(const nlohmann::json& _data)
{
	Asset_Base::Load(_data);
	LoadJsonFloat("playSpeed", playSpeed, _data);

	// �p�����[�^�[
	nlohmann::json paramsData;
	if (LoadJsonDataArray("animParameter", paramsData, _data))
	{
		pAnimParameters->Load(paramsData);
	}

	// �m�[�h�������[�h����
	nlohmann::json nodeInfoDatas;
	if (LoadJsonDataArray("nodeInfoData", nodeInfoDatas, _data))
	{
		// ��Ƀm�[�h���쐬
		for (auto& nd : nodeInfoDatas)
		{
			LoadNodeInfo(nd);
		}

		// ��ŃC�x���g�E�����쐬
		for (auto& nd : nodeInfoDatas)
		{
			LoadNotify(nd);
			LoadTransArrow(nd);
		}
	}

	// �f�t�H���g�m�[�h�����[�h
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
	while (IsHaveNode(_nodename))	// �m�[�h��������胋�[�v
	{
		_nodename = startName + std::to_string(loopCnt);
	}
}

void AnimationController::ImGuiAnimNotify(AnimNodeInfo& _nodeInfo)
{
#ifdef  EDIT
	// �ҏW
	for (auto& notify : _nodeInfo.notifyList)
	{
		if (ImGuiMethod::TreeNode(notify->GetNotifyName()))
		{
			if (ImGui::Button("X"))	// �폜
			{
				_nodeInfo.notifyList.remove(notify);
				ImGui::TreePop();
				return;
			}

			// �ҏW
			notify->ImGuiCall();
			ImGui::TreePop();
		}
	}

	// �ǉ�
	// �����N���X���ŃC�x���g�쐬
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

	// �ʒm�C�x���g
	auto& notifyDataList =  nodeInfoData["notifyList"];
	for (auto& pNotify : _nodeInfo.notifyList)
	{
		nlohmann::json notifyData;
		notifyData["notifyParam"] = pNotify->Save();
		notifyData["notifyType"] = pNotify->GetNotifyType();
		notifyDataList.push_back(notifyData);
	}

	// �J�ږ��
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
	std::string nodeName;	// �m�[�h��
	LoadJsonString("nodeName", nodeName, _nodeInfoData);
	AnimationNode_Base::NodeType createType;	// �m�[�h�^�C�v
	LoadJsonEnum<AnimationNode_Base::NodeType>("nodeType", createType, _nodeInfoData);
	auto pCreateNodeInfo = CreateNodeInfoByType(createType, nodeName);	// �m�[�h���쐬

	nlohmann::json nodeData;	// �m�[�h�킲�Ƃ̃��[�h
	LoadJsonData("animNode", nodeData, _nodeInfoData);
	pCreateNodeInfo->pAnimNode->Load(nodeData);
}

void AnimationController::LoadNotify(const nlohmann::json& _nodeInfoData)
{
	// �ʒm�C�x���g���X�g���擾
	nlohmann::json notifyDataList;
	if (!LoadJsonDataArray("notifyList", notifyDataList, _nodeInfoData))
		return;

	// �m�[�h���擾
	std::string nodeName;
	if (!LoadJsonString("nodeName", nodeName, _nodeInfoData))
		return;

	AnimNodeInfo* pNodeInfo = GetNodeInfo(nodeName);
	if (!pNodeInfo) return;

	for (auto& notifyData : notifyDataList)
	{
		// �ʒm�C�x���g�̎��
		AnimationNotify_Base::NotifyType loadType;
		if (!LoadJsonEnum<AnimationNotify_Base::NotifyType>("notifyType", loadType, notifyData))
			continue;

		// �ʒm�C�x���g�̃p�����[�^
		nlohmann::json notifyParamData;
		if (!LoadJsonData("notifyParam", notifyParamData, notifyData))
			continue;

		// �쐬
		std::unique_ptr<AnimationNotify_Base> pLoadNotify =  pNotifyFactory->Create(loadType);
		pLoadNotify->Load(notifyParamData);

		// �ǉ�
		pNodeInfo->notifyList.push_back(std::move(pLoadNotify));
	}
}

void AnimationController::LoadTransArrow(const nlohmann::json& _nodeInfoData)
{
	// ���쐬
	nlohmann::json arrowDataList;
	HashiTaku::LoadJsonDataArray("arrowDataList", arrowDataList, _nodeInfoData);
	for (auto& arrowData : arrowDataList)
	{
		std::string toNodeName;	// �J�ڐ�
		HashiTaku::LoadJsonString("toName", toNodeName, arrowData);

		std::string fromName;	// �J�ڌ�
		HashiTaku::LoadJsonString("nodeName", fromName, _nodeInfoData);

		// �����쐬
		if (AnimTransitionArrow* pTransArrow = CreateTransitionArrow(fromName, toNodeName))
		{
			nlohmann::json arrowParamData;	// ���̃p�����[�^
			if (HashiTaku::LoadJsonData("arrowParam", arrowParamData, arrowData))
				pTransArrow->Load(arrowParamData);
		}
	}
}


AnimNodeInfo* AnimationController::CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName)
{
	if (_nodeName.empty()) return nullptr;	// ���O���͂��ĂȂ��Ȃ�I��

	std::string nodeName = _nodeName;
	NotDuplicateNodeName(nodeName);	// �R���g���[���[���ŏd�����Ȃ��悤�ȃm�[�h�����쐬����

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
		assert(!"�s���ȃm�[�h�^�C�v�ł�");
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
	std::vector<std::string> nodeNames;	// �S�m�[�h�����擾���Ă���
	for (auto& ni : animNodeInfos)
		nodeNames.push_back(ni->pAnimNode->GetNodeName());

	// �f�t�H���g�m�[�h�ݒ�
	ImGuiSetDefaultNode(nodeNames);

	if (ImGuiMethod::TreeNode("All Node"))
	{
		ImGuiNode(nodeNames);	// �����m�[�h
		ImGuiCreateNode();	// �m�[�h�ǉ�
		ImGui::TreePop();
	}

	// �p�����[�^
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
	while (nodeItr != animNodeInfos.end())	// �S�m�[�h
	{
		AnimationNode_Base& animNode = *(*nodeItr)->pAnimNode;
		bool isDelete = false;

		std::string nodeName = animNode.GetNodeName();
		if (ImGuiMethod::TreeNode(nodeName))
		{
			isDelete = ImGui::Button("Delete");	// �폜
			animNode.ImGuiCall();	// �m�[�h
			ImGuiMethod::LineSpaceMid();

			// �ʒm�C�x���g
			ImGui::Text("Notify");
			ImGuiAnimNotify(*(*nodeItr));
			ImGuiMethod::LineSpaceMid();

			// ���
			ImGui::Text("Arrow");
			ImGuiTransArrow(*(*nodeItr), _nodeNames);
			ImGui::TreePop();
		}

		// �m�[�h���폜
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
	// �R���{�{�b�N�X�Ńf�t�H���g�m�[�h�ݒ�
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

	// �m�[�h��
	static char input[IM_INPUT_BUF] = "";
	ImGui::InputText("NodeName", input, IM_INPUT_BUF);

	// �m�[�h�^�C�v
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
	// �������Ă�����p�����[�^
	auto itr = _nodeInfo.transitionArrows.begin();
	while (itr != _nodeInfo.transitionArrows.end())
	{
		bool isDelete = false;
		std::string toNodeName = (*itr)->GetToNode().GetNodeName();

		std::string treeCaption = "To " + toNodeName;
		if (ImGuiMethod::TreeNode(treeCaption.c_str()))
		{
			isDelete = ImGui::Button("X");	// �폜�{�^��
			(*itr)->ImGuiCall();

			// �D�揇��
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

	// �D�揇�ʕς����Ȃ�
	if (isChangePriority)
		_nodeInfo.transitionArrows.sort(SortArrowPriority);

	// �V���ɖ����쐬����
	ImGui::Text("CreateArrow");
	ImGuiMethod::ComboBox("ToNode", selectId, _nodeNames);
	ImGui::SameLine();
	if (ImGui::Button("+##TransArrow"))	// �쐬
	{
		std::string fromName = _nodeInfo.pAnimNode->GetNodeName();
		CreateTransitionArrow(fromName, _nodeNames[selectId]);
	}
#endif // EDIT
}