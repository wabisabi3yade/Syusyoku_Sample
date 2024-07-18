#include "pch.h"
#include "SceneLights.h"

using namespace DirectX::SimpleMath;

// 各光源の最大数
constexpr u_int MAX_SPOTS(30);
constexpr u_int MAX_POINTS(60);

// GUIパラメータ変更速度
constexpr float PARAM_CHANGE_SPEED(0.1f);

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
	ImGui::Begin(TO_UTF8("シーン　光源"));

	ImGuiDirection();
	ImGuiPoint();
	ImGuiSpot();

	ImGui::End();
}

void SceneLights::ImGuiDirection()
{
#ifdef EDIT
	if (ImGui::TreeNode(TO_UTF8("ディレクションライト")))
	{
		// 表示
		bool isDisplay = pDirection->GetDisplay();
		ImGui::Checkbox("Display", &isDisplay);
		pDirection->SetDisplay(isDisplay);

		// 座標
		Vector3 p = pDirection->GetPosition();
		ImGuiMethod::DragFloat3(p, "座標", PARAM_CHANGE_SPEED);
		pDirection->SetPosition(p);

		//色
		Color c = pDirection->GetColor();
		ImGuiMethod::ColorEdit4(c, "カラー");
		pDirection->SetColor(c);

		// 方向
		Vector3 d = pDirection->GetDirection();
		ImGuiMethod::DragFloat3(d, "方向", PARAM_CHANGE_SPEED);
		pDirection->SetDirection(d);

		ImGui::TreePop();
	}
#endif
}

void SceneLights::ImGuiPoint()
{
#ifdef EDIT
	if (ImGui::TreeNode(TO_UTF8("ポイントライト")))
	{
		u_int lightCnt = 1;
		for (auto& pPointL : pPointLights)	// シーン内のポイントライト
		{
			std::string idxStr = std::to_string(lightCnt);

			if (ImGui::TreeNode(idxStr.c_str()))
			{
				// 表示
				bool isDisplay = pPointL->GetDisplay();
				ImGui::Checkbox("Display", &isDisplay);
				pPointL->SetDisplay(isDisplay);

				// 座標
				Vector3 p = pPointL->GetPosition();
				ImGuiMethod::DragFloat3(p, "座標", PARAM_CHANGE_SPEED);
				pPointL->SetPosition(p);

				// 色
				Color c = pPointL->GetColor();
				ImGuiMethod::ColorEdit4(c, "カラー");
				pPointL->SetColor(c);

				// 範囲
				float r = pPointL->GetRange();
				ImGui::DragFloat(TO_UTF8("範囲"), &r, PARAM_CHANGE_SPEED);
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
	if (ImGui::TreeNode(TO_UTF8("スポットライト")))
	{
		u_int lightCnt = 1;
		for (auto& pSpotL : pSpotLights)	// シーン内のポイントライト
		{
			std::string idxStr = std::to_string(lightCnt);

			if (ImGui::TreeNode(idxStr.c_str()))
			{
				// 表示
				bool isDisplay = pSpotL->GetDisplay();
				ImGui::Checkbox("Display", &isDisplay);
				pSpotL->SetDisplay(isDisplay);

				// 座標
				Vector3 p = pSpotL->GetPosition();
				ImGuiMethod::DragFloat3(p, "座標", PARAM_CHANGE_SPEED);
				pSpotL->SetPosition(p);

				// 色
				Color c = pSpotL->GetColor();
				ImGuiMethod::ColorEdit4(c, "カラー");
				pSpotL->SetColor(c);

				// 照射距離
				float dis = pSpotL->GetDistance();
				ImGui::DragFloat(TO_UTF8("照射距離"), &dis, PARAM_CHANGE_SPEED);
				pSpotL->SetDistance(dis);

				// 照射角度
				float a = pSpotL->GetRangeAngle();
				ImGui::DragFloat(TO_UTF8("照射角度"), &a, PARAM_CHANGE_SPEED);
				pSpotL->SetDistance(a);

				// 方向
				Vector3 dir = pSpotL->GetLightDir();
				ImGuiMethod::DragFloat3(dir, TO_UTF8("方向"), PARAM_CHANGE_SPEED);
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
