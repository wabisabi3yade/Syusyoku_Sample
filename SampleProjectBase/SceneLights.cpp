#include "pch.h"
#include "SceneLights.h"

using namespace DirectX::SimpleMath;

// �e�����̍ő吔
constexpr u_int MAX_SPOTS(30);
constexpr u_int MAX_POINTS(60);

// GUI�p�����[�^�ύX���x
constexpr float PARAM_CHANGE_SPEED(0.1f);

void SceneLights::UpdateParameter()
{
	// �p�����[�^�̍X�V

	// �|�C���g���C�g�̍X�V
	pointParameters.clear();
	for (auto& point : pPointLights)
	{
		pointParameters.push_back(point->GetParameter());
	}

	// �X�|�b�g���C�g�̍X�V
	spotParameters.clear();
	for (auto& spot : pSpotLights)
	{
		spotParameters.push_back(spot->GetParameter());
	}
}

void SceneLights::ImGuiDisplay()
{
	ImGui::Begin(TO_UTF8("�V�[���@����"));

	ImGuiDirection();
	ImGuiPoint();
	ImGuiSpot();

	ImGui::End();
}

void SceneLights::ImGuiDirection()
{
#ifdef EDIT
	if (ImGui::TreeNode(TO_UTF8("�f�B���N�V�������C�g")))
	{
		// �\��
		bool isDisplay = pDirection->GetDisplay();
		ImGui::Checkbox("Display", &isDisplay);
		pDirection->SetDisplay(isDisplay);

		// ���W
		Vector3 p = pDirection->GetPosition();
		ImGuiMethod::DragFloat3(p, "���W", PARAM_CHANGE_SPEED);
		pDirection->SetPosition(p);

		//�F
		Color c = pDirection->GetColor();
		ImGuiMethod::ColorEdit4(c, "�J���[");
		pDirection->SetColor(c);

		// ����
		Vector3 d = pDirection->GetDirection();
		ImGuiMethod::DragFloat3(d, "����", PARAM_CHANGE_SPEED);
		pDirection->SetDirection(d);

		ImGui::TreePop();
	}
#endif
}

void SceneLights::ImGuiPoint()
{
#ifdef EDIT
	if (ImGui::TreeNode(TO_UTF8("�|�C���g���C�g")))
	{
		u_int lightCnt = 1;
		for (auto& pPointL : pPointLights)	// �V�[�����̃|�C���g���C�g
		{
			std::string idxStr = std::to_string(lightCnt);

			if (ImGui::TreeNode(idxStr.c_str()))
			{
				// �\��
				bool isDisplay = pPointL->GetDisplay();
				ImGui::Checkbox("Display", &isDisplay);
				pPointL->SetDisplay(isDisplay);

				// ���W
				Vector3 p = pPointL->GetPosition();
				ImGuiMethod::DragFloat3(p, "���W", PARAM_CHANGE_SPEED);
				pPointL->SetPosition(p);

				// �F
				Color c = pPointL->GetColor();
				ImGuiMethod::ColorEdit4(c, "�J���[");
				pPointL->SetColor(c);

				// �͈�
				float r = pPointL->GetRange();
				ImGui::DragFloat(TO_UTF8("�͈�"), &r, PARAM_CHANGE_SPEED);
				pPointL->SetRange(r);
			}

			lightCnt++;
		}

		ImGui::TreePop();
	}
#endif
}

void SceneLights::ImGuiSpot()
{
#ifdef EDIT
	if (ImGui::TreeNode(TO_UTF8("�X�|�b�g���C�g")))
	{
		u_int lightCnt = 1;
		for (auto& pSpotL : pSpotLights)	// �V�[�����̃|�C���g���C�g
		{
			std::string idxStr = std::to_string(lightCnt);

			if (ImGui::TreeNode(idxStr.c_str()))
			{
				// �\��
				bool isDisplay = pSpotL->GetDisplay();
				ImGui::Checkbox("Display", &isDisplay);
				pSpotL->SetDisplay(isDisplay);

				// ���W
				Vector3 p = pSpotL->GetPosition();
				ImGuiMethod::DragFloat3(p, "���W", PARAM_CHANGE_SPEED);
				pSpotL->SetPosition(p);

				// �F
				Color c = pSpotL->GetColor();
				ImGuiMethod::ColorEdit4(c, "�J���[");
				pSpotL->SetColor(c);

				// �Ǝˋ���
				float dis = pSpotL->GetDistance();
				ImGui::DragFloat(TO_UTF8("�Ǝˋ���"), &dis, PARAM_CHANGE_SPEED);
				pSpotL->SetDistance(dis);

				// �Ǝˊp�x
				float a = pSpotL->GetRangeAngle();
				ImGui::DragFloat(TO_UTF8("�Ǝˊp�x"), &a, PARAM_CHANGE_SPEED);
				pSpotL->SetDistance(a);

				// ����
				Vector3 dir = pSpotL->GetLightDir();
				ImGuiMethod::DragFloat3(dir, TO_UTF8("����"), PARAM_CHANGE_SPEED);
				pSpotL->SetDirection(dir);
			}

			lightCnt++;
		}

		ImGui::TreePop();
	}
#endif
}

void SceneLights::Draw()
{
	pDirection->Draw();
}

SceneLights::SceneLights()
{
	pDirection = std::make_unique<NullDirectionLight>();
}

void SceneLights::Update()
{
	UpdateParameter();
	ImGuiDisplay();
	Draw();
}

DirectionLight* SceneLights::SetDirectionLight(std::unique_ptr<DirectionLight> _direction)
{
	pDirection = std::move(_direction);

	return pDirection.get();
}

PointLight* SceneLights::SetPointLight(std::unique_ptr<PointLight> _point)
{
	if (static_cast<u_int>(pPointLights.size()) > MAX_POINTS)
	{
		HASHI_DEBUG_LOG("�z�u�ł���|�C���g���C�g�̍ő吔�𒴂��܂���"
			+ std::to_string(MAX_POINTS));
		return nullptr;
	}

	// �|�C���g���C�g��z��ɒǉ�
	PointLight* ptr = _point.get();
	pPointLights.push_back(std::move(_point));
	return ptr;
}

SpotLight* SceneLights::SetSpotLight(std::unique_ptr<SpotLight> _spot)
{
	if (static_cast<u_int>(pPointLights.size()) > MAX_SPOTS)
	{
		HASHI_DEBUG_LOG("�z�u�ł���X�|�b�g���C�g�̍ő吔�𒴂��܂���"
			+ std::to_string(MAX_SPOTS));
		return nullptr;
	}

	// �X�|�b�g���C�g��z��ɒǉ�
	SpotLight* ptr = _spot.get();
	pSpotLights.push_back(std::move(_spot));
	return ptr;
}

DirectionLParameter SceneLights::GetDirectionParameter()
{
	if (pDirection != nullptr)
		return pDirection->GetParameter();

	HASHI_DEBUG_LOG("�f�B���N�V�������C�g���ݒ肳��Ă��܂���");
	DirectionLParameter d;
	return d;
}

const std::vector<PointLParameter>& SceneLights::GetPointParameter()
{
	return pointParameters;
}

const std::vector<SpotLParameter>& SceneLights::GetSpotParameter()
{
	return spotParameters;
}
