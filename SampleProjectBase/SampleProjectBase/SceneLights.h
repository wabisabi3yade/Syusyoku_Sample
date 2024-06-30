#pragma once
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
// シーン内の光源リスト
class SceneLights
{
	// ディレクション
	std::unique_ptr<Light> pDirection;
	// 点光源
	std::list<std::unique_ptr<PointLight>> pointLights;
	// スポットライト
	std::list<std::unique_ptr<SpotLight>> spotLights;

public:
	SceneLights(){};
	~SceneLights(){};

	void SetDirectionLight(std::unique_ptr<Light> _direction);
	PointLight& SetPointLight(std::unique_ptr<PointLight> _point);
	SpotLight& SetSpotLight(std::unique_ptr<SpotLight> _spot);


};

