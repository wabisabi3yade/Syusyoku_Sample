#include "pch.h"
#include "CP_Animation.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"
#include "AssetGetter.h"
#include "AnimationController.h"

using namespace DirectX::SimpleMath;

CP_Animation::BoneCombMtricies CP_Animation::boneComb;

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
	// �R���g���[���[���\��
	std::string text = "controllerName:";
	std::string controllerName = "Null";
	if (pAnimController)
		controllerName = pAnimController->GetAssetName();
	text += controllerName;
	ImGui::Text(text.c_str());

	// �R���g���[���[�ύX
	if (AssetGetter::ImGuiGetCombobox<AnimationController>("animationController", controllerName))
	{
		AnimationController* pGetAnimCon = AssetGetter::GetAsset<AnimationController>(controllerName);
		if (pGetAnimCon)
			SetAnimationController(*pGetAnimCon);
	}

	// �R���g���[���[�Đ�
	if (pAnimConPlayer)
		pAnimConPlayer->ImGuiCall();
}

void CP_Animation::SetBool(const std::string& _paramName, bool _isBool)
{
#ifdef EDIT
	// �R�s�[�A�j���[�V�����p�����[�^�����邩�m�F
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetBool(_paramName, _isBool);
}

void CP_Animation::SetInt(const std::string& _paramName, int _intVal)
{
#ifdef EDIT
	// �R�s�[�A�j���[�V�����p�����[�^�����邩�m�F
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetInt(_paramName, _intVal);
}

void CP_Animation::SetFloat(const std::string& _paramName, float _floatVal)
{
#ifdef EDIT
	// �R�s�[�A�j���[�V�����p�����[�^�����邩�m�F
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetFloat(_paramName, _floatVal);
}

void CP_Animation::SetTrigger(const std::string& _paramName)
{
#ifdef EDIT
	// �R�s�[�A�j���[�V�����p�����[�^�����邩�m�F
	if (!IsExistCopyAnimParameter()) return;
#endif // EDIT

	pAnimConPlayer->GetCopyAnimParameters().SetTrigger(_paramName);
}

bool CP_Animation::GetBool(const std::string& _paramName)
{
#ifdef EDIT
	// �R�s�[�A�j���[�V�����p�����[�^�����邩�m�F
	if (!IsExistCopyAnimParameter()) return false;
#endif // EDIT

	return pAnimConPlayer->GetCopyAnimParameters().GetBool(_paramName);
}

int CP_Animation::GetInt(const std::string& _paramName)
{
#ifdef EDIT
	// �R�s�[�A�j���[�V�����p�����[�^�����邩�m�F
	if (!IsExistCopyAnimParameter()) return 0;
#endif // EDIT

	return pAnimConPlayer->GetCopyAnimParameters().GetInt(_paramName);
}

float CP_Animation::GetFloat(const std::string& _paramName)
{
#ifdef EDIT
	// �R�s�[�A�j���[�V�����p�����[�^�����邩�m�F
	if (!IsExistCopyAnimParameter()) return 0.0f;
#endif // EDIT

	return pAnimConPlayer->GetCopyAnimParameters().GetFloat(_paramName);
}

void CP_Animation::SetupSkeletalMesh()
{
	// �����_���[�ɐݒ肵�Ă���X�P���^�����b�V�����擾
	CP_MeshRenderer* pMR = gameObject->GetComponent<CP_MeshRenderer>();
	pSkeletalMesh = dynamic_cast<SkeletalMesh*>(pMR->GetRenderMesh());
}

void CP_Animation::SetAnimationController(AnimationController& _controller)
{
	pAnimController = &_controller;
}

void CP_Animation::SetCurPlayerSpeed(float _setSpeed)
{
	pAnimConPlayer->GetCurNodePlayer().SetPlaySpeedTimes(_setSpeed);
}

SkeletalMesh& CP_Animation::GetSkeletalMesh()
{
	return *pSkeletalMesh;
}

AnimationController* CP_Animation::GetAnimationController()
{
	return pAnimController;
}

const DirectX::SimpleMath::Vector3& CP_Animation::GetMotionPosSpeedPerSec() const
{
#ifdef EDIT
	if (!pAnimConPlayer)
	{
		HASHI_DEBUG_LOG("�A�j���[�V�����Đ����쐬����Ă��܂���");
		return Vector3::Zero;
	}
#endif // EDIT

	return pAnimConPlayer->GetCurNodePlayer().GetRootMotionSpeed();
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
	if (!pSkeletalMesh) return;

	pAnimConPlayer = std::make_unique<AnimControllPlayer>(*pAnimController, pSkeletalMesh->GetBoneList(), GetTransform());
}

void CP_Animation::UpdateAnimationMtx()
{
	// �A�j���[�V�����v���C���[�ōX�V����
	pAnimConPlayer->Update();
}

bool CP_Animation::IsCanPlay()
{
	// �A�j���[�V�����R���g���[���[��ݒ�
	if (pAnimController == nullptr) return false;

	assert(pSkeletalMesh != nullptr && "�X�P���^�����b�V����ݒ�");

	return true;
}

bool CP_Animation::IsExistCopyAnimParameter()
{
	if (!pAnimConPlayer)
	{
		HASHI_DEBUG_LOG("�V�[�����J�n��Ăяo���Ă�������");
		return false;
	}

	return true;
}

void CP_Animation::UpdateBoneCombMtx()
{
	TreeNode& pRootNode = pSkeletalMesh->GetRootNode();

	// ���[�g�|�W�V�������W�������āA���b�V�����ړ������Ȃ��悤�ɂ���(y���W�͔��f���Ȃ�)
	Vector3 rootPos;
	pAnimConPlayer->GetCurrentRootPos(rootPos);
	rootPos.y = 0.0f;
	rootPos *= -1.0f;
	Matrix posMtx = Matrix::CreateTranslation(rootPos);


	// �m�[�h��H���đS�̂̃R���r�l�[�V�����s����X�V���Ă���
	UpdateNodeHierarchy(pRootNode, posMtx);
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
	offsetMtx = _other.offsetMtx;
	pAnimController = _other.pAnimController;
}
