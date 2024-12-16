#include "pch.h"
#include "CP_Animation.h"
#include "GameObject.h"
#include "SkeletalMesh.h"
#include "CP_MeshRenderer.h"
#include "AssetGetter.h"
#include "AnimationController.h"

namespace HashiTaku
{
	CP_Animation::BoneCombMtricies CP_Animation::boneCombBuffer;

	// �{�[���s��̃X���b�g�ԍ�
	constexpr int SHBUFFER_BONE_SLOT(1);

	CP_Animation::CP_Animation() :
		pSkeletalMesh(nullptr), pAnimController(nullptr), boneCnt(0)
	{
	}

	void CP_Animation::Init()
	{
		for (u_int i = 0; i < MAX_BONEMTX; i++)
		{
			boneCombBuffer.matrix[i] = DXSimp::Matrix::Identity.Transpose();
		}

		// �{�[���̃R�s�[���s��
		CopyBoneList();
	}

	void CP_Animation::Awake()
	{
		if (!pSkeletalMesh)
			CopyBoneList();

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
	}

	void CP_Animation::Draw()
	{
		// �{�[���s����o�b�t�@�Ƃ��đ��M
		UpdateBoneBuffer();
	}

	void CP_Animation::ImGuiDebug()
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

	void CP_Animation::SetAnimationController(AnimationController& _controller)
	{
		pAnimController = &_controller;
	}

	void CP_Animation::SetControllerPlaySpeed(float _setSpeed)
	{
		pAnimConPlayer->SetCurrentPlaySpeed(_setSpeed);
	}

	void CP_Animation::SetCurNodePlayerSpeed(float _setSpeed)
	{
		pAnimConPlayer->GetCurNodePlayer().SetPlaySpeedTimes(_setSpeed);
	}

	void CP_Animation::SetPlayRatio(float _playRatio)
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("�A�j���[�V�����Đ����쐬����Ă��܂���");
			return;
		}
#endif // EDIT

		pAnimConPlayer->GetCurNodePlayer().SetCurPlayRatio(_playRatio);
	}

	void CP_Animation::AddChangeAnimObserver(ChangeAnimObserver& _observer)
	{
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("�A�j���[�V�����Đ��N���X���������Ă��Ȃ��̂Œǉ��ł��܂���");
			return;
		}

		pAnimConPlayer->AddChangeAnimObserver(_observer);
	}

	void CP_Animation::RemoveChangeAnimObserver(ChangeAnimObserver& _observer)
	{
		pAnimConPlayer->RemoveChangeAnimObserver(_observer);
	}

	const DirectX::SimpleMath::Vector3& CP_Animation::GetMotionPosSpeedPerSec() const
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("�A�j���[�V�����Đ����쐬����Ă��܂���");
			return  DXSimp::Vector3::Zero;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurNodePlayer().GetRootMotionSpeed();
	}

	DirectX::SimpleMath::Vector3 CP_Animation::GetCurAnimRMPos(float _ratio)
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("�A�j���[�V�����Đ����쐬����Ă��܂���");
			return  DXSimp::Vector3::Zero;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurNodePlayer().GetRootMotionPos(_ratio, true);
	}

	float CP_Animation::GetControllerPlaySpeed() const
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("�A�j���[�V�����Đ����쐬����Ă��܂���");
			return 0.0f;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurrentPlaySpeed();
	}

	float CP_Animation::GetCurrentPlayRatio() const
	{
#ifdef EDIT
		if (!pAnimConPlayer)
		{
			HASHI_DEBUG_LOG("�A�j���[�V�����Đ����쐬����Ă��܂���");
			return 0.0f;
		}
#endif // EDIT

		return pAnimConPlayer->GetCurNodePlayer().GetCurPlayRatio();
	}

	BoneList* CP_Animation::GetMoveBoneList()
	{
		return pMoveBoneList.get();
	}

	u_int CP_Animation::GetBoneCnt() const
	{
		if (!pSkeletalMesh) return 0;

		return pSkeletalMesh->GetBoneCnt();
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
		if (LoadJsonString("animConName", animConName, _data))
		{
			AnimationController* pLoadController = AssetGetter::GetAsset<AnimationController>(animConName);
			if (pLoadController)
				SetAnimationController(*pLoadController);
		}
	}

	void CP_Animation::SetupAnimCon()
	{
		if (!pMoveBoneList) return;

		pAnimConPlayer = std::make_unique<AnimControllPlayer>(*pAnimController, *pMoveBoneList, GetTransform());
	}

	void CP_Animation::CopyBoneList()
	{
		CP_MeshRenderer* pMr = gameObject->GetComponent<CP_MeshRenderer>();
		if (!pMr) return;

		// �����_���[�ɐݒ肵�Ă���X�P���^�����b�V�����擾
		pSkeletalMesh = dynamic_cast<SkeletalMesh*>(pMr->GetRenderMesh());
		if (!pSkeletalMesh) return;

		// �A�j���[�V��������{�[�����R�s�[���A�쐬
		pMoveBoneList = std::make_unique<BoneList>(pSkeletalMesh->GetBoneList());

		// ���f���֌W
		pMr->SetVertexShader("VS_SkinAnimation");
	}

	void CP_Animation::UpdateAnimationMtx()
	{
		// �A�j���[�V�����v���C���[�ōX�V����
		pAnimConPlayer->Update(DeltaTime());
	}

	bool CP_Animation::IsCanPlay()
	{
#ifdef EDIT
		// �A�j���[�V�����R���g���[���[��ݒ�
		if (pAnimController == nullptr) return false;
		assert(pMoveBoneList && "�A�j���[�V��������{�[�����쐬����Ă��܂���");
#endif // EDIT

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
		using namespace DXSimp;

		const TreeNode* pRootNode = pSkeletalMesh->GetRootNode();

		// ���[�g�|�W�V�������W�������āA���b�V�����ړ������Ȃ��悤�ɂ���(y���W�͔��f���Ȃ�)
		Vector3 rootPos;
		pAnimConPlayer->GetCurrentRootPos(rootPos);
		rootPos.y = 0.0f;
		rootPos *= -1.0f;
		Matrix posMtx = Matrix::CreateTranslation(rootPos);

		// ���[�h�̉�]�A�X�P�[��
		Vector3 loadScales = Vector3::One * pSkeletalMesh->GetLoadOffsetScale();
		Vector3 loadAngles = pSkeletalMesh->GetLoadOffsetAngles();
		rootOffsetMtx =
			Matrix::CreateScale(Vector3::One * loadScales) * Mtx::CreateRoratateMtx(loadAngles);

		// �m�[�h��H���đS�̂̃R���r�l�[�V�����s����X�V���Ă���
		UpdateNodeHierarchy(*pRootNode, posMtx);
	}

	void CP_Animation::UpdateNodeHierarchy(const TreeNode& _treeNode, 
		const DXSimp::Matrix& _parentMtx)
	{
		DXSimp::Matrix nodeMatrix = DXSimp::Matrix::Identity;

		// �Ή������{�[��������Ȃ�
		if (_treeNode.HasBone())
		{
			u_int boneId = _treeNode.GetBoneIdx();
			Bone& moveBone = *pMoveBoneList->GetBone(boneId);
			nodeMatrix = moveBone.GetAnimMtx();

			// ���[�J����ԓ��̃{�[�����W�����߂�
			moveBone.CreateGlobalMtx(_parentMtx, rootOffsetMtx);

			// �R���r�l�[�V�����s������߂�
			moveBone.CreateCombMtx(_parentMtx);
		}

		DXSimp::Matrix toWorldMtx = nodeMatrix * _parentMtx;

		// �ċA�I�Ƀ{�[�����X�V���Ă���
		for (u_int c_i = 0; c_i < _treeNode.GetChildNum(); c_i++)
		{
			UpdateNodeHierarchy(*_treeNode.GetChild(c_i), toWorldMtx);
		}
	}

	void CP_Animation::UpdateBoneBuffer()
	{
		if (!pSkeletalMesh) return;

		if (pMoveBoneList)	// �������{�[�������邩
		{
			u_int bufferCnt = static_cast<u_int>(pMoveBoneList->GetBoneCnt());
			// �{�[�������[�v
			for (u_int b_i = 0; b_i < bufferCnt; b_i++)
			{
				const Bone& bone = *pMoveBoneList->GetBone(b_i);

				// �{�[����ID�Ԗڂɍs�������
				boneCombBuffer.matrix[bone.GetIndex()] = bone.GetCombMtx();

				// �V�F�[�_�[�ɓn���̂œ]�u�s����쐬
				boneCombBuffer.matrix[bone.GetIndex()] =
					boneCombBuffer.matrix[bone.GetIndex()].Transpose();
			}
		}

		// �V�F�[�_�[�Ƀ{�[���s���n��
		u_int mtrlCnt = pSkeletalMesh->GetMaterialNum();
		for (u_int m_i = 0; m_i < mtrlCnt; m_i++)
		{
			Material* pMaterial = pSkeletalMesh->GetMaterial(m_i);
			/*pMaterial->GetVertexShader().Map(1, &boneComb, sizeof(BoneCombMtricies));*/
			pMaterial->GetVertexShader().UpdateSubResource(1, &boneCombBuffer);
		}
	}
}