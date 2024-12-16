#pragma once
#include "Light.h"

namespace HashiTaku
{
	struct SpotLParameter
	{
		LightParameter base;
		DirectX::SimpleMath::Vector3 direction{ 0.0f, -1.0f, 0.0f };
		float distance{ 5.0f };
		float rangeAngle{ 45.0f };
		float dummy1, dummy2, dummy3;
	};

	class SpotLight :
		public Light
	{
		// ���̋���
		float lightDistance;

		// ���ˊp�x
		float rangeAngle;

		// ���̕���
		DirectX::SimpleMath::Vector3 lightDir;
	public:
		SpotLight() : lightDistance(5.0f), rangeAngle(45.0f), lightDir(0.0f, -1.0f, 0.0f)
		{
			type = Type::Spot;
		};
		~SpotLight() {};

		void SetDistance(float _distance);
		void SetRangeAngle(float _rangeAngle);
		void SetDirection(const DirectX::SimpleMath::Vector3& _direction);

		// �e�p�����[�^�擾
		float GetDistance();
		float GetRangeAngle();
		const DirectX::SimpleMath::Vector3& GetLightDir();

		SpotLParameter GetParameter();
	};
}