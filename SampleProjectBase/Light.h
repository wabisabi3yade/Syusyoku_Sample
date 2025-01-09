#pragma once
#include "GameObject.h"

namespace HashiTaku
{
	struct LightParameter
	{
		DXSimp::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
		DXSimp::Vector3 position{ 0.0f, 0.0f, 0.0f };
		float dummy{ 0.0f };
	};

	// ライトの基底
	class Light
	{
	public:
		enum class Type
		{
			Direction,
			Point,
			Spot,
			Num,
			None
		};
	private:
		// ライトの座標に目印
		bool isDebugDisplay;
	protected:
		// ライトの種類
		Type type;

		// ライトのパラメータ構造体
		LightParameter parameter;
	public:
		Light() : type(Type::None), isDebugDisplay(false) {}
		~Light() {};

		virtual void Draw() {};

		// 各パラメータセット
		void SetDisplay(bool _isDisplay) { isDebugDisplay = _isDisplay; }
		void SetPosition(const DXSimp::Vector3& _position) { parameter.position = _position; }
		void SetColor(const DXSimp::Color& _color) { parameter.color = _color; }

		bool GetDisplay() { return isDebugDisplay; }
		const DXSimp::Vector3& GetPosition() const { return parameter.position; }
		const DXSimp::Color& GetColor() const { return parameter.color; }
	};
}