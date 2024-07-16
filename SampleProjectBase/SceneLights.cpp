#include "pch.h"
#include "SceneLights.h"

using namespace DirectX::SimpleMath;

// 各光源の最大数
constexpr u_int MAX_SPOTS(30);
constexpr u_int MAX_POINTS(30);

void SceneLights::UpdateParameter()
{
	// パラメータの更新

	// ポイントライトの更新
	pointParameters.clear();
	for (auto& point : pPointLights)
	{
		pointParameters.push_back(point->GetParameter());
	}

	// スポットライトの更新
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
	ImGui::Begin(ShiftJisToUtf8("シーン　光源").c_str());
	if (ImGui::TreeNode(ShiftJisToUtf8("ディレクションライト").c_str()))
	{
		bool isDisplay = pDirection->GetDisplay();
		ImGui::Checkbox("Display", &isDisplay);
		pDirection->SetDisplay(isDisplay);

		Vector3 p = pDirection->GetPosition();
		ImGuiMethod::DragFloat3(p, "座標");
		pDirection->SetPosition(p);

		Color c = pDirection->GetColor();
		ImGuiMethod::ColorEdit4(c,"カラー");
		pDirection->SetColor(c);

		Vector3 d = pDirection->GetDirection();
		ImGuiMethod::DragFloat3(d, "方向");
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
		HASHI_DEBUG_LOG("配置できるポイントライトの最大数を超えました" 
			+ std::to_string(MAX_POINTS));
		return nullptr;
	}

	// ポイントライトを配列に追加
	PointLight* ptr = _point.get();
	pPointLights.push_back(std::move(_point));
	return ptr;
}

SpotLight* SceneLights::SetSpotLight(std::unique_ptr<SpotLight> _spot)
{
	if (static_cast<u_int>(pPointLights.size()) > MAX_SPOTS)
	{
		HASHI_DEBUG_LOG("配置できるスポットライトの最大数を超えました"
			+ std::to_string(MAX_SPOTS));
		return nullptr;
	}

	// スポットライトを配列に追加
	SpotLight* ptr = _spot.get();
	pSpotLights.push_back(std::move(_spot));
	return ptr;
}

DirectionLParameter SceneLights::GetDirectionParameter()
{
	if (pDirection != nullptr)
		return pDirection->GetParameter();

	HASHI_DEBUG_LOG("ディレクションライトが設定されていません");
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
