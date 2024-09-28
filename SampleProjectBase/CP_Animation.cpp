#include "pch.h"
#include "CP_Animation.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"
#include "AssetGetter.h"
#include "AnimationController.h"

using namespace DirectX::SimpleMath;

Vector3 g_rootOffset;

CP_Animation::CP_Animation() : pSkeletalMesh(nullptr), pAnimController(nullptr)
{
}

CP_Animation::CP_Animation(const CP_Animation& _other)
{
	Copy(_other);
}

CP_Animation& CP_Animation::operator=(const CP_Animation& _other)
{
	Copy(_other);

	return *this;
}

void CP_Animation::Awake()
{
	// �X�P���^�����b�V������
	SetupSkeletalMesh();

	// �A�j���[�V�����R���g���[���[����
	SetupAnimCon();
}

void CP_Animation::Update()
{
	if (!IsCanPlay()) return;

	// �A�j���[�V�����s����X�V
	UpdateAnimationMtx();

	// �R���r�l�[�V�����s����X�V
	UpdateBoneCombMtx();

	// �{�[���s����o�b�t�@�Ƃ��đ��M
	UpdateBoneBuffer();
}

void CP_Animation::ImGuiSetting()
{
	static bool isWindowOpen = true;

	// �R���g���[���[���擾
	std::string controllerName;
	if (pAnimController)
		controllerName = pAnimController->GetAssetName();

	// �R���g���[���[���\��
	std::string text = "controllerName:";
	text += controllerName;
	ImGui::Text(text.c_str());

	// �R���g���[���[�ύX
	if (AssetGetter::ImGuiGetCombobox<AnimationController>("animationController", controllerName))
	{
		AnimationController* pGetAnimCon = AssetGetter::GetAsset<AnimationController>(controllerName);
		if (pGetAnimCon)
			SetAnimationController(*pGetAnimCon);
	}

	ImGui::Begin("Animation", &isWindowOpen);
	if (pAnimController)
		pAnimController->ImGuiCall();
	ImGui::End();
}

void CP_Animation::SetupSkeletalMesh()
{
	// �����_���[�ɐݒ肵�Ă���X�P���^�����b�V�����擾
	CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
	SkeletalMesh* pSetMesh = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());

	if (pSetMesh)	// �X�P���^�����b�V��������Ȃ�
		pSkeletalMesh = pSetMesh;
}

void CP_Animation::SetAnimationController(AnimationController& _controller)
{
	pAnimController = &_controller;
}

SkeletalMesh& CP_Animation::GetSkeletalMesh()
{
	return *pSkeletalMesh;
}

AnimationController* CP_Animation::GetAnimationController()
{
	return pAnimController;
}

nlohmann::json CP_Animation::Save()
{
	auto data = Component::Save();

	if (pAnimController)
	{
		data["animConName"] = pAnimController->GetAssetName();
	}

	return data;
}

void CP_Animation::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	std::string animConName = "";
	if (HashiTaku::LoadJsonString("animConName", animConName, _data))
	{
		AnimationController* pLoadController = AssetGetter::GetAsset<AnimationController>(animConName);
		if (pLoadController)
			SetAnimationController(*pLoadController);
	}
}

void CP_Animation::SetupAnimCon()
{
	if (pSkeletalMesh)
		pAnimController->Begin(pSkeletalMesh->GetBoneList());
}

void CP_Animation::UpdateAnimationMtx()
{
	// �A�j���[�V�����R���g���[���[�ōX�V����
	pAnimController->Update(pSkeletalMesh->GetBoneList());
}

bool CP_Animation::IsCanPlay()
{
	// �A�j���[�V�����R���g���[���[��ݒ�
	if (pAnimController == nullptr) return false;

	assert(pSkeletalMesh != nullptr && "�X�P���^�����b�V����ݒ�");

	return true;
}

void CP_Animation::UpdateBoneCombMtx()
{
	TreeNode& pRootNode = pSkeletalMesh->GetRootNode();

	Vector3 loadScales = Vector3::One * pSkeletalMesh->GetLoadOffsetScale();
	Vector3 loadAngles = pSkeletalMesh->GetLoadOffsetAngles();
	offsetMtx =
		Matrix::CreateScale(Vector3::One * pSkeletalMesh->GetLoadOffsetScale()) * Mtx::CreateRoratateMtx(loadAngles);

	Matrix transformMtx = pRootNode.GetTransformMtx();

	// �m�[�h��H���đS�̂̃R���r�l�[�V�����s����X�V���Ă���
	UpdateNodeHierarchy(pRootNode, Matrix::Identity);
}

void CP_Animation::UpdateNodeHierarchy(TreeNode& _treeNode, const Matrix& _parentMtx)
{
	Matrix nodeMatrix = Matrix::Identity;

	// �Ή������{�[��������Ȃ�
	if (_treeNode.HasBone())
	{
		Bone& pBone = _treeNode.GetBone();

		nodeMatrix = pBone.GetAnimMtx();

		// ���[�J����ԓ��̃{�[�����W�����߂�
		pBone.CreateGlobalMtx(_parentMtx, offsetMtx);

		// �R���r�l�[�V�����s������߂�
		pBone.CreateCombMtx(_parentMtx);
	}

	Matrix toWorldMtx = nodeMatrix * _parentMtx;

	// �ċA�I�Ƀ{�[�����X�V���Ă���
	for (u_int c_i = 0; c_i < _treeNode.GetChildNum(); c_i++)
	{
		UpdateNodeHierarchy(*_treeNode.GetChild(c_i), toWorldMtx);
	}
}

void CP_Animation::UpdateBoneBuffer()
{
	u_int boneCnt = pSkeletalMesh->GetBoneNum();

	// �{�[�������[�v
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		const Bone& bone = *pSkeletalMesh->GetBone(b_i);

		// �{�[����ID�Ԗڂɍs�������
		boneComb.matrix[bone.GetIndex()] = bone.GetCombMtx();
		boneComb.matrix[bone.GetIndex()] = boneComb.matrix[bone.GetIndex()].Transpose();
	}

	// �V�F�[�_�[�Ƀ{�[���s���n��
	u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
	for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
	{
		Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
		/*pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));*/
		pMaterial->GetVertexShader().UpdateSubResource(1, &boneComb);
	}
}

void CP_Animation::Copy(const CP_Animation& _other)
{
	if (&_other == this) return;

	Component::operator=(_other);

	pSkeletalMesh = _other.pSkeletalMesh;
	/*pAnimController = _other.pAnimController;*/
}
