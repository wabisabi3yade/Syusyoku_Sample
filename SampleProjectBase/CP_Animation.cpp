#include "pch.h"
#include "CP_Animation.h"

#include "GameObject.h"

// ���b�V��
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"

// �A�Z�b�g�擾
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

CP_Animation::CP_Animation() : pSkeletalMesh(nullptr), playingTime_s(0.0f), playSpeed(1.0f), isPlaying(false)
{
	pAnimController = std::make_unique<AnimationController>();
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

	// �Đ����Ԃ�i�߂�
	ProgressPlayTime();

	//// �A�j���[�V�����s����X�V
	UpdateAnimationMtx();

	// �R���r�l�[�V�����s����X�V
	UpdateBoneCombMtx();

	// �{�[���s����o�b�t�@�Ƃ��đ��M
	UpdateBoneBuffer();
}

void CP_Animation::ImGuiSetting()
{
	static bool isWindowOpen = false;

	if (ImGui::Button(TO_UTF8("�E�B���h�E")))
		isWindowOpen = true;

	if (!isWindowOpen) return;

	std::string name = "�A�j���[�V����" + pAnimController->GetAssetName();
	ImGui::Begin(TO_UTF8(name), &isWindowOpen);

	ImGui::Text(std::to_string(playingTime_s).c_str());
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.1f);
	ImGui::Checkbox("Play", &isPlaying);

	pAnimController->ImGuiSetting();

	ImGui::End();
}

void CP_Animation::AddAnimations(const std::string& _animName)
{
	pAnimController->AddAnimation(_animName);
}

void CP_Animation::RemoveAnimations(const std::string& _animName)
{
	pAnimController->RemoveAnimation(_animName);
}

void CP_Animation::SetSkeletalMesh(SkeletalMesh& _skeletalMesh)
{
	pSkeletalMesh = &_skeletalMesh;
}

void CP_Animation::ProgressPlayTime()
{
	// ���Ԃ�i�߂�
	playingTime_s += playSpeed * MainApplication::DeltaTime();

	const AnimationData& pCurrentAnimation = GetCurrentAnimData();
	if (playingTime_s >pCurrentAnimation.GetAnimationTime())	// �A�j���[�V�����̑S�̎��Ԃ𒴂�����
		playingTime_s = 0.0f;
}

void CP_Animation::UpdateAnimationMtx()
{
	u_int boneNum = pSkeletalMesh->GetBoneNum();

	// �{�[���̃L���b�V���擾�̂��߃p�����[�^���擾����
	std::vector<BoneTransform> boneTransforms(boneNum);

	const AnimationData& currentAnimData = GetCurrentAnimData();
	// �`�����l���������[�v���ăA�j���[�V�����s����쐬
	for (unsigned int c_i = 0; c_i < currentAnimData.GetChannelCount(); c_i++)
	{
		std::string boneName = currentAnimData.GetBoneName(c_i);

		Bone* pBone = pSkeletalMesh->GetBoneByName(boneName);

		BoneTransform transform;

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		transform.scale = currentAnimData.GetScale(c_i, playingTime_s);

		//�N�H�[�^�j�I��
		transform.rotation = currentAnimData.GetQuaternion(c_i, playingTime_s);

		// ���W
		transform.position = currentAnimData.GetPosition(c_i, playingTime_s);

		// �A�j���[�V�����s����쐬
		Matrix scaleMtx = Matrix::CreateScale(transform.scale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(transform.rotation);
		Matrix transformMtx = Matrix::CreateTranslation(transform.position);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// �{�[���ɃA�j���[�V�����s����Z�b�g
		pBone->SetAnimationMtx(animationMtx);

		// �g�����X�t�H�[���z��ɒǉ�����
		boneTransforms[pBone->GetIndex()] = std::move(transform);
	}

	// �{�[���̃L���b�V�����X�V����
	GetCurrentNode().UpdateCache(boneTransforms, MainApplication::DeltaTime());
}

bool CP_Animation::IsCanPlay()
{
	// �Đ����@���@���݂̃A�j���[�V����������Ȃ�@�X�V����
	if (!isPlaying) return false;
	if (!pAnimController->IsSetAnimation()) return false;

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
		nodeMatrix = pBone.GetAnimMtx() ;
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
		const Bone& bone = pSkeletalMesh->GetBone(b_i);

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

void CP_Animation::UpdateTranslation()
{
}

AnimStateNode& CP_Animation::GetCurrentNode()
{
	assert(pAnimController->IsSetAnimation() && "�Đ����̃A�j���[�V�����m�[�h������܂���");

	return pAnimController->GetCurrentNode();
}

const AnimationData& CP_Animation::GetCurrentAnimData()
{
	assert(pAnimController->IsSetAnimation() && "�Đ����̃A�j���[�V�����f�[�^������܂���");

	return pAnimController->GetCurrentNode().GetAnimationData();
}
