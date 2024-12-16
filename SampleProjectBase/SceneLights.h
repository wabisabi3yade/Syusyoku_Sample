#pragma once

// �e�탉�C�g
#include "Light.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

namespace HashiTaku
{
	// ���C�g�̍ő吔
	constexpr u_int  MAX_POINTLIGHT(10);
	constexpr u_int  MAX_SPOTLIGHT(4);

	// �V�[�����̌����̃p�����[�^
	struct SceneLightsParam
	{
		DirectionLParameter dirParam;

		/*PointLParameter pointParams[MAX_POINTLIGHT];

		SpotLParameter spotParams[MAX_SPOTLIGHT];*/
	};

	// �V�[�����̌������X�g
	class SceneLights : public IImGuiUser
	{
	private:
		/// @brief �V�[�����̌����p�����[�^
		std::unique_ptr<SceneLightsParam> pSceneLightsParam;

		// �f�B���N�V����
		std::unique_ptr<DirectionLight> pDirection;

		// �|�C���g���C�g
		std::list<std::unique_ptr<PointLight>> pPointLights;

		// �X�|�b�g���C�g
		std::list<std::unique_ptr<SpotLight>> pSpotLights;

	public:
		SceneLights();
		~SceneLights() {};

		void Update();

		// �e���C�g�Z�b�g
		DirectionLight* SetDirectionLight(std::unique_ptr<DirectionLight> _direction);
		PointLight* SetPointLight(std::unique_ptr<PointLight> _point);
		SpotLight* SetSpotLight(std::unique_ptr<SpotLight> _spot);

		/// @brief �����̃p�����[�^���擾����
		/// @return �����p�����[�^
		SceneLightsParam& GetLightsParam();

	private:
		// �����̃p�����[�^���X�V����
		void UpdateParameter();

		void ImGuiDebug() override;

		// ImGui�̕\��
		void ImGuiDirection();
		void ImGuiPoint();
		void ImGuiSpot();

		void Draw();
	};
}