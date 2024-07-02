#pragma once
#include "Light.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

// シーン内の光源リスト
class SceneLights
{
	// ディレクション
	std::unique_ptr<DirectionLight> pDirection;

	// ポイントライト
	std::list<std::unique_ptr<PointLight>> pPointLights;
	// ポイントライトパラメータ配列
	std::vector<PointLParameter> pointParameters;

	// スポットライト
	std::list<std::unique_ptr<SpotLight>> pSpotLights;
	// スポットライトパラメータ配列
	std::vector<SpotLParameter> spotParameters;

	// 光源のパラメータを更新する
	void UpdateParameter();

	// ImGuiの表示
	void ImGuiDisplay();
	void ImGuiDirection();

	void Draw();
public:
	SceneLights() {};
	~SceneLights() {};

	void Update();

	void SetDirectionLight(std::unique_ptr<DirectionLight> _direction);
	PointLight* SetPointLight(std::unique_ptr<PointLight> _point);
	SpotLight* SetSpotLight(std::unique_ptr<SpotLight> _spot);

	DirectionLParameter GetDirectionParameter();
	const std::vector<PointLParameter>& GetPointParameter();
	const std::vector<SpotLParameter>& GetSpotParameter();
};

