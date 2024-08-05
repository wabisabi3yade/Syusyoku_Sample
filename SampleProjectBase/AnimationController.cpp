#include "pch.h"
#include "AnimationController.h"
// �A�Z�b�g
#include "AssetGetter.h"
// �{�[�����X�g�擾���邽��
#include "SkeletalMesh.h"
// �������
#include "InertInterpAnimation.h"

using namespace DirectX::SimpleMath;

AnimationController::AnimationController() : pCurrentAnimNode(nullptr), pNextAnimNode(nullptr), isTransitioning(false)
{
	inertInterp = std::make_unique<InertInterpAnimation>();
}

void AnimationController::Update(BoneList& _boneList, float _playingTime)
{
	// �{�[���̃L���b�V���擾�̂��߃p�����[�^���擾����
	std::vector<BoneTransform> boneTransforms(_boneList.GetBoneCnt());

	if (isTransitioning)	// �A�j���[�V�����J�ڒ��Ȃ�
		TransitionUpdate(_boneList, _playingTime, boneTransforms);
	else
		NormalUpdate(_boneList, _playingTime, boneTransforms);

	// ������ԗp�̃L���b�V�����X�V����
	inertInterp->UpdateBoneCache(boneTransforms);
}

void AnimationController::NormalUpdate(BoneList& _boneList, float& _playingTime, std::vector<BoneTransform>& _cacheTransform)
{
	const AnimationData& currentAnimData = pCurrentAnimNode->GetAnimationData();

	//�{�[�������[�v
	for (unsigned int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		// �Đ����Ԃ���e�p�����[�^���擾
		// �X�P�[��
		transform.scale = currentAnimData.GetScale(b_i, _playingTime);

		//�N�H�[�^�j�I��
		transform.rotation = currentAnimData.GetQuaternion(b_i, _playingTime);

		// ���W
		transform.position = currentAnimData.GetPosition(b_i, _playingTime);

		// �A�j���[�V�����s����쐬
		Matrix scaleMtx = Matrix::CreateScale(transform.scale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(transform.rotation);
		Matrix transformMtx = Matrix::CreateTranslation(transform.position);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// �{�[���ɃA�j���[�V�����s����Z�b�g
		bone.SetAnimationMtx(animationMtx);

		// �L���b�V���z��ɒǉ�����
		_cacheTransform[b_i] = std::move(transform);
	}
}

void AnimationController::TransitionUpdate(BoneList& _boneList, float& _playingTime, std::vector<BoneTransform>& _cacheTransform)
{
	blendElapsedTime += MainApplication::DeltaTime();

	if (blendElapsedTime > blendTime)	// �u�����h���ԉ߂�����
	{
		// �J�ڏI���̏���
		TransitionEnd(_playingTime);
	}

	for (u_int b_i = 0; b_i < _boneList.GetBoneCnt(); b_i++)
	{
		Bone& bone = _boneList.GetBone(b_i);

		BoneTransform transform;

		transform.position = inertInterp->CalcBlendPos(b_i, blendElapsedTime);
		transform.scale = inertInterp->CalcBlendScale(b_i, blendElapsedTime);
		transform.rotation = inertInterp->CalcBlendRot(b_i, blendElapsedTime);

		// �A�j���[�V�����s����쐬
		Matrix scaleMtx = Matrix::CreateScale(transform.scale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(transform.rotation);
		Matrix transformMtx = Matrix::CreateTranslation(transform.position);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		// �{�[���ɃA�j���[�V�����s����Z�b�g
		bone.SetAnimationMtx(animationMtx);

		// �L���b�V���z��ɒǉ�����
		_cacheTransform[b_i] = std::move(transform);
	}
}

void AnimationController::ImGuiSetting()
{
	//if (!ImGui::TreeNode(TO_UTF8("�A�j���[�V�����R���g���[���["))) return;

	if (IsSetAnimation())
	{
		pCurrentAnimNode->ImGuiSetting();
	}

	ImGui::Text(TO_UTF8("�A�j���[�V�����J��"));
	for (auto& a : pAnimationNodes)	// �{�^���ŃA�j���[�V�����ς���
	{
		std::string animName = a.second->GetAnimationData().GetAssetName();
		if(ImGui::Button(animName.c_str()))
		{
			ChangeAnimation(animName);	// �A�j���[�V�����ύX
		}
	}

	ImGui::Text(TO_UTF8("�J�ڏ��"));

	if (isTransitioning)
		ImGuiTransition();

	/*ImGui::TreePop();*/
}

void AnimationController::ChangeAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName))
	{
		HASHI_DEBUG_LOG(_animName + "�͎擾�ł��܂���ł���");
		return;
	}
	

	pNextAnimNode = pAnimationNodes[_animName].get();

	// ������Ԃ̏�������
	bool isSuccess = inertInterp->Calculate(*pNextAnimNode, blendTime);

	if (isSuccess)	// �������Ă�����
	{
		isTransitioning = true;
		blendElapsedTime = 0.0f;
	}


}

void AnimationController::AddAnimation(const std::string& _animName)
{
	// �����A�j���[�V��������������
	if (IsHaveAnim(_animName))	return;

	AnimationData* pGetAnim = AssetGetter::GetAsset<AnimationData>(_animName);

	// �A�j���[�V�������Ȃ�������
	if (pGetAnim == nullptr) return;

	// �m�[�h���쐬����
	std::unique_ptr<AnimStateNode> pAnimNode = std::make_unique<AnimStateNode>(_animName);
	pAnimNode->SetAnimationData(*pGetAnim);

	pAnimationNodes[_animName] = std::move(pAnimNode);

	if (!IsSetAnimation())	// �Đ����̃A�j���[�V�������Ȃ��Ȃ�Z�b�g����
		pCurrentAnimNode = pAnimationNodes[_animName].get();
}

void AnimationController::RemoveAnimation(const std::string& _animName)
{
	if (!IsHaveAnim(_animName)) return;

	pAnimationNodes.erase(_animName);
}

bool AnimationController::IsSetAnimation()
{
	return pCurrentAnimNode != nullptr;
}

AnimStateNode& AnimationController::GetCurrentNode()
{
	return *pCurrentAnimNode;
}

void AnimationController::ImGuiTransition()
{
	std::string text = pCurrentAnimNode->GetNodeName();
	text += " ---> ";
	text += pNextAnimNode->GetNodeName();
	ImGui::Text(TO_UTF8(text));

	text = "�o�߁F" + std::to_string(blendElapsedTime) + "(s)";
	ImGui::Text(TO_UTF8(text));

	text = "�u�����h����" + std::to_string(blendTime) + "(s)";
	ImGui::Text(TO_UTF8(text));
}

bool AnimationController::IsHaveAnim(const std::string& _animName)
{
	// �z����ɓ������O�����邩
	for (auto& a : pAnimationNodes)
	{
		if (_animName == a.first)
			return true;
	}

	return false;
}

void AnimationController::TransitionEnd(float& _playTime)
{
	_playTime = 0.0f;
	
	pCurrentAnimNode = pNextAnimNode;
	pNextAnimNode = nullptr;
	isTransitioning = false;
}
