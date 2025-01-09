#include "pch.h"
#include "SceneLights.h"

namespace HashiTaku
{
	// GUI�p�����[�^�ύX���x
	constexpr float PARAM_CHANGE_SPEED(0.1f);

	void SceneLights::UpdateParameter()
	{
		// �f�B���N�V�������C�g
		pSceneLightsParam->dirParam = pDirection->GetParameter();

		//// �|�C���g���C�g
		//u_int pointCnt = 0;
		//for (auto& p : pPointLights)
		//{
		//	pSceneLightsParam->pointParams[pointCnt] = p->GetParameter();
		//	pointCnt++;
		//}

		//// �g�p���Ă��Ȃ��z��͌������f����Ȃ��悤�ɂ��Ă���
		//for (; pointCnt < MAX_POINTLIGHT; pointCnt++)
		//{
		//	pSceneLightsParam->pointParams[pointCnt].range = 0.0f;
		//}

		//// �X�|�b�g���C�g
		//u_int spotCnt = 0;
		//for (auto& s : pSpotLights)
		//{
		//	pSceneLightsParam->spotParams[spotCnt] = s->GetParameter();
		//	spotCnt++;
		//}
		//for (;  spotCnt < MAX_SPOTLIGHT;  spotCnt++)
		//{
		//	pSceneLightsParam->spotParams[spotCnt].distance = 0.0f;
		//}
	}

	void SceneLights::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Begin(TO_UTF8("�V�[���@����"));

		ImGuiDirection();
		/*ImGuiPoint();
		ImGuiSpot();*/

		ImGui::End();
#endif // EDIT
	}


	void SceneLights::ImGuiDirection()
	{
#ifdef EDIT
		if (ImGuiMethod::TreeNode(TO_UTF8("�f�B���N�V�������C�g")))
		{
			// �\��
			bool isDisplay = pDirection->GetDisplay();
			ImGui::Checkbox("Display", &isDisplay);
			pDirection->SetDisplay(isDisplay);

			// ���W
			DXSimp::Vector3 p = pDirection->GetPosition();
			ImGuiMethod::DragFloat3(p, "���W", PARAM_CHANGE_SPEED);
			pDirection->SetPosition(p);

			//�F
			DXSimp::Color c = pDirection->GetColor();
			ImGuiMethod::ColorEdit4(c, "�J���[");
			pDirection->SetColor(c);

			// ��
			DXSimp::Color ac = pDirection->GetAmbiemt();
			ImGuiMethod::ColorEdit4(ac, "�A���r�G���g");
			pDirection->SetAmbiemt(ac);

			// ����
			DXSimp::Vector3 d = pDirection->GetDirection();
			ImGuiMethod::DragFloat3(d, "����", PARAM_CHANGE_SPEED);
			pDirection->SetDirection(d);

			ImGui::TreePop();
		}
#endif
	}

//	void SceneLights::ImGuiPoint()
//	{
//#ifdef EDIT
//		if (ImGuiMethod::TreeNode(TO_UTF8("�|�C���g���C�g")))
//		{
//			u_int lightCnt = 1;
//			for (auto& pPointL : pPointLights)	// �V�[�����̃|�C���g���C�g
//			{
//				std::string idxStr = std::to_string(lightCnt);
//
//				if (ImGuiMethod::TreeNode(idxStr.c_str()))
//				{
//					// �\��
//					bool isDisplay = pPointL->GetDisplay();
//					ImGui::Checkbox("Display", &isDisplay);
//					pPointL->SetDisplay(isDisplay);
//
//					// ���W
//					DXSimp::Vector3 p = pPointL->GetPosition();
//					ImGuiMethod::DragFloat3(p, "���W", PARAM_CHANGE_SPEED);
//					pPointL->SetPosition(p);
//
//					// �F
//					DXSimp::Color c = pPointL->GetColor();
//					ImGuiMethod::ColorEdit4(c, "�J���[");
//					pPointL->SetColor(c);
//
//					// �͈�
//					float r = pPointL->GetRange();
//					ImGui::DragFloat(TO_UTF8("�͈�"), &r, PARAM_CHANGE_SPEED);
//					pPointL->SetRange(r);
//				}
//
//				lightCnt++;
//			}
//
//			ImGui::TreePop();
//		}
//#endif
//	}
//
//	void SceneLights::ImGuiSpot()
//	{
//#ifdef EDIT
//		if (ImGuiMethod::TreeNode(TO_UTF8("�X�|�b�g���C�g")))
//		{
//			u_int lightCnt = 1;
//			for (auto& pSpotL : pSpotLights)	// �V�[�����̃|�C���g���C�g
//			{
//				std::string idxStr = std::to_string(lightCnt);
//
//				if (ImGuiMethod::TreeNode(idxStr.c_str()))
//				{
//					// �\��
//					bool isDisplay = pSpotL->GetDisplay();
//					ImGui::Checkbox("Display", &isDisplay);
//					pSpotL->SetDisplay(isDisplay);
//
//					// ���W
//					DXSimp::Vector3 p = pSpotL->GetPosition();
//					ImGuiMethod::DragFloat3(p, "���W", PARAM_CHANGE_SPEED);
//					pSpotL->SetPosition(p);
//
//					// �F
//					DXSimp::Color c = pSpotL->GetColor();
//					ImGuiMethod::ColorEdit4(c, "�J���[");
//					pSpotL->SetColor(c);
//
//					// �Ǝˋ���
//					float dis = pSpotL->GetDistance();
//					ImGui::DragFloat(TO_UTF8("�Ǝˋ���"), &dis, PARAM_CHANGE_SPEED);
//					pSpotL->SetDistance(dis);
//
//					// �Ǝˊp�x
//					float a = pSpotL->GetRangeAngle();
//					ImGui::DragFloat(TO_UTF8("�Ǝˊp�x"), &a, PARAM_CHANGE_SPEED);
//					pSpotL->SetDistance(a);
//
//					// ����
//					DXSimp::Vector3 dir = pSpotL->GetLightDir();
//					ImGuiMethod::DragFloat3(dir, TO_UTF8("����"), PARAM_CHANGE_SPEED);
//					pSpotL->SetDirection(dir);
//				}
//
//				lightCnt++;
//			}
//
//			ImGui::TreePop();
//		}
//#endif
//	}

	void SceneLights::Draw()
	{
		pDirection->Draw();
	}

	SceneLights::SceneLights()
	{
		pSceneLightsParam = std::make_unique<SceneLightsParam>();
		pDirection = std::make_unique<DirectionLight>();
		pDirection->SetDirection({ 1.0f, -0.6f, 0.1f });
	}

	void SceneLights::Update()
	{
		UpdateParameter();
		Draw();
	}

	DirectionLight* SceneLights::SetDirectionLight(std::unique_ptr<DirectionLight> _direction)
	{
		pDirection = std::move(_direction);

		return pDirection.get();
	}

	//PointLight* SceneLights::SetPointLight(std::unique_ptr<PointLight> _point)
	//{
	//	if (static_cast<u_int>(pPointLights.size()) >= MAX_POINTLIGHT)
	//	{
	//		HASHI_DEBUG_LOG("�z�u�ł���|�C���g���C�g�̍ő吔�𒴂��܂���"
	//			+ std::to_string(MAX_POINTLIGHT));
	//		return nullptr;
	//	}

	//	// �|�C���g���C�g��z��ɒǉ�
	//	PointLight* ptr = _point.get();
	//	pPointLights.push_back(std::move(_point));
	//	return ptr;
	//}

	//SpotLight* SceneLights::SetSpotLight(std::unique_ptr<SpotLight> _spot)
	//{
	//	if (static_cast<u_int>(pPointLights.size()) >= MAX_SPOTLIGHT)
	//	{
	//		HASHI_DEBUG_LOG("�z�u�ł���X�|�b�g���C�g�̍ő吔�𒴂��܂���"
	//			+ std::to_string(MAX_SPOTLIGHT));
	//		return nullptr;
	//	}

	//	// �X�|�b�g���C�g��z��ɒǉ�
	//	SpotLight* ptr = _spot.get();
	//	pSpotLights.push_back(std::move(_spot));
	//	return ptr;
	//}

	SceneLightsParam& SceneLights::GetLightsParam()
	{
		return *pSceneLightsParam;
	}

	const DirectionLight& SceneLights::GetDirectionLight() const
	{
		return *pDirection;
	}

	//DirectionLParameter SceneLights::GetDirectionParameter()
	//{
	//	if (pDirection != nullptr)
	//		return pDirection->GetParameter();
	//
	//	HASHI_DEBUG_LOG("�f�B���N�V�������C�g���ݒ肳��Ă��܂���");
	//	DirectionLParameter d;
	//	return d;
	//}

	//const std::vector<PointLParameter>& SceneLights::GetPointParameter()
	//{
	//	return pointParameters;
	//}
	//
	//const std::vector<SpotLParameter>& SceneLights::GetSpotParameter()
	//{
	//	return spotParameters;
	//}
}