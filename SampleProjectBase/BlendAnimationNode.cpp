#include "pch.h"
#include "BlendAnimationNode.h"
#include "AnimationParameters.h"

#include "AssetGetter.h"
#include "SkeletalMesh.h"

namespace HashiTaku
{
	constexpr float DUPLICATE_PLUS(0.01f);	// �u�����h�l���d�������Ƃ��́{�l

	constexpr short X_AXIS_IDX(0);	// X��������킷�̃C���f�b�N�X�ԍ�
	constexpr short Y_AXIS_IDX(1);	// Y��������킷�̃C���f�b�N�X�ԍ�
	constexpr short TWOAXIS_MAXBLEND_NUM(4);	// 2���ł̃u�����h������Ƃ��̍ő�̃A�j���[�V������

	BlendAnimationNode::BlendAnimationNode(const AnimationParameters& _animParams,
		const std::string& _nodeName)
		: AnimationNode_Base(_nodeName, NodeType::Blend), pAnimParameters(&_animParams), isUseBlend(true)
	{
		SetAnimationTime(1.0f);
	}

	void BlendAnimationNode::AddAnimationData(const std::string& _animName)
	{
		AnimPointData createData;

		AnimationData* pData = AssetGetter::GetAsset<AnimationData>(_animName);
		if (!pData) return;

		createData.pAnimation = pData;
		animationPoints.push_back(createData);
	}

	const std::string& BlendAnimationNode::GetTargetParamName(u_int _axisIdx) const
	{
		assert(GetAxisCnt() > _axisIdx && "���ԍ����傫�����܂�");

		return axisParameters[_axisIdx].targetParamName;
	}

	json BlendAnimationNode::Save()
	{
		auto data = AnimationNode_Base::Save();

		data["useBlend"] = isUseBlend;

		// ���̃p�����[�^��ۑ�
		for (auto& a : axisParameters)
			data["axisParam"].push_back(SaveAxisParameter(a));

		// �e�A�j���[�V�������Z�[�u
		for (auto& b : animationPoints)
			data["blendPoint"].push_back(SaveBlendPoint(b));

		return data;
	}

	void BlendAnimationNode::Load(const json& _data)
	{
		AnimationNode_Base::Load(_data);

		LoadJsonBoolean("useBlend", isUseBlend, _data);

		json blendSaveDatas;
		if (LoadJsonDataArray("axisParam", blendSaveDatas, _data))
			for (auto& d : blendSaveDatas)	// ���p�����[�^�����[�h����
				LoadAxisParameter(d);

		if (LoadJsonDataArray("blendPoint", blendSaveDatas, _data))
			for (auto& d : blendSaveDatas)	// �u�����h�|�C���g�����[�h����
				LoadBlendPoint(d);
	}

	bool BlendAnimationNode::CompareBlendDistance(const PointToDistance& _a, const PointToDistance& _b)
	{
		return _a.distance < _b.distance;
	}

	bool BlendAnimationNode::CompareBlendvalue(const AnimPointData& _a, const AnimPointData& _b)
	{
		return _a.blendValue.x < _b.blendValue.x;
	}


	void BlendAnimationNode::FindBlendingDataSingleAxis(float _blendValue, std::vector<BlendAnimationNode::BlendingData>& _outData) const
	{
		BlendingData blendingData;

		// �ȉ��̏����̂Ƃ��A�l������A�j���[�V�����͈����
		auto minItr = animationPoints.begin();
		auto maxItr = std::prev(animationPoints.end());

		if (_blendValue <= minItr->blendValue.x) // �ŏ��l��菬����
		{
			blendingData.pAnimation = (*minItr).pAnimation;
			blendingData.blendWeight = 1.0f;
			_outData.push_back(blendingData);

			return;
		}
		else if (_blendValue >= maxItr->blendValue.x) // �ő�l���傫��
		{
			blendingData.pAnimation = (*maxItr).pAnimation;
			blendingData.blendWeight = 1.0f;
			_outData.push_back(blendingData);

			return;
		}


		// ���݂̊������傫���������f�[�^�̒�����T��
		for (auto prevItr = animationPoints.begin(); prevItr != std::prev(animationPoints.end()); prevItr++)
		{
			auto nextItr = std::next(prevItr);
			if (_blendValue > nextItr->blendValue.x) continue;
			// ��������

			//�@�O
			blendingData.pAnimation = (*prevItr).pAnimation;
			// �u�����h�l�̊��������߂�
			float blendDistance = (*nextItr).blendValue.x - (*prevItr).blendValue.x;
			float prevBlendRatio = (_blendValue - (*prevItr).blendValue.x) / blendDistance;
			blendingData.blendWeight = 1.0f - prevBlendRatio;
			_outData.push_back(blendingData);

			// ��
			blendingData.pAnimation = (*nextItr).pAnimation;
			blendingData.blendWeight = prevBlendRatio;
			_outData.push_back(blendingData);

			break;
		}
	}

	void BlendAnimationNode::FindBlendPairTwoAxis(DXSimp::Vector2 _blendValues, std::vector<BlendingData>& _outData) const
	{
		// ����4�_�ȉ��͏������Ȃ�
		if (GetAnimPointCnt() < TWOAXIS_MAXBLEND_NUM) return;

		using namespace DXSimp;
		std::list<PointToDistance> pointData;

		//�@�|�C���g�Ƃ̋��������߂�
		for (auto& p : animationPoints)
		{
			PointToDistance pd;
			pd.point = &p;
			pd.distance = Vector2::Distance(_blendValues, p.blendValue);
			if (pd.distance < Mathf::epsilon)
			{
				pd.distance = Mathf::epsilon;
			}

			if (pd.distance <= Mathf::epsilon)	// ������0.0�Ȃ�
			{
				BlendingData bd;
				bd.pAnimation = p.pAnimation;
				bd.blendWeight = 1.0f;
				_outData.push_back(bd);
				return;
			}

			pointData.push_back(std::move(pd));
		}

		// �����������ɕ��ׂ�
		pointData.sort(CompareBlendDistance);

		auto itr = pointData.begin();
		float sumWeight = 0.0f;	// ���v�̃E�F�C�g
		for (u_int p_i = 0; p_i < TWOAXIS_MAXBLEND_NUM; p_i++)
		{
			/*sumDistance += (*itr).distance;*/

			// �u�����h���郊�X�g�ɒǉ�����
			BlendingData bd;
			bd.pAnimation = (*itr).point->pAnimation;

			// �d�݂����߂�
			bd.blendWeight = 1.0f / (*itr).distance;
			sumWeight += bd.blendWeight;

			_outData.push_back(std::move(bd));

			itr++;
		}

		// �E�F�C�g��0.0�`1.0�ɐ��K������
		for (auto& bd : _outData)
		{
			bd.blendWeight /= sumWeight;
		}

	}

	void BlendAnimationNode::SortBlendPoint()
	{
		animationPoints.sort(CompareBlendvalue);
	}

	void BlendAnimationNode::AddAxisParam()
	{
		if (GetAxisCnt() >= MAX_AXIS_CNT) return;

		// ���𑝂₷
		AxisParameter addAxisParam;
		axisParameters.push_back(addAxisParam);
	}

	void BlendAnimationNode::RemoveAxisParam()
	{
		if (GetAxisCnt() <= MIN_AXIS_CNT) return;

		// �������炷
		axisParameters.pop_back();
	}

	float BlendAnimationNode::GetBlendMoveTime(u_int _axisIdx) const
	{
		assert(GetAxisCnt() > _axisIdx && "���ԍ����傫�����܂�");

		return axisParameters[_axisIdx].blendMoveTime;
	}

	EaseKind BlendAnimationNode::GetBlendMoveEasing(u_int _axisIdx) const
	{
		assert(GetAxisCnt() > _axisIdx && "���ԍ����傫�����܂�");

		return axisParameters[_axisIdx].blendMoveEase;
	}

	void BlendAnimationNode::CalcRootMotionSpeed(const std::vector<BlendingData>& _blendDatas, DXSimp::Vector3& _outPos) const
	{
		using namespace DXSimp;
		_outPos = Vector3::Zero;
		for (auto& anim : _blendDatas)
		{
			_outPos += anim.pAnimation->GetRootMotionPosSpeedPerSec() * anim.blendWeight;
		}

	}

	u_int BlendAnimationNode::GetAxisCnt() const
	{
		return static_cast<u_int>(axisParameters.size());
	}

	void BlendAnimationNode::GetRootMotionPos(float _ratio, const std::vector<BlendingData>& _blendDatas, DXSimp::Vector3& _outPos) const
	{
		using namespace DXSimp;
		for (auto& anim : _blendDatas)
		{
			Vector3 rootPos = anim.pAnimation->GetRootMotionPos(_ratio);
			_outPos += rootPos * anim.blendWeight;
		}
	}

	u_int BlendAnimationNode::GetAnimPointCnt() const
	{
		return static_cast<u_int>(animationPoints.size());
	}

	bool BlendAnimationNode::GetIsUseBlend() const
	{
		return isUseBlend;
	}

	void BlendAnimationNode::ImGuiDebug()
	{
		AnimationNode_Base::ImGuiDebug();

		//ImGui::Checkbox("UseBlend", &isUseBlend);

		ImGuiAxisParam();

		ImGuiMethod::LineSpaceSmall();

		ImGuiAnimationInBlend();

		static std::string str;
		AssetGetter::ImGuiGetCombobox<AnimationData>("AddAnimation", str);

		if (ImGui::Button("Add Blend"))
			AddAnimationData(str);
	}

	void BlendAnimationNode::ImGuiAnimationInBlend()
	{
		if (GetAxisCnt() == 0) return;

		for (auto itr = animationPoints.begin(); itr != animationPoints.end();)
		{
			AnimPointData& animPoint = (*itr);
			const std::string& animName = animPoint.pAnimation->GetAssetName();
			bool isDelete = false;

			if (ImGuiMethod::TreeNode(animName.c_str()))
			{
				isDelete = ImGui::Button("X");

				const float speed = 0.01f;
				float min = axisParameters[X_AXIS_IDX].minRangeValue;
				float max = axisParameters[X_AXIS_IDX].maxRangeValue;
				if (ImGui::DragFloat("Blend", &animPoint.blendValue.x, speed, min, max))
				{
					SortBlendPoint();	// ���ԕς���
				}

				if (GetAxisCnt() == 2)	// 2���Ȃ�
				{
					min = axisParameters[Y_AXIS_IDX].minRangeValue;
					max = axisParameters[Y_AXIS_IDX].maxRangeValue;
					ImGui::DragFloat("##BlendY", &animPoint.blendValue.y, speed, min, max);
				}

				ImGui::TreePop();
			}

			if (isDelete)
				itr = animationPoints.erase(itr);
			else
				itr++;
		}
	}

	void BlendAnimationNode::ImGuiAxisParam()
	{
#ifdef EDIT
		for (u_int a_i = 0; a_i < GetAxisCnt(); a_i++)
		{
			const char* treeCaption = "Axis X";
			if (a_i == Y_AXIS_IDX)
				treeCaption = "Axis Y";

			if (ImGuiMethod::TreeNode(treeCaption))
			{
				AxisParameter& axisParam = axisParameters[a_i];

				// �Q�Ƃ���p�����[�^����ύX����
				std::vector<const std::string*> animFloatParams;
				pAnimParameters->GetFloatNameList(animFloatParams);
				ImGuiMethod::ComboBox("Parameter", axisParam.targetParamName, animFloatParams);

				// �u�����h����
				ImGui::DragFloat("BlendMoveTime", &axisParam.blendMoveTime, 0.01f, 0.0f, 100.0f);
				// �u�����h�C�[�W���O
				Easing::ImGuiSelect(axisParam.blendMoveEase);
				ImGui::TreePop();
			}
		}

		// ����ǉ��E�폜
		if (ImGui::Button("+"))
			AddAxisParam();
		ImGui::SameLine();
		if (ImGui::Button("-"))
			RemoveAxisParam();

#endif // EDIT
	}

	void BlendAnimationNode::ImGuiChangeTargetParam()
	{
#ifdef EDIT
		// float�^�ϐ��݂̂̃p�����[�^�����擾
		std::vector<const std::string*> pAnimParams;
		pAnimParameters->GetFloatNameList(pAnimParams);
#endif // EDIT
	}

	json BlendAnimationNode::SaveAxisParameter(const AxisParameter& _axisParam)
	{
		json saveData;

		saveData["name"] = _axisParam.targetParamName;
		saveData["time"] = _axisParam.blendMoveTime;
		saveData["ease"] = _axisParam.blendMoveEase;

		return saveData;
	}

	json BlendAnimationNode::SaveBlendPoint(const AnimPointData& _blendPoint)
	{
		json saveData;
		saveData["animName"] = _blendPoint.pAnimation->GetAssetName();
		SaveJsonVector2("blendValue", _blendPoint.blendValue, saveData);
		return saveData;
	}

	void BlendAnimationNode::LoadAxisParameter(const json& _blendData)
	{
		AxisParameter create;
		LoadJsonString("name", create.targetParamName, _blendData);
		LoadJsonFloat("time", create.blendMoveTime, _blendData);
		LoadJsonEnum<EaseKind>("ease", create.blendMoveEase, _blendData);

		axisParameters.push_back(create);
	}

	void BlendAnimationNode::LoadBlendPoint(const json& _blendData)
	{
		AnimPointData loadBlend;

		loadBlend.pAnimation = LoadJsonAsset<AnimationData>("animName", _blendData);
		if (!loadBlend.pAnimation) return;

		LoadJsonVector2("blendValue", loadBlend.blendValue, _blendData);
		animationPoints.push_back(std::move(loadBlend));
	}
}