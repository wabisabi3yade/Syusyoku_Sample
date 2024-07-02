#pragma once
#include "Light.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"

// �V�[�����̌������X�g
class SceneLights
{
	// �f�B���N�V����
	std::unique_ptr<DirectionLight> pDirection;

	// �|�C���g���C�g
	std::list<std::unique_ptr<PointLight>> pPointLights;
	// �|�C���g���C�g�p�����[�^�z��
	std::vector<PointLParameter> pointParameters;

	// �X�|�b�g���C�g
	std::list<std::unique_ptr<SpotLight>> pSpotLights;
	// �X�|�b�g���C�g�p�����[�^�z��
	std::vector<SpotLParameter> spotParameters;

	// �����̃p�����[�^���X�V����
	void UpdateParameter();

	// ImGui�̕\��
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

