#include "pch.h"
#include "CP_Animation.h"

#include "GameObject.h"

// �A�j���[�V����
#include "AnimationData.h"

// ���b�V��
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"

// �A�Z�b�g�擾
#include "AssetGetter.h"

using namespace DirectX::SimpleMath;

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
	ImGui::DragFloat("PlaySpeed", &playSpeed, 0.1f);

	ImGui::Checkbox("Play", &isPlaying);

	// �R���{�{�b�N�X
	std::string currentName = "";
	if (pCurrentAnimation != nullptr)
		currentName = pCurrentAnimation->GetName();

	std::vector<std::string> animNames;
	for (auto& anim : pHaveAnimations)
	{
		animNames.push_back(anim->GetName());
	}
	bool isChange = ImGuiMethod::ComboBox(TO_UTF8("�A�j���[�V����"), currentName, animNames);
	if (isChange)
	{
		PlayAnimation(currentName);
	}

}

void CP_Animation::PlayAnimation(const std::string& _animName)
{
	// ���O����A�j���[�V�������Đ�����
	AnimationData* pPlayAnim = FindAnimaton(_animName);
	if (pPlayAnim == nullptr) return;

	pCurrentAnimation = pPlayAnim;
	isPlaying = true;
}

void CP_Animation::AddAnimations(AnimationData& _addAnim)
{
	pHaveAnimations.push_back(&_addAnim);
}

void CP_Animation::AddAnimations(const std::string& _animName)
{
	AnimationData* pAnimData = AssetGetter::GetAsset<AnimationData>(_animName);

	if (pAnimData == nullptr)
		return;

	pHaveAnimations.push_back(pAnimData);
}

void CP_Animation::RemoveAnimations(const std::string& _animName)
{
	// �폜�������m�F���邽�߂�
	// �v�f�����擾
	u_int num = static_cast<u_int>(pHaveAnimations.size());

	// ���O�������A�j���[�V������T���Ď�菜��
	pHaveAnimations.remove_if([&](AnimationData* pAnim)
		{
			return pAnim->GetName() == _animName;
		});

	if (static_cast<u_int>(pHaveAnimations.size()) != num)
		HASHI_DEBUG_LOG("����ɍ폜����܂���");
}

void CP_Animation::SetSkeletalMesh(SkeletalMesh& _skeletalMesh)
{
	pSkeletalMesh = &_skeletalMesh;
}

void CP_Animation::ProgressPlayTime()
{
	// ���Ԃ�i�߂�
	playingTime_s += playSpeed * MainApplication::DeltaTime();

	// �A�j���[�V�����̑S�̎��Ԃ𒴂�����
	if (playingTime_s > pCurrentAnimation->GetAnimationTime())
		playingTime_s = 0.0f;
}

void CP_Animation::UpdateAnimationMtx()
{
	static u_int flame = 0;

	// �`�����l���������[�v���ăA�j���[�V�����s����쐬
	for (unsigned int c_i = 0; c_i < pCurrentAnimation->GetChannelCount(); c_i++)
	{
		std::string boneName = pCurrentAnimation->GetBoneName(c_i);

		Bone* pBone = pSkeletalMesh->GetBoneByName(boneName);

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		Vector3 animScale = /*pCurrentAnimation->GetScale(c_i, playingTime_s)*/Vector3::One;

		//�N�H�[�^�j�I��
		Quaternion animQuat = pCurrentAnimation->GetQuaternion(c_i, playingTime_s);

		// ���W
		Vector3 animPos = pCurrentAnimation->GetPosition(c_i, playingTime_s);

		// �A�j���[�V�����s����쐬
		Matrix scaleMtx = Matrix::CreateScale(Vector3::One);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(animQuat);
		Matrix transformMtx = Matrix::CreateTranslation(animPos);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// �{�[���ɃA�j���[�V�����s����Z�b�g
		pBone->SetAnimationMtx(animationMtx);
	}

	flame++;
}

bool CP_Animation::IsCanPlay()
{
	// �Đ����@���@���݂̃A�j���[�V����������Ȃ�@�X�V����
	if (!isPlaying) return false;
	if (pCurrentAnimation == nullptr) false;

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

	// �{�[�������[�v
	for (u_int b_i = 0; b_i < boneCnt; b_i++)
	{
		const Bone& bone = pSkeletalMesh->GetBone(b_i);

		// �{�[����ID�Ԗڂɍs�������
		boneComb.matrix[bone.GetIndex()] = bone.GetCombMtx();
		boneComb.matrix[bone.GetIndex()] = boneComb.matrix[bone.GetIndex()].Transpose();
	}

	// �}�e���A���Ƀ{�[���s���n��
	u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
	for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
	{
		Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
		//pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));
		pMaterial->GetVertexShader().UpdateSubResource(1, &boneComb);
	}
}

AnimationData* CP_Animation::FindAnimaton(const std::string& _animName)
{
	// �������O�̃A�j���[�V������T��
	auto itr = std::find_if(pHaveAnimations.begin(), pHaveAnimations.end(),
		[&](AnimationData* pAnim)
		{
			return pAnim->GetName() == _animName;
		});

	if (itr == pHaveAnimations.end())
	{
		HASHI_DEBUG_LOG(_animName + "��������܂���ł���");
		return nullptr;
	}

	return *itr;
}
