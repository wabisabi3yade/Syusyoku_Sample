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

	// ���C�g�̊��
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
		// ���C�g�̍��W�ɖڈ�
		bool isDebugDisplay;
	protected:
		// ���C�g�̎��
		Type type;

		// ���C�g�̃p�����[�^�\����
		LightParameter parameter;
	public:
		Light() : type(Type::None), isDebugDisplay(false) {}
		~Light() {};

		virtual void Draw() {};

		// �e�p�����[�^�Z�b�g
		void SetDisplay(bool _isDisplay) { isDebugDisplay = _isDisplay; }
		void SetPosition(const DXSimp::Vector3& _position) { parameter.position = _position; }
		void SetColor(const DXSimp::Color& _color) { parameter.color = _color; }

		bool GetDisplay() { return isDebugDisplay; }
		const DXSimp::Vector3& GetPosition() const { return parameter.position; }
		const DXSimp::Color& GetColor() const { return parameter.color; }
	};
}