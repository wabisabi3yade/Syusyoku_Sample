#include "pch.h"
#include "SceneLights.h"

void SceneLights::SetDirectionLight(std::unique_ptr<Light> _direction)
{
	// �f�B���N�V�������C�g��
	if (_direction->GetType() != Light::Type::Direction) return;

	pDirection = std::move(_direction);
}

PointLight& SceneLights::SetPointLight(std::unique_ptr<PointLight> _point)
{
	// �_������z��ɒǉ����A�A�h���X��Ԃ�
	PointLight* ptr = _point.get();
	pointLights.push_back(std::move(_point));
	return *ptr;
}

SpotLight& SceneLights::SetSpotLight(std::unique_ptr<SpotLight> _spot)
{
	// �X�|�b�g���C�g��z��ɒǉ����A�A�h���X��Ԃ�
	SpotLight* ptr = _spot.get();
	spotLights.push_back(std::move(_spot));
	return *ptr;
}
