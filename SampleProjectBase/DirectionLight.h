#pragma once
#include "Light.h"

namespace HashiTaku
{
	struct DirectionLParameter
	{
		LightParameter base;
		DXSimp::Color ambeimt{ 0.5f, 0.5f, 0.5f, 1.0f };	// 環境光
		DXSimp::Vector3 direction{ 0.0f, -1.0f, 1.0f };	// 方向
		float dummy;
	};

	class DirectionLight
		: public Light
	{
		// 環境色
		DXSimp::Vector4 ambiemt;

		// 方向ベクトル
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

	/// @brief ディレクションライトのNullオブジェクト
	class NullDirectionLight : public DirectionLight
	{
	public:
		NullDirectionLight() {}
		~NullDirectionLight() {}

		void Draw() override { /*assert(!"Nullのディレクションライトが使われています");*/ };
	};
}