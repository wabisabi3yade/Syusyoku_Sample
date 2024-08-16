#include "pch.h"
#include "CP_Animation.h"

#include "GameObject.h"

// ���b�V��
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"

// �A�Z�b�g�擾
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

CP_Animation::CP_Animation() : pSkeletalMesh(nullptr), pAnimController(nullptr)
{
}

void CP_Animation::Init()
{
	name = "CP_Animation";

	// �����_���[�ɐݒ肵�Ă���X�P���^�����b�V�����擾
	CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
	SkeletalMesh* pSetMesh = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());

	if (pSetMesh != nullptr)
	{
		pSkeletalMesh = pSetMesh;
	}
}

void CP_Animation::LateUpdate()
{
	if (!IsCanPlay()) return;

	//// �A�j���[�V�����s����X�V
	UpdateAnimationMtx();

	// �R���r�l�[�V�����s����X�V
	UpdateBoneCombMtx();

	// �{�[���s����o�b�t�@�Ƃ��đ��M
	UpdateBoneBuffer();
}

void CP_Animation::ImGuiSetting()
{
	static bool isWindowOpen = true;

	if (ImGui::Button(TO_UTF8("�E�B���h�E")))
		isWindowOpen = true;

	if (!isWindowOpen) return;

	std::string name = "�A�j���[�V����" + pAnimController->GetAssetName();
	ImGui::Begin(TO_UTF8(name), &isWindowOpen);

	pAnimController->ImGuiSetting();

	ImGui::End();
}

void CP_Animation::SetSkeletalMesh(SkeletalMesh& _skeletalMesh)
{
	pSkeletalMesh = &_skeletalMesh;
}

void CP_Animation::SetAnimationController(AnimationController& _controller)
{
	pAnimController = &_controller;
}

SkeletalMesh& CP_Animation::GetSkeletalMesh()
{
	return *pSkeletalMesh;
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

		// �R���r�l�[�V�����s������߂�
		pBone.CreateCombMtx(_parentMtx);
		nodeMatrix = pBone.GetAnimMtx();
	}

	// ���[���h�ϊ��̍s����X�V������
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

	// ���f���̃X�P�[���{�����A�j���[�V�����s��ɔ��f����
	Matrix scaleMtx = Matrix::CreateScale(pSkeletalMesh->GetScaleTimes());

	// �{�[�������[�v
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		const Bone& bone = *pSkeletalMesh->GetBone(b_i);

		// �{�[����ID�Ԗڂɍs�������
		boneComb.matrix[bone.GetIndex()] = bone.GetCombMtx() * scaleMtx;
		boneComb.matrix[bone.GetIndex()] = boneComb.matrix[bone.GetIndex()].Transpose();
	}

	// �}�e���A���Ƀ{�[���s���n��
	u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
	for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
	{
		Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
		/*pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));*/
		pMaterial->GetVertexShader().UpdateSubResource(1, &boneComb);
	}
}