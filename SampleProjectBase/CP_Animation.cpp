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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
	if (!isPlaying) return;
	if (pCurrentAnimation == nullptr) return;

	assert(pSkeletalMesh != nullptr && "�X�P���^�����b�V����ݒ�");

	// �Đ����Ԃ�i�߂�
	ProgressPlayTime();

	// �A�j���[�V�����s����X�V
	UpdateAnimationMtx();

	// �R���r�l�[�V�����s����X�V
	UpdateBoneCombMtx();
}

void CP_Animation::ImGuiSetting()
{
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

	if (playingTime_s > pCurrentAnimation->GetAnimationTime())
		playingTime_s = 0.0f;
}

void CP_Animation::UpdateBoneCombMtx()
{
	const aiNode* pRootNode = pCurrentAnimation->GetRootNode();

	// �m�[�h��H���đS�̂̃R���r�l�[�V�����s����X�V���Ă���
	UpdateNodeHierarchy(*pRootNode, Matrix::Identity);
}

void CP_Animation::UpdateNodeHierarchy(const aiNode& _aiNode, const Matrix& _parentMtx)
{
	std::string nodeName = _aiNode.mName.C_Str();

	Bone* pBone = GetBoneByName(nodeName);

	// �R���r�l�[�V�����s������߂� (�{�[���I�t�Z�b�g * �A�j���[�V���� * �t�I�t�Z�b�g * �e�܂ł̍s��)
	pBone->CreateCombMtx(_parentMtx);

	// ���[���h�ϊ��̍s����X�V������
	Matrix toWorldMtx = _parentMtx * pBone->GetAnimMtx();

	// �q�m�[�h�̍s����X�V�i�ċA�I�j
	for (u_int c_i = 0; c_i < _aiNode.mNumChildren; c_i++)
	{
		UpdateNodeHierarchy(*_aiNode.mChildren[c_i], toWorldMtx);
	}
}

void CP_Animation::UpdateAnimationMtx()
{
	// �{�[���������[�v���ăR���r�l�[�V�����s����쐬
	for (unsigned int c_i = 0; c_i < pCurrentAnimation->GetChannelCount(); c_i++)
	{
		std::string boneName = pCurrentAnimation->GetBoneName(c_i);

		Bone* pBone = GetBoneByName(boneName);

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		Vector3 animScale = pCurrentAnimation->GetScale(c_i, playingTime_s);

		//�N�H�[�^�j�I��
		Quaternion animQuat = pCurrentAnimation->GetQuaternion(c_i, playingTime_s);

		// ���W
		Vector3 animPos = pCurrentAnimation->GetPosition(c_i, playingTime_s);

		HASHI_DEBUG_LOG("quat" +std::to_string(animQuat.x) + " " +
			std::to_string(animQuat.y) + " " +
			std::to_string(animQuat.z) + " " + 
			std::to_string(animQuat.w));

		HASHI_DEBUG_LOG("scale" + std::to_string(animScale.x) + " " +
			std::to_string(animScale.y) + " " +
			std::to_string(animScale.z));

		HASHI_DEBUG_LOG("pos" + std::to_string(animPos.x) + " " +
			std::to_string(animPos.y) + " " +
			std::to_string(animPos.z));

		// �A�j���[�V�����s����쐬
		Matrix scaleMtx = Matrix::CreateScale(animScale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(animQuat);
		Matrix transformMtx = Matrix::CreateTranslation(animPos);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// �{�[���ɃA�j���[�V�����s����Z�b�g
		pBone->SetAnimationMtx(animationMtx);
	}
}

Bone* CP_Animation::GetBoneByName(const std::string& _boneName)
{
	assert(pSkeletalMesh != nullptr);

	Bone* pRetBone = nullptr;

	const std::vector<BonePerMesh>& bones = pSkeletalMesh->GetBones();

	// ���b�V���̐����擾
	u_int meshCnt = static_cast<u_int>(bones.size());

	// ����m�F���Ă���
	// ���b�V�������[�v
	for (u_int m_i = 0; m_i < meshCnt; m_i++)
	{
		// ���b�V�����̃{�[���̐����擾
		const BonePerMesh& bonePerMesh = bones[m_i];
		u_int boneCnt = static_cast<u_int>(bonePerMesh.size());

		// �{�[�������[�v
		for (u_int b_i = 0; b_i < boneCnt; b_i++)
		{
			if (bonePerMesh[b_i]->GetBoneName() != _boneName)
				continue;

			// ��������
			pRetBone = bonePerMesh[b_i].get();
			break;
		}

		if (pRetBone != nullptr)
			break;
	}

	assert(pRetBone != nullptr && "�{�[����������܂���ł���");

	return nullptr;
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

void CP_Animation::ConnectBoneId(AnimationData& _connectAnim)
{
	if (pSkeletalMesh == nullptr)
	{
		HASHI_DEBUG_LOG("��ɃX�P���^�����b�V����ݒ肵�Ă�������");
		return;
	}



}
