#pragma once
#include "Light.h"

namespace HashiTaku
{
	struct PointLParameter
	{
		LightParameter base;
		float range{ 5.0f };
		float dummy[3] = {};
	};

	// �|�C���g���C�g
	class PointLight :
		public Light
	{
	private:
		// ���͈̔�
		float lightRange;

		void Copy(const PointLight& _other);
	public:
		PointLight() : lightRange(5.0f) { type = Type::Point; };
		PointLight(const PointLight& _other);
		~PointLight() {}

		PointLight& operator=(const PointLight& _other);

		/// @brief �͈͂�ݒ�
		/// @param _range �͈�
		void SetRange(float _range);

		/// @brief ���͈̔͂��擾����
		/// @return ���͈̔�
		float GetRange() const;
		PointLParameter GetParameter();
	};
}