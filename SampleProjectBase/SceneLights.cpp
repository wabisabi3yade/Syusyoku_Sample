#include "pch.h"
#include "SceneLights.h"

namespace HashiTaku
{
	// GUIパラメータ変更速度
	constexpr float PARAM_CHANGE_SPEED(0.1f);

	void SceneLights::UpdateParameter()
	{
		// ディレクションライト
		pSceneLightsParam->dirParam = pDirection->GetParameter();

		//// ポイントライト
		//u_int pointCnt = 0;
		//for (auto& p : pPointLights)
		//{
		//	pSceneLightsParam->pointParams[pointCnt] = p->GetParameter();
		//	pointCnt++;
		//}

		//// 使用していない配列は光が反映されないようにしておく
		//for (; pointCnt < MAX_POINTLIGHT; pointCnt++)
		//{
		//	pSceneLightsParam->pointParams[pointCnt].range = 0.0f;
		//}

		//// スポットライト
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
		ImGui::Begin(TO_UTF8("シーン　光源"));

		ImGuiDirection();
		/*ImGuiPoint();
		ImGuiSpot();*/

		ImGui::End();
#endif // EDIT
	}


	void SceneLights::ImGuiDirection()
	{
#ifdef EDIT
		if (ImGuiMethod::TreeNode(TO_UTF8("ディレクションライト")))
		{
			// 表示
			bool isDisplay = pDirection->GetDisplay();
			ImGui::Checkbox("Display", &isDisplay);
			pDirection->SetDisplay(isDisplay);

			// 座標
			DXSimp::Vector3 p = pDirection->GetPosition();
			ImGuiMethod::DragFloat3(p, "座標", PARAM_CHANGE_SPEED);
			pDirection->SetPosition(p);

			//色
			DXSimp::Color c = pDirection->GetColor();
			ImGuiMethod::ColorEdit4(c, "カラー");
			pDirection->SetColor(c);

			// 環境
			DXSimp::Color ac = pDirection->GetAmbiemt();
			ImGuiMethod::ColorEdit4(ac, "アンビエント");
			pDirection->SetAmbiemt(ac);

			// 方向
			DXSimp::Vector3 d = pDirection->GetDirection();
			ImGuiMethod::DragFloat3(d, "方向", PARAM_CHANGE_SPEED);
			pDirection->SetDirection(d);

			ImGui::TreePop();
		}
#endif
	}

//	void SceneLights::ImGuiPoint()
//	{
//#ifdef EDIT
//		if (ImGuiMethod::TreeNode(TO_UTF8("ポイントライト")))
//		{
//			u_int lightCnt = 1;
//			for (auto& pPointL : pPointLights)	// シーン内のポイントライト
//			{
//				std::string idxStr = std::to_string(lightCnt);
//
//				if (ImGuiMethod::TreeNode(idxStr.c_str()))
//				{
//					// 表示
//					bool isDisplay = pPointL->GetDisplay();
//					ImGui::Checkbox("Display", &isDisplay);
//					pPointL->SetDisplay(isDisplay);
//
//					// 座標
//					DXSimp::Vector3 p = pPointL->GetPosition();
//					ImGuiMethod::DragFloat3(p, "座標", PARAM_CHANGE_SPEED);
//					pPointL->SetPosition(p);
//
//					// 色
//					DXSimp::Color c = pPointL->GetColor();
//					ImGuiMethod::ColorEdit4(c, "カラー");
//					pPointL->SetColor(c);
//
//					// 範囲
//					float r = pPointL->GetRange();
//					ImGui::DragFloat(TO_UTF8("範囲"), &r, PARAM_CHANGE_SPEED);
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
//		if (ImGuiMethod::TreeNode(TO_UTF8("スポットライト")))
//		{
//			u_int lightCnt = 1;
//			for (auto& pSpotL : pSpotLights)	// シーン内のポイントライト
//			{
//				std::string idxStr = std::to_string(lightCnt);
//
//				if (ImGuiMethod::TreeNode(idxStr.c_str()))
//				{
//					// 表示
//					bool isDisplay = pSpotL->GetDisplay();
//					ImGui::Checkbox("Display", &isDisplay);
//					pSpotL->SetDisplay(isDisplay);
//
//					// 座標
//					DXSimp::Vector3 p = pSpotL->GetPosition();
//					ImGuiMethod::DragFloat3(p, "座標", PARAM_CHANGE_SPEED);
//					pSpotL->SetPosition(p);
//
//					// 色
//					DXSimp::Color c = pSpotL->GetColor();
//					ImGuiMethod::ColorEdit4(c, "カラー");
//					pSpotL->SetColor(c);
//
//					// 照射距離
//					float dis = pSpotL->GetDistance();
//					ImGui::DragFloat(TO_UTF8("照射距離"), &dis, PARAM_CHANGE_SPEED);
//					pSpotL->SetDistance(dis);
//
//					// 照射角度
//					float a = pSpotL->GetRangeAngle();
//					ImGui::DragFloat(TO_UTF8("照射角度"), &a, PARAM_CHANGE_SPEED);
//					pSpotL->SetDistance(a);
//
//					// 方向
//					DXSimp::Vector3 dir = pSpotL->GetLightDir();
//					ImGuiMethod::DragFloat3(dir, TO_UTF8("方向"), PARAM_CHANGE_SPEED);
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
	//		HASHI_DEBUG_LOG("配置できるポイントライトの最大数を超えました"
	//			+ std::to_string(MAX_POINTLIGHT));
	//		return nullptr;
	//	}

	//	// ポイントライトを配列に追加
	//	PointLight* ptr = _point.get();
	//	pPointLights.push_back(std::move(_point));
	//	return ptr;
	//}

	//SpotLight* SceneLights::SetSpotLight(std::unique_ptr<SpotLight> _spot)
	//{
	//	if (static_cast<u_int>(pPointLights.size()) >= MAX_SPOTLIGHT)
	//	{
	//		HASHI_DEBUG_LOG("配置できるスポットライトの最大数を超えました"
	//			+ std::to_string(MAX_SPOTLIGHT));
	//		return nullptr;
	//	}

	//	// スポットライトを配列に追加
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
	//	HASHI_DEBUG_LOG("ディレクションライトが設定されていません");
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