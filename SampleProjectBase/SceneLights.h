#pragma once

// 各種ライト
#include "Light.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace HashiTaku
{
	// ライトの最大数
	constexpr u_int  MAX_POINTLIGHT(10);
	constexpr u_int  MAX_SPOTLIGHT(4);

	// シーン内の光源のパラメータ
	struct SceneLightsParam
	{
		DirectionLParameter dirParam;

		/*PointLParameter pointParams[MAX_POINTLIGHT];

		SpotLParameter spotParams[MAX_SPOTLIGHT];*/
	};

	// シーン内の光源リスト
	class SceneLights : public IImGuiUser
	{
	private:
		/// @brief シーン内の光源パラメータ
		std::unique_ptr<SceneLightsParam> pSceneLightsParam;

		// ディレクション
		std::unique_ptr<DirectionLight> pDirection;

		// ポイントライト
		std::list<std::unique_ptr<PointLight>> pPointLights;

		// スポットライト
		std::list<std::unique_ptr<SpotLight>> pSpotLights;

	public:
		SceneLights();
		~SceneLights() {};

		void Update();

		// 各ライトセット
		DirectionLight* SetDirectionLight(std::unique_ptr<DirectionLight> _direction);
		PointLight* SetPointLight(std::unique_ptr<PointLight> _point);
		SpotLight* SetSpotLight(std::unique_ptr<SpotLight> _spot);

		/// @brief 光源のパラメータを取得する
		/// @return 光源パラメータ
		SceneLightsParam& GetLightsParam();

	private:
		// 光源のパラメータを更新する
		void UpdateParameter();

		void ImGuiDebug() override;

		// ImGuiの表示
		void ImGuiDirection();
		void ImGuiPoint();
		void ImGuiSpot();

		void Draw();
	};
}