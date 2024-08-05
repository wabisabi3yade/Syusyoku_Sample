#pragma once

// イージング基底クラス
class Ease_Base
{
public:
	Ease_Base() {}
	~Ease_Base() {}

	// 各種イージング
	virtual float EaseIn(float _ratio) const = 0;
	virtual float EaseOut(float _ratio) const = 0;
	virtual float EaseInOut(float _ratio) const = 0;
};

class Ease_Linear : public Ease_Base
{
public:
	Ease_Linear() {}
	~Ease_Linear() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_Sine : public Ease_Base
{
public:
	Ease_Sine() {};
	~Ease_Sine() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class  Ease_Quad : public Ease_Base
{
public:
	Ease_Quad() {};
	~Ease_Quad() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class  Ease_Cubic : public Ease_Base
{
public:
	Ease_Cubic() {}
	~Ease_Cubic() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class  Ease_Quart : public Ease_Base
{
public:
	Ease_Quart() {};
	~Ease_Quart() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class  Ease_Back : public Ease_Base
{
public:
	Ease_Back() {};
	~Ease_Back() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class  Ease_Elastic : public Ease_Base
{
public:
	Ease_Elastic() {};
	~Ease_Elastic() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class  Ease_Bounce : public Ease_Base
{
public:
	Ease_Bounce() {};
	~Ease_Bounce() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

inline float Ease_Linear::EaseIn(float _ratio) const
{
	return _ratio;
}

inline float Ease_Linear::EaseOut(float _ratio) const
{
	return _ratio;
}

inline float Ease_Linear::EaseInOut(float _ratio) const
{
	return _ratio;
}

inline float Ease_Sine::EaseIn(float _ratio) const
{
	return 1.0f - cos((_ratio * Mathf::PI) * 0.5f);
}

inline float Ease_Sine::EaseOut(float _ratio) const
{
	return sin((_ratio * Mathf::PI) * 0.5f);
}

inline float Ease_Sine::EaseInOut(float _ratio) const
{
	return -(cos(Mathf::PI * _ratio) - 1.0f) * 0.5f;
}

inline float Ease_Quad::EaseIn(float _ratio) const
{
	return _ratio * _ratio;
}

inline float Ease_Quad::EaseOut(float _ratio) const
{
	float v = 1.0f - _ratio;
	return 1.0f - v * v;
}

inline float Ease_Quad::EaseInOut(float _ratio) const
{
	float v = -2.0f * _ratio + 2.0f;
	return _ratio < 0.5f ? 2.0f * _ratio * _ratio : 1.0f - v * v * 0.5f;
}

inline float Ease_Cubic::EaseIn(float _ratio) const
{
	return _ratio * _ratio * _ratio;
}

inline float Ease_Cubic::EaseOut(float _ratio) const
{
	float v = 1 - _ratio;
	return 1.0f - v * v * v;
}

inline float Ease_Cubic::EaseInOut(float _ratio) const
{
	float v = -2.0f * _ratio + 2.0f;
	return _ratio < 0.5f ? 4.0f * _ratio * _ratio * _ratio : 1.0f - v * v * v * 0.5f;
}

inline float Ease_Quart::EaseIn(float _ratio) const
{
	return _ratio * _ratio * _ratio * _ratio;
}

inline float Ease_Quart::EaseOut(float _ratio) const
{
	float v = 1.0f - _ratio;
	return 1.0f - v * v * v * v;
}

inline float Ease_Quart::EaseInOut(float _ratio) const
{
	float r1 = 8.0f * _ratio * _ratio * _ratio * _ratio;

	float v = -2.0f * _ratio + 2.0f;
	float r2 = 1.0f - v * v * v * v * 0.5f;

	return _ratio < 0.5f ? r1 : r2;
}

inline float Ease_Back::EaseIn(float _ratio) const
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * _ratio * _ratio * _ratio - c1 * _ratio * _ratio;
}

inline float Ease_Back::EaseOut(float _ratio) const
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	float v = _ratio - 1.0f;

	return 1.0f + c3 * v * v * v + c1 * v * v;
}

inline float Ease_Back::EaseInOut(float _ratio) const
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	float v1 = 2 * _ratio;
	float v2 = 2 * _ratio - 2.0f;
	return _ratio < 0.5f
		? (v1 * v1 * ((c2 + 1.0f) * 2.0f * _ratio - c2)) * 0.5f
		: (v2 * v2 * ((c2 + 1.0f) * (_ratio * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;
}

inline float Ease_Elastic::EaseIn(float _ratio) const
{
	const float c4 = (2.0f * Mathf::PI) / 3.0f;

	return _ratio == 0.0f
		? 0.0f
		: _ratio == 1.0f
		? 1.0f
		: -pow(2.0f, 10.0f * _ratio - 10.0f) * sin((_ratio * 10.0f - 10.75f) * c4);
}

inline float Ease_Elastic::EaseOut(float _ratio) const
{
	const float c4 = (2.0f * Mathf::PI) / 3.0f;

	return _ratio == 0.0f
		? 0.0f
		: _ratio == 1.0f
		? 1.0f
		: pow(2.0f, -10.0f * _ratio) * sin((_ratio * 10.0f - 0.75f) * c4) + 1.0f;
}

inline float Ease_Elastic::EaseInOut(float _ratio) const
{
	const float c5 = (2.0f * Mathf::PI) / 4.5f;

	return _ratio == 0.0f
		? 0.0f
		: _ratio == 1.0f
		? 1.0f
		: _ratio < 0.5f
		? -(pow(2.0f, 20.0f * _ratio - 10.0f) * sin((20.0f * _ratio - 11.125f) * c5)) * 0.5f
		: (pow(2.0f, -20.0f * _ratio + 10.0f) * sin((20.0f * _ratio - 11.125f) * c5)) * 0.5f + 1.0f;
}

inline float Ease_Bounce::EaseIn(float _ratio) const
{
	return 1.0f - EaseOut(1.0f - _ratio);
}

inline float Ease_Bounce::EaseOut(float _ratio) const
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (_ratio < 1.0f / d1) {
		return n1 * _ratio * _ratio;
	}
	else if (_ratio < 2.0f / d1) {
		_ratio -= 1.5 / d1;
		return n1 * _ratio * _ratio + 0.75f;
	}
	else if (_ratio < 2.5f / d1) {
		_ratio -= 2.25f / d1;
		return n1 * _ratio * _ratio + 0.9375f;
	}
	else {
		_ratio -= 2.625f / d1;
		return n1 * _ratio * _ratio + 0.984375f;
	}
}

inline float Ease_Bounce::EaseInOut(float _ratio) const
{
	return _ratio < 0.5f
		? (1.0f - EaseOut(1.0f - 2.0f * _ratio)) * 0.5f
		: (1.0f + EaseOut(2.0f * _ratio - 1.0f)) * 0.5f;
}



