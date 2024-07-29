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

u_int g_cnt = 0;

namespace Anim
{
	Matrix ToDirectXMatrix(const aiMatrix4x4& _aiMatrix)
	{
		Matrix dxMatrix = Matrix(
			_aiMatrix.a1, _aiMatrix.b1, _aiMatrix.c1, _aiMatrix.d1,
			_aiMatrix.a2, _aiMatrix.b2, _aiMatrix.c2, _aiMatrix.d2,
			_aiMatrix.a3, _aiMatrix.b3, _aiMatrix.c3, _aiMatrix.d3,
			_aiMatrix.a4, _aiMatrix.b4, _aiMatrix.c4, _aiMatrix.d4
		);

		return dxMatrix;
	}
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
	// �Đ����@���@���݂̃A�j���[�V����������Ȃ�@�X�V����
	if (!isPlaying) return;
	if (pCurrentAnimation == nullptr) return;

	assert(pSkeletalMesh != nullptr && "�X�P���^�����b�V����ݒ�");

	// �Đ����Ԃ�i�߂�
	ProgressPlayTime();

	// �A�j���[�V�����s����X�V
	UpdateAnimationMtx();

	// �R���r�l�[�V�����s����X�V
	UpdateBoneCombMtx();

	// �{�[���s����o�b�t�@�Ƃ��đ��M
	UpdateBoneBuffer();
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

	// �A�j���[�V�����̑S�̎��Ԃ𒴂�����
	/*if (playingTime_s > pCurrentAnimation->GetAnimationTime())
		playingTime_s = 0.0f;*/
}

void CP_Animation::UpdateBoneCombMtx()
{

	g_cnt = 0;
	const aiNode* pRootNode = pCurrentAnimation->GetRootNode();

	// �m�[�h��H���đS�̂̃R���r�l�[�V�����s����X�V���Ă���
	UpdateNodeHierarchy(*pRootNode, Matrix::Identity);

	return;
}

void CP_Animation::UpdateNodeHierarchy(const aiNode& _aiNode, const Matrix& _parentMtx)
{
	Matrix nodeMatrix = Anim::ToDirectXMatrix(_aiNode.mTransformation);

	std::string nodeName = _aiNode.mName.C_Str();

	//Bone* pBone = GetBoneByName(nodeName);

	//if (pBone != nullptr)
	//{
	//	// �R���r�l�[�V�����s������߂� (�{�[���I�t�Z�b�g * �A�j���[�V���� * �t�I�t�Z�b�g * �e�܂ł̍s��)
	//	pBone->CreateCombMtx(_parentMtx);
	//	nodeMatrix = pBone->GetAnimMtx();
	//	g_cnt++;
	//}

	// ���[���h�ϊ��̍s����X�V������
	Matrix toWorldMtx = nodeMatrix * _parentMtx;

	// �q�m�[�h�̍s����X�V�i�ċA�I�j
	for (u_int c_i = 0; c_i < _aiNode.mNumChildren; c_i++)
	{
		UpdateNodeHierarchy(*_aiNode.mChildren[c_i], Matrix::Identity);
	}
}

void CP_Animation::UpdateAnimationMtx()
{
	static u_int flame = 0;

	// �{�[���������[�v���ăR���r�l�[�V�����s����쐬
	for (unsigned int c_i = 0; c_i < pCurrentAnimation->GetChannelCount(); c_i++)
	{
		std::string boneName = pCurrentAnimation->GetBoneName(c_i);

		std::vector<Bone*> pBones = GetBoneByName(boneName);

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		Vector3 animScale = pCurrentAnimation->GetScale(c_i, flame);

		//�N�H�[�^�j�I��
		Quaternion animQuat = pCurrentAnimation->GetQuaternion(c_i, flame);

		// ���W
		Vector3 animPos = pCurrentAnimation->GetPosition(c_i, flame);

		/*HASHI_DEBUG_LOG(boneName + " " + std::to_string(animQuat.x));*/
		static Vector3 sv = Vector3::Zero;

		/*sv += Vector3::One * 0.01f;*/

		// �A�j���[�V�����s����쐬
		Matrix scaleMtx = Matrix::CreateScale(animScale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(animQuat);
		Matrix transformMtx = Matrix::CreateTranslation(animPos);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// �{�[���ɃA�j���[�V�����s����Z�b�g
		for (auto& b : pBones)
			b->SetAnimationMtx(animationMtx);
	}

	flame++;
}

void CP_Animation::UpdateBoneBuffer()
{
	u_int cnt = 0;
	const std::vector<BonePerMesh>& bones = pSkeletalMesh->GetBones();
	u_int boneGroupCnt = static_cast<u_int>(bones.size());

	// ���b�V�������[�v
	for (u_int m_i = 0; m_i < boneGroupCnt; m_i++)
	{
		const BonePerMesh& bonePerMesh = bones[m_i];
		u_int boneCnt = static_cast<u_int>(bones[m_i].size());

		// �{�[�������[�v
		for (u_int b_i = 0; b_i < boneCnt; b_i++)
		{
			// �{�[����ID�Ԗڂɍs�������
			boneComb.matrix[bonePerMesh[b_i]->GetIndex()] = bonePerMesh[b_i]->GetCombMtx();
			boneComb.matrix[bonePerMesh[b_i]->GetIndex()].Transpose();
			cnt++;
		}

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

void CP_Animation::CPUAnimation()
{
	u_int meshCnt = pSkeletalMesh->GetMeshNum();
	for (u_int m_i = 0; m_i < meshCnt; m_i++)
	{
		SingleMesh* sm = pSkeletalMesh->GetMesh(m_i);
		u_int vertexCnt = sm->GetVertexNum();
		for (u_int v_i = 0; v_i < vertexCnt; v_i++)
		{

		}
	}
}

std::vector<Bone*> CP_Animation::GetBoneByName(const std::string& _boneName)
{
	assert(pSkeletalMesh != nullptr);

	std::vector<Bone*> pRetBones;

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

			pRetBones.push_back(bonePerMesh[b_i].get());
		}
	}

	/*assert(pRetBone != nullptr && "�{�[����������܂���ł���");*/

	return pRetBones;
}

void CP_Animation::RemoveStr(std::string& _str, const std::string& _removeStr)
{
	size_t pos = std::string::npos;
	// ����̕���������ׂČ����č폜
	while ((pos = _str.find(_removeStr)) != std::string::npos) {
		_str.erase(pos);
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

void CP_Animation::ConnectBoneId(AnimationData& _connectAnim)
{
	if (pSkeletalMesh == nullptr)
	{
		HASHI_DEBUG_LOG("��ɃX�P���^�����b�V����ݒ肵�Ă�������");
		return;
	}



}