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
	// �f�t�H���g�m�[�h�����݂̃m�[�h�̃Z�b�g
	pCurrentNodeInfo = pDefaultNodeInfo;

	// �{�[�����X�g���Z�b�g
	pBoneList = &_boneList;
}

void AnimationController::Update(BoneList& _boneList)
{
	if (!IsCanPlay()) return;

	// �A�j���[�V�����X�V
	AnimatioUpdate();

	// �L���b�V���X�V
	CacheUpdate();

	// �J�ڂ̏�����B�����Ă��邩�m�F
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
	// �J�ڂ̎��Ԃ�i�߂�
	float transElapsedTime = pInertInterp->ProgressTransitionTime(playSpeed);

	if (pInertInterp->GetIsTransitionEnd())	// �u�����h���ԉ߂�����
	{
		// �J�ڏI���̏���
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
	if (!pNextNodeInfo)	// �A�j���[�V�������Ȃ�������
	{
		HASHI_DEBUG_LOG(_animName + "�͎擾�ł��܂���ł���");
		return;
	}

	// �Đ��J�n����
	isPlay = true;

	// �X�V����
	pPrevAnimNode = pCurrentNodeInfo->pAnimNode.get();
	pCurrentNodeInfo = GetNodeInfo(_animName);

	// �J�n����
	pCurrentNodeInfo->pAnimNode->Begin();
	pCurrentNodeInfo->pAnimNode->SetCurPlayRatio(0.0f);

	OnChangeAnimComplete();
}

void AnimationController::ChangeAnimation(const AnimTransitionArrow& _transitionArrow)
{
	// �Đ��J�n����
	isPlay = true;

	// �X�V����
	pPrevAnimNode = pPrevAnimNode = pCurrentNodeInfo->pAnimNode.get();
	pCurrentNodeInfo = GetNodeInfo(_transitionArrow.GetToNode());

	// �J�n����
	pCurrentNodeInfo->pAnimNode->Begin();

	// �J�ږ����Z�b�g����
	pCurTransArrow = &_transitionArrow;
	float targetAnimRatio = pCurTransArrow->GetTargetRatio();
	float transitionTime = pCurTransArrow->GetTransitionTime();
	EaseKind ease = pCurTransArrow->GetEaseKind();

	// ��ԊJ�n
	switch (pCurTransArrow->GetInterpolateKind())
	{
	case AnimInterpolateKind::CrossFade:
		CrossFadeStart(targetAnimRatio, transitionTime, ease);
		break;

	case AnimInterpolateKind::Inertialization:
		InterpTransitionStart(targetAnimRatio, transitionTime);
		break;

	default:
		assert(!"�A�j���[�V�����Ԃ̕�Ԃ̎�ގw�肪����łȂ��ł�");
		break;
	}
}


void AnimationController::SetBlendRatio(float _ratio)
{
	using enum AnimationNode_Base::NodeType;

	// ���݂̃m�[�h���u�����h�łȂ��Ȃ�
	AnimationNode_Base& currentNode = *pCurrentNodeInfo->pAnimNode;
	if (currentNode.GetNodeType() != Blend) return;

	BlendAnimationNode& pBlend = static_cast<BlendAnimationNode&>(currentNode);

	pBlend.SetTargetBlendRatio(_ratio);
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

void AnimationController::CreateBlendNode(const std::vector<std::string>& _animNames,
	const std::vector<float>& _ratios, const std::string& _nodeName)
{
	assert(_animNames.size() == _ratios.size() && "���O�Ɗ����̗v�f�����Ⴂ�܂�");

	std::unique_ptr<BlendAnimationNode> pCreateBlend = std::make_unique<BlendAnimationNode>(*pAnimParameters, _nodeName);

	u_int maxNum = static_cast<u_int>(_animNames.size());

	// �u�����h�A�j���[�V�����ɃA�j���[�V������ǉ�����
	for (u_int n_i = 0; n_i < maxNum; n_i++)
	{
		pCreateBlend->SetAnimationData(_animNames[n_i]);

		// �u�����h�������Z�b�g
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
		HASHI_DEBUG_LOG(_fromNodeName + " �J�ڌ��m�[�h��������܂���ł���");
		return nullptr;
	}

	AnimNodeInfo* toNodeInfo = GetNodeInfo(_toNodeName);
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
	fromNodeInfo->pTransArrows.push_back(std::move(pTransitionArrow));

	return pRetArrow;
}

void AnimationController::RemoveNode(const std::string& _nodeName)
{
	if (!GetNodeInfo(_nodeName)) return;	// �Ȃ������珈�����Ȃ�

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
		HASHI_DEBUG_LOG(_name + "��������܂���ł���");
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
	if (isTransitioning)	// �A�j���[�V�����J�ڒ��Ȃ�
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
	// �{�[���̃L���b�V���擾�̂��߃p�����[�^���擾����
	std::vector<BoneTransform> cacheTransforms;

	for (u_int b_i = 0; b_i < pBoneList->GetBoneCnt(); b_i++)
	{
		cacheTransforms.push_back(pBoneList->GetBone(b_i).GetAnimationTransform());
	}

	// ������ԗp�̃L���b�V�����X�V����
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

	// �g���K�[�ϐ������Z�b�g����
	pAnimParameters->ResetTrigger();

	if (!pTransArrow) return;	// �J�ڂ��Ȃ��Ȃ�I���

	ChangeAnimation(*pTransArrow);
}

void AnimationController::OnChangeAnimComplete()
{
	HASHI_DEBUG_LOG(pCurrentNodeInfo->pAnimNode->GetNodeName() + "�ֈڍs");
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
	while (IsHaveNode(_nodename))	// �m�[�h��������胋�[�v
	{
		_nodename = startName + std::to_string(loopCnt);
	}
}

void AnimationController::CrossFadeStart(float _targetAnimRatio, float _transitionTime, HashiTaku::EaseKind _easeKind)
{
	// �J�n���̊������Z�b�g
	AnimationNode_Base* pCurNode = pCurrentNodeInfo->pAnimNode.get();
	pCurNode->SetCurPlayRatio(_targetAnimRatio);

	pCrossFadeInterp->Begin(*pPrevAnimNode, *pCurNode, _transitionTime, _easeKind);

	OnTransitionStart();
}

void AnimationController::InterpTransitionStart(float _targetAnimRatio, float _transitionTime)
{
	u_int boneCnt = pBoneList->GetBoneCnt();
	std::vector<BoneTransform> requestPose(boneCnt);

	// �J�ڏI�����ɍĐ������̂Ń^�[�Q�b�g�̊������Z�b�g���Ă���
	AnimationNode_Base* pCurNode = pCurrentNodeInfo->pAnimNode.get();
	pCurNode->SetCurPlayRatio(_targetAnimRatio);

	// ���̃A�j���[�V�����̎p�����擾����
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		pCurNode->GetAnimTransform(requestPose[b_i], b_i, _targetAnimRatio);
	}

	// ������Ԃ̏�������
	bool isSuccess = pInertInterp->Calculate(requestPose, _transitionTime);

	if (!isSuccess) return;	// ���s������

	OnTransitionStart();
}

void AnimationController::OnTransitionEnd()
{
	OnChangeAnimComplete();

	isTransitioning = false;	// �J�ڏI��
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
	std::string nodeName;	// �m�[�h��
	LoadJsonString("nodeName", nodeName, _nodeInfoData);
	AnimationNode_Base::NodeType createType;	// �m�[�h�^�C�v
	LoadJsonEnum<AnimationNode_Base::NodeType>("nodeType", createType, _nodeInfoData);
	auto pCreateNodeInfo = CreateNodeInfoByType(createType, nodeName);	// �m�[�h���쐬

	nlohmann::json nodeData;	// �m�[�h�킲�Ƃ̃��[�h
	LoadJsonData("animNode", nodeData, _nodeInfoData);
	pCreateNodeInfo->pAnimNode->Load(nodeData);
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


AnimationController::AnimNodeInfo* AnimationController::CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName)
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

void AnimationController::Copy(const AnimationController& _other)
{
	if (this == &_other) return;

	CopyNodes(_other);	// �m�[�h���R�s�[
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

	std::vector<std::string> nodeNames;	// �S�m�[�h�����擾���Ă���
	for (auto& ni : animNodeInfos)
		nodeNames.push_back(ni->pAnimNode->GetNodeName());

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

		if (ImGuiMethod::TreeNode(animNode.GetNodeName()))
		{
			isDelete = ImGui::Button("Delete");	// �폜
			animNode.ImGuiCall();	// �m�[�h
			ImGuiMethod::LineSpaceMid();

			// ���
			ImGui::Text("Arrow");
			ImGuiTransArrow(*(*nodeItr), _nodeNames);
			ImGui::TreePop();
		}

		// �m�[�h���폜
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

	text = "�o�߁F" + std::to_string(interpolate->GetElapsedTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "�u�����h����" + std::to_string(interpolate->GetTransitionTime()) + "(s)";
	ImGui::Text(TO_UTF8(text));
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

	// �������Ă�����p�����[�^
	auto itr = _nodeInfo.pTransArrows.begin();
	while (itr != _nodeInfo.pTransArrows.end())
	{
		bool isDelete = false;
		std::string toNodeName = (*itr)->GetToNode().GetNodeName();

		std::string treeCaption = "To " + toNodeName;
		if (ImGuiMethod::TreeNode(treeCaption.c_str()))
		{
			(*itr)->ImGuiCall();
			isDelete = ImGui::Button("X");	// �폜�{�^��
			ImGui::TreePop();
		}

		if (isDelete)
			itr = _nodeInfo.pTransArrows.erase(itr);
		else
			itr++;
	}

	// �V���ɖ����쐬����
	ImGui::Text("CreateArrow");
	ImGuiMethod::ComboBox("ToNode", selectId, _nodeNames);
	ImGui::SameLine();
	if (ImGui::Button("+"))	// �쐬
	{
		std::string fromName = _nodeInfo.pAnimNode->GetNodeName();
		CreateTransitionArrow(fromName, _nodeNames[selectId]);
	}
#endif // EDIT
}