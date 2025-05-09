#include "pch.h"
#include "VisualEffect.h"

namespace HashiTaku
{
	VisualEffect::VisualEffect() :
		loadScale(1.0f),
		vfxColor(DXSimp::Color(1.0f, 1.0f, 1.0f, 1.0f))
	{
	}

	VisualEffect::VisualEffect(const Effekseer::EffectRef& _effect, float _loadScale) :
		effect(_effect), loadScale(_loadScale)
	{
	}

	VisualEffect::~VisualEffect()
	{
	}

	void VisualEffect::SetVfxColor(const DXSimp::Color& _color)
	{
		vfxColor = _color;
	}

	const Effekseer::EffectRef& VisualEffect::GetEffect() const
	{
		return effect;
	}

	const DXSimp::Color& VisualEffect::GetVfxColor() const
	{
		return vfxColor;
	}

	json VisualEffect::Save()
	{
		auto data = AssetPath_Base::Save();
		SaveJsonVector4("color", vfxColor, data);
		data["loadScale"] = loadScale;
		return data;
	}

	void VisualEffect::Load(const json& _data)
	{
		AssetPath_Base::Load(_data);
		LoadJsonColor("color", vfxColor, _data);
	}
}