#include "pch.h"
#include "SF_Light.h"

// システム
#include "InSceneSystemManager.h"

// ライト
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace HashiTaku
{
	DirectionLight* SceneFunction::LightFunc::CreateDirection()
	{
		std::unique_ptr<DirectionLight> d = std::make_unique<DirectionLight>();
		SceneLights& l = InSceneSystemManager::GetInstance()->GetSceneLights();

		return l.SetDirectionLight(std::move(d));
	}

	PointLight* SceneFunction::LightFunc::CreatePoint()
	{
		std::unique_ptr<PointLight> p = std::make_unique<PointLight>();
		SceneLights& l = InSceneSystemManager::GetInstance()->GetSceneLights();

		return l.SetPointLight(std::move(p));
	}

	SpotLight* SceneFunction::LightFunc::CreateSpot()
	{
		std::unique_ptr<SpotLight> s = std::make_unique<SpotLight>();
		SceneLights& l = InSceneSystemManager::GetInstance()->GetSceneLights();

		return l.SetSpotLight(std::move(s));
	}
}