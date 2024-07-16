#include "pch.h"
#include "SceneLights.h"

using namespace DirectX::SimpleMath;

// �e�����̍ő吔
constexpr u_int MAX_SPOTS(30);
constexpr u_int MAX_POINTS(30);

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
	ImGuiDirection();
}

void SceneLights::ImGuiDirection()
{
#ifdef EDIT
	ImGui::Begin(ShiftJisToUtf8("�V�[���@����").c_str());
	if (ImGui::TreeNode(ShiftJisToUtf8("�f�B���N�V�������C�g").c_str()))
	{
		bool isDisplay = pDirection->GetDisplay();
		ImGui::Checkbox("Display", &isDisplay);
		pDirection->SetDisplay(isDisplay);

		Vector3 p = pDirection->GetPosition();
		ImGuiMethod::DragFloat3(p, "���W");
		pDirection->SetPosition(p);

		Color c = pDirection->GetColor();
		ImGuiMethod::ColorEdit4(c,"�J���[");
		pDirection->SetColor(c);

		Vector3 d = pDirection->GetDirection();
		ImGuiMethod::DragFloat3(d, "����");
		pDirection->SetDirection(d);

		ImGui::TreePop();
	}

	ImGui::End();
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

void SceneLights::SetDirectionLight(std::unique_ptr<DirectionLight> _direction)
{
	pDirection = std::move(_direction);
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
