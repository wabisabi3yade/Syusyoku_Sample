#pragma once
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
// �V�[�����̌������X�g
class SceneLights
{
	// �f�B���N�V����
	std::unique_ptr<Light> pDirection;
	// �_����
	std::list<std::unique_ptr<PointLight>> pointLights;
	// �X�|�b�g���C�g
	std::list<std::unique_ptr<SpotLight>> spotLights;

public:
	SceneLights(){};
	~SceneLights(){};

	void SetDirectionLight(std::unique_ptr<Light> _direction);
	PointLight& SetPointLight(std::unique_ptr<PointLight> _point);
	SpotLight& SetSpotLight(std::unique_ptr<SpotLight> _spot);


};

