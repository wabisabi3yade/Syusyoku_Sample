#include "pch.h"
#include "SceneLights.h"

void SceneLights::SetDirectionLight(std::unique_ptr<Light> _direction)
{
	// ディレクションライトか
	if (_direction->GetType() != Light::Type::Direction) return;

	pDirection = std::move(_direction);
}

PointLight& SceneLights::SetPointLight(std::unique_ptr<PointLight> _point)
{
	// 点光源を配列に追加し、アドレスを返す
	PointLight* ptr = _point.get();
	pointLights.push_back(std::move(_point));
	return *ptr;
}

SpotLight& SceneLights::SetSpotLight(std::unique_ptr<SpotLight> _spot)
{
	// スポットライトを配列に追加し、アドレスを返す
	SpotLight* ptr = _spot.get();
	spotLights.push_back(std::move(_spot));
	return *ptr;
}
