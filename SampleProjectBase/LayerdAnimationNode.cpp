#include "pch.h"
#include "LayerdAnimationNode.h"
#include "AssetGetter.h"
#include "SkeletalMesh.h"

namespace HashiTaku
{
	constexpr int NOT_SET_BONEID(-1);	// �{�[�����w�肵�Ă��Ȃ����̃{�[��ID
	constexpr u_int START_BLEND_DEPTH(9999);	// �J�n���̃u�����h�̐[��

	LayerdAnimationNode::LayerdAnimationNode(const BoneList& _boneList, const std::string& _nodeName) :
		AnimationNode_Base(_nodeName, NodeType::Layerd),
		pBoneList(&_boneList),
		pBaseAnimation(nullptr),
		pBlendAnimation(nullptr),
		beginBlendPlayFrame(0),
		beginBlendBoneId(NOT_SET_BONEID),
		isMeshSpaceRotationBlend(false),
		blendDepth(0)
	{
		blendSpeedCurve.SetCurveName("Blend Speed");
	}

	void LayerdAnimationNode::GetAnimTransform(std::vector<BoneTransform>& _outTransforms, float _baseRatio, float _blendRatio) const
	{
		// �@�\�ł��Ă��Ȃ��Ȃ�
		if (!CanWarking()) return;

		if (isMeshSpaceRotationBlend)	// ���b�V����Ԃŉ�]�ʂ̃u�����h���s��
		{
			GetMeshSpaceBlendTransform(_outTransforms,
				pBoneList->GetRootNode(),
				DXSimp::Quaternion::Identity,
				DXSimp::Quaternion::Identity,
				DXSimp::Quaternion::Identity,
				_baseRatio,
				_blendRatio,
				START_BLEND_DEPTH);
		}
		else 	// ���[�J����Ԃŉ�]�ʂ̃u�����h���s��
		{
			GetLocalSpaceBlendTransform(_outTransforms,
				_baseRatio,
				_blendRatio);
		}

	}

	void LayerdAnimationNode::GetMeshSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms,
		const TreeNode* _pCurrentNode,
		const DXSimp::Quaternion& _parenBaseRotation,
		const DXSimp::Quaternion& _parentBlendRotation,
		const DXSimp::Quaternion& _parentCalculateRotation,
		float _baseRatio,
		float _blendRatio,
		u_int _curBlendDepth) const
	{
		// �{�[�����Ȃ��Ȃ玟�ɐi��
		if (!_pCurrentNode->HasBone())
		{
			u_int childCnt = _pCurrentNode->GetChildNum();
			for (u_int c_i = 0; c_i < childCnt; c_i++)
				GetMeshSpaceBlendTransform(_outTransforms,
					_pCurrentNode->GetChild(c_i),
					_parenBaseRotation,
					_parentBlendRotation,
					_parentCalculateRotation,
					_baseRatio,
					_blendRatio,
					_curBlendDepth + 1
				);

			return;
		}

		// �u�����h�J�n�̃m�[�h�Ȃ�[����0�ɂ���
		if (_pCurrentNode->GetBoneIdx() == beginBlendBoneId)
			_curBlendDepth = 1;

		u_int uBoneId = static_cast<u_int>(_pCurrentNode->GetBoneIdx());
		const Bone& bone = *_pCurrentNode->GetBone();

		// ���b�V����Ԃ̉�]��(����ւ̈�����)
		DXSimp::Quaternion baseMeshSpaceRotation;
		DXSimp::Quaternion blendMeshSpaceRotation;
		DXSimp::Quaternion calcMeshSpaceRotation;

		// �u�����h���̃A�j���[�V�����Ȃ�(�J�n���̐[���������A���Ȃ킿�u�����h�J�n�{�[�������q�m�[�h)
		if (_curBlendDepth < START_BLEND_DEPTH)
		{
			// �u�����h�[���ȓ��Ȃ�
			if (_curBlendDepth <= blendDepth)
			{
				// �x�[�X���̃A�j���[�V�����g�����X�t�H�[��
				BoneTransform baseAnimTransform;
				bool hasAnimation = pBaseAnimation->GetTransformByRatio(uBoneId, _baseRatio, baseAnimTransform);
				if (!hasAnimation)	// �����p���A�j���[�V����
				{
					baseAnimTransform = bone.GetLocalNodeTransform();
				}

				// �u�����h�����v�Z����
				BoneTransform blendAnimTransform;
				hasAnimation = pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, blendAnimTransform);
				if (!hasAnimation)	// �����p���A�j���[�V����
				{
					blendAnimTransform = bone.GetLocalNodeTransform();
				}

				// �u�����h���銄�������߂�(�[��0�Ȃ�1)
				float blendFactor = 1.0f / blendDepth * _curBlendDepth;

				// �x�[�X�ƃu�����h�̃��b�V����Ԃ̉�]�ʂ����߂�
				baseMeshSpaceRotation = Quat::Multiply(baseAnimTransform.rotation, _parenBaseRotation);
				blendMeshSpaceRotation = Quat::Multiply(blendAnimTransform.rotation, _parentBlendRotation);

				// �u�����h�ƃx�[�X���Ńu�����h���s��(���b�V�����)
				calcMeshSpaceRotation = DXSimp::Quaternion::Slerp(baseMeshSpaceRotation, blendMeshSpaceRotation, blendFactor);

				// ���[�J����Ԃɖ߂�
				_outTransforms[uBoneId].rotation = Quat::RotationDifference(calcMeshSpaceRotation, _parentCalculateRotation);

				// �g�����X�t�H�[���ƁA�X�P�[�����u�����h(���[�J�����)
				_outTransforms[uBoneId].position =
					DXSimp::Vector3::Lerp(baseAnimTransform.position, blendAnimTransform.position, blendFactor);
				_outTransforms[uBoneId].scale =
					DXSimp::Vector3::Lerp(baseAnimTransform.scale, blendAnimTransform.scale, blendFactor);
			}
			else // �u�����h���̃A�j���[�V�����Ő[���͈̔͊O
			{
				// �u�����h�A�j���[�V�������擾
				BoneTransform blendAnimTransform;
				if (!pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, blendAnimTransform))
				{
					// �A�j���[�V�����Ȃ��Ȃ珉���p��
					blendAnimTransform = bone.GetLocalNodeTransform();
				}

				// ���b�V����Ԃ̉�]��
				blendMeshSpaceRotation = Quat::Multiply(blendAnimTransform.rotation, _parentBlendRotation);

				// �e�{�[���Ƃ̃��[�J����Ԃɖ߂�
				blendAnimTransform.rotation = Quat::RotationDifference(blendMeshSpaceRotation, _parentCalculateRotation);

				// ���ʂ���
				_outTransforms[uBoneId] = blendAnimTransform;

				// �v�Z�������b�V����Ԃ̉�]�ʂ����߂�(����ׂ̈�)
				calcMeshSpaceRotation = blendMeshSpaceRotation;
			}
		}
		else // �x�[�X���̃A�j���[�V����
		{
			pBaseAnimation->GetTransformByRatio(uBoneId, _baseRatio, _outTransforms[uBoneId]);

			// ���̂��߂Ɍv�Z���Ă���
			baseMeshSpaceRotation = Quat::Multiply(_outTransforms[uBoneId].rotation, _parenBaseRotation);

			// �u�����h�����v�Z����
			DXSimp::Quaternion blendRotation;
			pBlendAnimation->GetQuaternionByRatio(uBoneId, _blendRatio, blendRotation);
			blendMeshSpaceRotation = Quat::Multiply(blendRotation, _parentBlendRotation);

			// �x�[�X���𔽉f���Ă���̂Ńx�[�X�Ɠ���
			calcMeshSpaceRotation = baseMeshSpaceRotation;
		}

		// �q�m�[�h�ɓn��
		u_int childCnt = _pCurrentNode->GetChildNum();
		for (u_int c_i = 0; c_i < childCnt; c_i++)
			GetMeshSpaceBlendTransform(_outTransforms,
				_pCurrentNode->GetChild(c_i),
				baseMeshSpaceRotation,
				blendMeshSpaceRotation,
				calcMeshSpaceRotation,
				_baseRatio,
				_blendRatio,
				_curBlendDepth + 1	// �[�x�𑝂₵�Ă���
			);
	}

	void LayerdAnimationNode::GetLocalSpaceBlendTransform(std::vector<BoneTransform>& _outTransforms,
		float _baseRatio,
		float _blendRatio) const
	{
		// �x�[�X�̃{�[���O���[�v�ɂ̂݃A�j���[�V�����𔽉f������
		// �i�x�[�X���A�j���[�V�����j
		u_int baseBoneCnt = static_cast<u_int>(baseAnimBoneIds.size());
		for (u_int b_i = 0; b_i < baseBoneCnt; b_i++)
		{
			u_int uBoneId = static_cast<u_int>(baseAnimBoneIds[b_i]);
			pBaseAnimation->GetTransformByRatio(uBoneId, 
				_baseRatio,
				_outTransforms[uBoneId]);
		}

		// �u�����h�̃{�[���O���[�v�ɂ̂݃A�j���[�V�����𔽉f������
		// �i�u�����h���A�j���[�V�����j
		u_int blendBoneCnt = static_cast<u_int>(blendAnimBoneIds.size());
		for (u_int b_i = 0; b_i < blendBoneCnt; b_i++)
		{
			u_int uBoneId = static_cast<u_int>(blendAnimBoneIds[b_i]);
			pBlendAnimation->GetTransformByRatio(uBoneId, _blendRatio, _outTransforms[uBoneId]);
		}
	}

	u_int LayerdAnimationNode::GetAllKeyFrame() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 1;
#endif // EDIT

		return pBlendAnimation->GetAllAnimationFrame();
	}

	u_int LayerdAnimationNode::GetBlendAllKeyFrame() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 1;
#endif // EDIT

		return pBlendAnimation->GetAllAnimationFrame();
	}

	float LayerdAnimationNode::GetBlendCurveSpeed(float _playRatio) const
	{
		return blendSpeedCurve.GetValue(_playRatio);
	}

	float LayerdAnimationNode::GetBlendAnimationTime() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 1.0f;
#endif // EDIT

		return pBlendAnimation->GetAnimationTime();
	}

	float LayerdAnimationNode::GetBeginBlendPlayRatio() const
	{
#ifdef EDIT
		if (!pBlendAnimation) return 0.0f;
#endif // EDIT

		return static_cast<float>(beginBlendPlayFrame) / pBlendAnimation->GetAllAnimationFrame();
	}

	int LayerdAnimationNode::GetBeginBlendBoneId() const
	{
		return beginBlendBoneId;
	}

	DXSimp::Vector3 LayerdAnimationNode::GetRootMotionPos(float _ratio) const
	{
#ifdef EDIT
		if (!pBaseAnimation) return  DXSimp::Vector3::Zero;
#endif // EDIT

		return pBaseAnimation->GetRootMotionPos(_ratio);
	}

	const DXSimp::Vector3& LayerdAnimationNode::GetRootMotionPerSpeed() const
	{
#ifdef EDIT
		if (!pBaseAnimation) return  DXSimp::Vector3::Zero;
#endif // EDIT

		return pBaseAnimation->GetRootMotionPosSpeedPerSec();
	}

	bool LayerdAnimationNode::GetMotionSpaceRotationBlend() const
	{
		return isMeshSpaceRotationBlend;
	}

	json LayerdAnimationNode::Save()
	{
		auto data = AnimationNode_Base::Save();

		data["blendBeginFrame"] = beginBlendPlayFrame;
		data["blendCurve"] = blendSpeedCurve.Save();

		if (pBaseAnimation)
			data["baseAnim"] = pBaseAnimation->GetAssetName();
		if (pBlendAnimation)
			data["blendAnim"] = pBlendAnimation->GetAssetName();

		data["beginBoneId"] = beginBlendBoneId;
		data["beginBlendFlame"] = beginBlendPlayFrame;
		data["meshSpaceBlend"] = isMeshSpaceRotationBlend;
		data["blendDepth"] = blendDepth;

		return data;
	}

	void LayerdAnimationNode::Load(const json& _data)
	{
		AnimationNode_Base::Load(_data);

		LoadJsonUnsigned("beginBlendFlame", beginBlendPlayFrame, _data);
		LoadJsonBoolean("meshSpaceBlend", isMeshSpaceRotationBlend, _data);
		LoadJsonUnsigned("blendDepth", blendDepth, _data);
		json blendCurveData;
		if (LoadJsonData("blendCurve", blendCurveData, _data))
		{
			blendSpeedCurve.Load(blendCurveData);
		}

		std::string loadStr;
		if (LoadJsonString("baseAnim", loadStr, _data))
		{
			pBaseAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
			if (pBaseAnimation)
				SetAnimationTime(pBaseAnimation->GetAnimationTime());
		}
		if (LoadJsonString("blendAnim", loadStr, _data))
		{
			pBlendAnimation = AssetGetter::GetAsset<AnimationData>(loadStr);
			beginBlendPlayFrame = 0;
		}
		if (LoadJsonInteger("beginBoneId", beginBlendBoneId, _data))
		{
			SetBlendBeginBone(beginBlendBoneId);
		}
	}

	bool LayerdAnimationNode::CanWarking() const
	{
#ifdef EDIT
		// ��b�ƂȂ�A�j���[�V�������Ȃ�������@�\���Ȃ�
		if (!pBaseAnimation)
		{
			assert(!"��b�A�j���[�V�������Z�b�g���Ă�");
			return false;
		}

		if (!pBlendAnimation)
		{
			assert(!"�u�����h�A�j���[�V�������Z�b�g���Ă�");
			return false;
		}
#endif // EDIT

		return true;
	}

	void LayerdAnimationNode::SetBlendBeginBone(int _beginBoneId)
	{
		// �{�[���̐����傫���@�܂��́@0�ȉ��Ȃ珈�����Ȃ�
		if (_beginBoneId >= static_cast<int>(pBoneList->GetBoneCnt()) ||
			_beginBoneId < 0) return;

		beginBlendBoneId = _beginBoneId;

		// �x�[�X���ƃu�����h���Ń{�[���𕪂��Ă���
		baseAnimBoneIds.clear();
		blendAnimBoneIds.clear();
		SeparateBoneGroup(*pBoneList->GetRootNode());
	}

	void LayerdAnimationNode::SeparateBoneGroup(const TreeNode& _treeNode)
	{
		// �c���[�m�[�h�ɑΉ������{�[�������邩�m�F
		if (_treeNode.HasBone())
		{
			// �{�[���̃C���f�b�N�X���擾
			u_int boneId = _treeNode.GetBoneIdx();

			if (boneId == beginBlendBoneId) // �u�����h�J�n�̃{�[����������
			{
				// �u�����h�O���[�v���쐬����
				CreateBlendBoneGroup(_treeNode);
				return; // �ȍ~�̃{�[���͏������Ȃ�
			}

			baseAnimBoneIds.push_back(boneId);	// �x�[�X���ɒǉ�
		}

		u_int childCnt = _treeNode.GetChildNum();
		// �ċA�ŕ����Ă���
		for (u_int c_i = 0; c_i < childCnt; c_i++)
			SeparateBoneGroup(*_treeNode.GetChild(c_i));
	}

	void LayerdAnimationNode::CreateBlendBoneGroup(const TreeNode& _blendBoneNode)
	{
		if (_blendBoneNode.HasBone())
			blendAnimBoneIds.push_back(_blendBoneNode.GetBoneIdx());	// �u�����h�ɒǉ�

		// �ċA�ŕ����Ă���
		u_int childCnt = _blendBoneNode.GetChildNum();
		for (u_int c_i = 0; c_i < childCnt; c_i++)
			CreateBlendBoneGroup(*_blendBoneNode.GetChild(c_i));
	}

	void LayerdAnimationNode::ImGuiDebug()
	{
#ifdef EDIT
		AnimationNode_Base::ImGuiDebug();

		ImGui::Checkbox("MeshSpaceBlend", &isMeshSpaceRotationBlend);

		// �u�����h�J�n����{�[����Id���Z�b�g
		if (ImGui::DragInt("BlendBeginBoneId", &beginBlendBoneId, 1, 0, (int)pBoneList->GetBoneCnt()))
		{
			SetBlendBeginBone(beginBlendBoneId);
		}

		// �u�����h����[��
		ImGui::DragScalar("BlendDepth", ImGuiDataType_U32, &blendDepth);

		// �u�����h���̊J�n�t���[��
		u_int maxFrame = 0;
		if (pBlendAnimation) maxFrame = pBlendAnimation->GetAllAnimationFrame();
		ImGui::DragScalar("BlendBeginFrame", ImGuiDataType_U32, &beginBlendPlayFrame, 1, 0, &maxFrame);

		// �u�����h���̑��x�J�[�u
		blendSpeedCurve.ImGuiCall();

		// �A�j���[�V�������Z�b�g����
		static std::string str;
		AssetGetter::ImGuiGetCombobox<AnimationData>("Animation", str);
		if (ImGui::Button("Set Base"))
		{
			AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
			if (pGetData)
			{
				pBaseAnimation = pGetData;
				SetAnimationTime(pBaseAnimation->GetAnimationTime());
			}
		}
		ImGui::SameLine();

		// �A�j���[�V�������\��
		std::string animName = "Null";
		if (pBaseAnimation)
			animName = pBaseAnimation->GetAssetName();
		ImGui::Text(animName.c_str());

		if (ImGui::Button("Set Blend"))
		{
			AnimationData* pGetData = AssetGetter::GetAsset<AnimationData>(str);
			if (pGetData) pBlendAnimation = pGetData;
		}
		ImGui::SameLine();
		// �A�j���[�V�������\��
		animName = "Null";
		if (pBlendAnimation)
			animName = pBlendAnimation->GetAssetName();
		ImGui::Text(animName.c_str());
#endif // EDIT
	}
}