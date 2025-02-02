#include "pch.h"
#include "CP_StylishUI.h"
#include "GameObject.h"
#include "CP_UISlider.h"
#include "InSceneSystemManager.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	/// @brief ���E�̃X�^�C���b�V���|�C���g
	constexpr float MAX_LIMIT_STYLISHPOINT(5000);

	CP_StylishUI::CP_StylishUI() :
		pStylishRankRenderer(nullptr),
		pStylishGage(nullptr),
		pGageGroupObj(nullptr),
		curStylishRank(StylishRank::SSS),
		curStylishPoint(0),
		decadeBeginTime(7.0f),
		elapsedToDecadeTime(0.0f),
		rankupEase(EaseKind::Linear),
		curRankupDirState(RankupDirectionState::Wait),
		rankupScalingTime(1.0f),
		rankupScalingRatio(2.0f),
		elapsedRankupDirectionTime(0.0f)
	{
	}

	void CP_StylishUI::AddStylishPoint(float _addPoint)
	{
		// ���Z
		curStylishPoint += _addPoint;
		curStylishPoint = std::min(curStylishPoint, MAX_LIMIT_STYLISHPOINT);

		// �������ԃ��Z�b�g
		elapsedToDecadeTime = 0.0f;

		// ���̃����N�̏����ɒB�������H
		StylishRank nextRank = GetNextRank(curStylishRank);
		if (curStylishPoint >= GetEnterRankPoint(nextRank))
		{
			// �B�����玟�̃����N��
			ChangeRank(GetCurrentRankFromPoint());
		}

		// �X���C�_�[�ɔ��f
		ApplyStylishGage();
	}

	void CP_StylishUI::DecadeStylishPoint(float _decadePoint)
	{
		// ���Z
		curStylishPoint -= _decadePoint;
		curStylishPoint = std::max(curStylishPoint, 0.0f);

		// ���̃����N�̃|�C���g��艺��������H
		if (curStylishPoint < GetEnterRankPoint(curStylishRank))
		{
			// �O�̃����N��
			ChangeRank(GetCurrentRankFromPoint());
		}

		// �X���C�_�[�ɔ��f
		ApplyStylishGage();
	}

	json CP_StylishUI::Save()
	{
		json data = Component::Save();

		data["decadeBeginTime"] = decadeBeginTime;
		data["gageObjName"] = stylishGageName;
		data["gageGroupObjName"] = gageGroupObjName;
		data["rankParameters"] = SaveRankParameters();
		data["rankupShake"] = rankupShakeParam.Save();
		data["rankupScalingRatio"] = rankupScalingRatio;
		data["rankupEase"] = rankupEase;
		data["rankupScalingTime"] = rankupScalingTime;
		return data;
	}

	json CP_StylishUI::SaveRankParameters()
	{
		json rankParamDataList;

		// �e�����N���Ƃ̃p�����[�^���Z�[�u
		u_int rankCnt = static_cast<u_int>(StylishRank::MaxNum);
		for (u_int r_i = 0; r_i < rankCnt; r_i++)
		{
			json rankParamData;
			RankParameter& rankParam = rankParameters[r_i];

			// �p�����[�^
			if (rankParam.displayTexture)
			{
				rankParamData["texName"] = rankParam.displayTexture->GetAssetName();
			}
			rankParamData["enterPoint"] = rankParam.requireEnterPoints;
			rankParamData["decadeSpeed"] = rankParam.decadePointSpeed;

			// �ǉ�
			rankParamDataList.push_back(rankParamData);
		}

		return rankParamDataList;
	}

	void CP_StylishUI::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonFloat("decadeBeginTime", decadeBeginTime, _data);
		LoadJsonString("gageObjName", stylishGageName, _data);
		LoadJsonString("gageGroupObjName", gageGroupObjName, _data);
		LoadJsonFloat("rankupScalingRatio", rankupScalingRatio, _data);
		LoadJsonEnum<EaseKind>("rankupEase", rankupEase, _data);
		LoadJsonFloat("rankupScalingTime", rankupScalingTime, _data);
		// �����N�p�����[�^
		if (IsJsonContains(_data, "rankParameters"))
		{
			LoadRankParameters(_data["rankParameters"]);
		}
		if (IsJsonContains(_data, "rankupShake"))
		{
			rankupShakeParam.Load(_data["rankupShake"]);
		}

	}

	void CP_StylishUI::LoadRankParameters(const json& _rankParamDataList)
	{
		// �e�����N���Ƃ̃p�����[�^���Z�[�u
		u_int rankCnt = static_cast<u_int>(StylishRank::MaxNum);
		for (u_int r_i = 0; r_i < rankCnt; r_i++)
		{
			const json& rankParamData = _rankParamDataList[r_i];
			RankParameter& rankParam = rankParameters[r_i];

			// �p�����[�^
			std::string texName;
			if (LoadJsonString("texName", texName, rankParamData))
			{
				rankParam.displayTexture = AssetGetter::GetAsset<Texture>(texName);
			}
			LoadJsonFloat("enterPoint", rankParam.requireEnterPoints, rankParamData);
			LoadJsonFloat("decadeSpeed", rankParam.decadePointSpeed, rankParamData);
		}
	}

	void CP_StylishUI::Start()
	{
		pRankupShakeVector = std::make_unique<PerlinShake>();

		// �����N�オ�����Ƃ��̃p�����[�^������
		InitRankupParam();

		// �I�u�W�F�N�g���擾
		FindRequireObject();

		// �����N�Ȃ�����n�܂�
		ChangeRank(StylishRank::None);
	}

	void CP_StylishUI::Update()
	{
		// �����ł��邩�m�F
		if (!GetCanActive()) return;

		// �|�C���g�����炷
		DecadePointUpdate();

		// ���o�̍X�V����
		RankupDirectionUpdate();
	}

	void CP_StylishUI::InitRankupParam()
	{
		// �I�u�W�F�N�g�̏������W�ƃX�P�[������
		Transform& transform = GetTransform();
		rankUIBasePos = transform.GetPosition();
		rankUIBaseScale = transform.GetScale();
	}

	void CP_StylishUI::FindRequireObject()
	{
		// �擾
		pStylishRankRenderer = GetGameObject().GetComponent<CP_UIRenderer>();
		if (!pStylishRankRenderer)
			HASHI_DEBUG_LOG("UI�`�悪����܂���");

		// �Q�[�W�擾
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* pObj = sceneObjs.GetSceneObject(stylishGageName);
		if (pObj)
		{
			pStylishGage = pObj->GetComponent<CP_UISlider>();
			if (!pStylishGage)
				HASHI_DEBUG_LOG("UISlider������܂���");
		}
		ApplyStylishGage();

		// �Q�[�W�O���[�v
		pGageGroupObj = sceneObjs.GetSceneObject(gageGroupObjName);
		if (!pGageGroupObj)
			HASHI_DEBUG_LOG("�Q�[�W�O���[�v������܂���");

	}

	void CP_StylishUI::DecadePointUpdate()
	{
		// ���鎞�Ԃ��߂����Ȃ�
		if (elapsedToDecadeTime > decadeBeginTime)
		{
			// ���t���[���Ō��炷�|�C���g���v�Z����
			float decadePoint = rankParameters[static_cast<u_int>(curStylishRank)].decadePointSpeed *
				DeltaTime();

			// ���炷
			DecadeStylishPoint(decadePoint);
		}
		else // ������Ă���Ȃ�
		{
			// ���Z����
			elapsedToDecadeTime += DeltaTime();
		}
	}

	void CP_StylishUI::RankupDirectionUpdate()
	{
		switch (curRankupDirState)
		{
		case RankupDirectionState::Scaling:
			RankupScalingUpdate();
			break;

		case RankupDirectionState::Shaking:
			RankupShakeUpdate();
			break;

		default:
			break;
		}

	}

	void CP_StylishUI::RankupScalingUpdate()
	{
		if (!pStylishRankRenderer) return;

		// ���Ԃ��o��
		elapsedRankupDirectionTime += DeltaTime();

		// 0���Z�h�~
		float scaleTime = std::max(rankupScalingTime, Mathf::epsilon);
		// ���Ԃ̊���
		float timeRatio = elapsedRankupDirectionTime / scaleTime;
		timeRatio = Easing::EaseValue(timeRatio, rankupEase);

		// �X�P�[�����O
		DXSimp::Vector3 beginScale = rankUIBaseScale * rankupScalingRatio;
		beginScale.z = rankUIBaseScale.z;
		DXSimp::Vector3 curScale = DXSimp::Vector3::Lerp(beginScale,
			rankUIBaseScale,
			timeRatio);
		pStylishRankRenderer->GetTransform().SetScale(curScale);

		// ���l�ړ�
		float curAlpha = Mathf::Lerp(0.0f, 1.0f, timeRatio);
		pStylishRankRenderer->SetAlpha(curAlpha);

		// ���o���Ԃ��߂����玟�̃X�e�b�v��
		if (elapsedRankupDirectionTime > rankupScalingTime)
		{
			curRankupDirState = RankupDirectionState::Shaking;

			// �V�F�C�N�J�n
			pRankupShakeVector->BeginShake(rankupShakeParam);
		}
	}

	void CP_StylishUI::RankupShakeUpdate()
	{
		if (!pStylishRankRenderer) return;

		Transform& transform = pStylishRankRenderer->GetTransform();

		// �I�t�Z�b�g�v�Z
		pRankupShakeVector->Update(DeltaTime());

		// ���W���f
		transform.SetPosition(rankUIBasePos + pRankupShakeVector->GetShakeOffset());

		// �I������牉�o�I��
		if (!pRankupShakeVector->GetIsShaking())
		{
			curRankupDirState = RankupDirectionState::Wait;
		}
	}

	bool CP_StylishUI::GetCanActive() const
	{
		if (!pStylishRankRenderer) return false;
		if (!pStylishGage) return false;

		return true;
	}

	float CP_StylishUI::GetEnterRankPoint(StylishRank _checkRank) const
	{
		return rankParameters[static_cast<u_int>(_checkRank)].requireEnterPoints;
	}

	void CP_StylishUI::ChangeRank(StylishRank _targetRank)
	{
		// ���݂̃����N�Ɠ����Ȃ珈�����Ȃ�
		if (curStylishRank == _targetRank) return;

		// �ύX�O�̃����N�擾
		StylishRank prevRank = curStylishRank;

		// �����N�ύX
		curStylishRank = _targetRank;

		if (pStylishRankRenderer)
		{
			// �����N�̉摜��ς���
			u_int curRankId = static_cast<u_int>(curStylishRank);
			pStylishRankRenderer->SetTexture(rankParameters[curRankId].displayTexture);

			// �O��̃����N���オ���Ă����Ȃ�
			u_int prevRankId = static_cast<u_int>(prevRank);
			if (curRankId > prevRankId)
			{
				// ���o���n�߂�
				BeginRankUpDirection();
			}
		}

		// �����N�Ȃ��Ȃ�Q�[�W������
		if (_targetRank == StylishRank::None)
		{
			if (pGageGroupObj)
				pGageGroupObj->SetActive(false);
		}
		else if (prevRank == StylishRank::None) // �O��̃����N���Ȃ�����Ȃ�
		{
			if (pGageGroupObj)
				pGageGroupObj->SetActive(true);	// �Q�[�W�O���[�v��\��
		}
	}

	void CP_StylishUI::ApplyStylishGage()
	{
		if (!pStylishGage) return;

		// ���̃����N�ɓ���|�C���g���擾
		float nextEnterPoint = MAX_LIMIT_STYLISHPOINT;

		// �ő僉���N�łȂ��Ȃ�
		if (!GetIsCurMaxRank())
		{
			// ���̃����N�̃|�C���g���擾����
			StylishRank nextRank = GetNextRank(curStylishRank);
			nextEnterPoint = GetEnterRankPoint(nextRank);
		}

		// ���݂̃|�C���g�����܂ł̂ǂ̒��x�̊����Ȃ̂����߂�
		float curEnterPoint = GetEnterRankPoint(curStylishRank);
		float diffCurPoint = curStylishPoint - curEnterPoint;
		float diffNextEnterPoint = nextEnterPoint - curEnterPoint;
		diffNextEnterPoint = std::max(diffNextEnterPoint, Mathf::epsilon);

		// �X���C�_�[�������ŃZ�b�g����
		pStylishGage->SetCurrentRatio(diffCurPoint / diffNextEnterPoint);
	}

	void CP_StylishUI::BeginRankUpDirection()
	{
		// �����N�̕����̉��o������
		if (!pStylishRankRenderer) return;

		Transform& rankUiTransform = GetTransform();

		// �����N��UI�����o�̏�����Ԃɂ���
		elapsedRankupDirectionTime = 0.0f;
		curRankupDirState = RankupDirectionState::Scaling;

		// �g�債����Ԃ���
		DXSimp::Vector3 beginRankUIScale = rankUIBaseScale * rankupScalingRatio;
		beginRankUIScale.z = rankUIBaseScale.z;

		// ��������
		pStylishRankRenderer->SetAlpha(0.0f);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetNextRank(StylishRank _targetRank) const
	{
		// ��ԏ�̃����N�Ȃ�N�����Ȃ�
		if (_targetRank == StylishRank::SSS) return StylishRank::SSS;

		u_int nextRankId = static_cast<u_int>(curStylishRank) + 1;
		return static_cast<StylishRank>(nextRankId);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetPrevRank(StylishRank _targetRank) const
	{
		// ��ԏ�̃����N�Ȃ�N�����Ȃ�
		if (_targetRank == StylishRank::None) return StylishRank::None;

		u_int prevRankId = static_cast<u_int>(curStylishRank) - 1;
		return static_cast<StylishRank>(prevRankId);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetCurrentRankFromPoint()
	{
		int rankCnt = static_cast<u_int>(StylishRank::MaxNum);

		// ��̃����N���猩�Ă����A���݂̃|�C���g���K�v�ȃ|�C���g��菉�߂ď�ɂȂ郉���N�����݂̃����N
		for (int r_i = rankCnt - 1; r_i >= 0; r_i--)
		{
			if (curStylishPoint < rankParameters[r_i].requireEnterPoints)
				continue;

			return static_cast<StylishRank>(r_i);
		}

		HASHI_DEBUG_LOG("����Ƀ����N���擾�ł��܂���ł���");
		return StylishRank::None;
	}

	bool CP_StylishUI::GetIsCurMaxRank() const
	{
		return curStylishRank == StylishRank::SSS;
	}

	void CP_StylishUI::ImGuiDebug()
	{
#ifdef EDIT
		Component::ImGuiDebug();

		// ���݂̏��
		ImGui::Text(std::string(magic_enum::enum_name(curStylishRank)).c_str());	// �����N
		ImGui::Text("curPoint:%f", curStylishPoint);	// ���݂̃|�C���g
		ImGui::Text("nextPoint:%f", GetEnterRankPoint(GetNextRank(curStylishRank)));	// ���̃|�C���g
		ImGui::Text("elapsed:%f", elapsedToDecadeTime);	// ���݂̌o��
		ImGui::DragFloat("decadeBeginTime", &decadeBeginTime, 0.1f, 0.0f, 100.0f);

		// �����N�オ�����Ƃ��̉��o
		ImGuiRankupDirection();

		// �����N�p�����[�^
		ImGuiRankParameter();

		// �I�u�W�F�N�g���Z�b�g
		ImGuiSetObject();

#endif // EDIT
	}

	void CP_StylishUI::ImGuiRankupDirection()
	{
		if (!ImGuiMethod::TreeNode("Rankup Directio")) return;

		Easing::ImGuiSelect(rankupEase, "Rankup Ease");
		ImGui::DragFloat("Scaling Time", &rankupScalingTime, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Scaling Ratio", &rankupScalingRatio, 0.01f, 1.0f, 100.0f);

		rankupShakeParam.ImGuiCall();

		ImGui::TreePop();
	}

	void CP_StylishUI::ImGuiRankParameter()
	{
#ifdef EDIT
		u_int rankCnt = static_cast<u_int>(StylishRank::MaxNum);
		for (u_int r_i = 1; r_i < rankCnt; r_i++)
		{
			// �����N�����擾����
			StylishRank rank = static_cast<StylishRank>(r_i);
			std::string rankName = std::string(magic_enum::enum_name(rank));

			if (!ImGuiMethod::TreeNode(rankName)) continue;
			RankParameter& rankParam = rankParameters[r_i];

			// �����N�̃e�N�X�`��
			std::string texName;
			if (rankParam.displayTexture)
				texName = rankParam.displayTexture->GetAssetName();
			if (AssetGetter::ImGuiGetCombobox<Texture>("Texture", texName))
			{
				rankParam.displayTexture = AssetGetter::GetAsset<Texture>(texName);
			}

			ImGui::DragFloat("EnterPoint", &rankParam.requireEnterPoints, 0.1f, 0.0f, 10000.0f);	// �����N�̏���
			ImGui::DragFloat("DecadeSpeed", &rankParam.decadePointSpeed, 0.1f, 0.0f, 10000.0f);	// ���鑬�x

			ImGui::TreePop();
		}
#endif // EDIT
	}

	void CP_StylishUI::ImGuiSetObject()
	{
#ifdef EDIT
		// ����
		static char input[IM_INPUT_BUF] = "\0";
		ImGui::InputText("##Obj", input, IM_INPUT_BUF);

		// �Z�b�g
		if (ImGui::Button("Set Gage"))
		{
			stylishGageName = input;
		}
		ImGui::SameLine();
		ImGui::Text(stylishGageName.c_str());

		if (ImGui::Button("Set GageGroup"))
		{
			gageGroupObjName = input;
		}
		ImGui::SameLine();
		ImGui::Text(gageGroupObjName.c_str());
#endif // EDIT
	}
}
