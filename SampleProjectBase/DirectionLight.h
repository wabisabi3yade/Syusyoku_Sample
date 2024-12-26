#pragma once
#include "Light.h"

namespace HashiTaku
{
	struct DirectionLParameter
	{
		LightParameter base;
		DXSimp::Color ambeimt{ 0.5f, 0.5f, 0.5f, 1.0f };	// ����
		DXSimp::Vector3 direction{ 0.0f, -1.0f, 1.0f };	// ����
		float dummy;
	};

	class DirectionLight
		: public Light
	{
		// ���F
		DXSimp::Vector4 ambiemt;

		// �����x�N�g��
		DXSimp::Vector3 direction;
	public:
		DirectionLight() : ambiemt(0.6f, 0.6f, 0.6f, 1.0f), direction(0.0f, -1.0f, 1.0f) { type = Type::Direction; }
		~DirectionLight() {}

		virtual void Draw() override;

		void SetDirection(const DXSimp::Vector3& _dir);
		void SetAmbiemt(const DXSimp::Color& _color);

		DXSimp::Vector3 GetDirection();
		DXSimp::Color GetAmbiemt();

		DirectionLParameter GetParameter();
	};

	/// @brief �f�B���N�V�������C�g��Null�I�u�W�F�N�g
	class NullDirectionLight : public DirectionLight
	{
	public:
		NullDirectionLight() {}
		~NullDirectionLight() {}

		void Draw() override { /*assert(!"Null�̃f�B���N�V�������C�g���g���Ă��܂�");*/ };
	};
}