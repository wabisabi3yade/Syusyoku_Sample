#pragma once

class IEasing
{
public:
	IEasing() {}
	virtual ~IEasing() {}

	virtual float Get(float _ratio) const = 0;
};

class EaseFunc_Base
{
public:
	EaseFunc_Base() {}
	virtual ~EaseFunc_Base() {}

	// 各種イージング
	virtual float EaseIn(float _ratio) const = 0;
	virtual float EaseOut(float _ratio) const { return 0.0f; };
	virtual float EaseInOut(float _ratio) const { return 0.0f; };
};

class EaseFunc_Linear : public EaseFunc_Base
{
public:
	EaseFunc_Linear() {}
	~EaseFunc_Linear() {}

	inline float EaseIn(float _ratio) const override;
};

class Ease_Linear : public EaseFunc_Linear, public IEasing
{
public:
	Ease_Linear() {}
	~Ease_Linear() {}

	inline float Get(float _ratio) const override { return EaseFunc_Linear::EaseIn(_ratio); };
};

class EaseFunc_Sine : public EaseFunc_Base
{
public:
	EaseFunc_Sine() {}
	~EaseFunc_Sine() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InSine : public EaseFunc_Sine, public IEasing
{
public:
	Ease_InSine() {}
	~Ease_InSine() {}

	inline float Get(float _ratio) const override { return EaseFunc_Sine::EaseIn(_ratio); }
};

class Ease_OutSine : public  EaseFunc_Sine, public IEasing
{
public:
	Ease_OutSine() {}
	~Ease_OutSine() {}

	inline float Get(float _ratio) const override { return EaseFunc_Sine::EaseOut(_ratio); }
};

class Ease_InOutSine : public EaseFunc_Sine, public IEasing
{
public:
	Ease_InOutSine() {}
	~Ease_InOutSine() {}

	inline float Get(float _ratio) const override { return EaseFunc_Sine::EaseInOut(_ratio); };
};

class  EaseFunc_Quad : public EaseFunc_Base
{
public:
	EaseFunc_Quad() {};
	~EaseFunc_Quad() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InQuad : public EaseFunc_Quad, public IEasing
{
public:
	Ease_InQuad() {}
	~Ease_InQuad() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quad::EaseIn(_ratio); }
};

class Ease_OutQuad : public EaseFunc_Quad, public IEasing
{
public:
	Ease_OutQuad() {}
	~Ease_OutQuad() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quad::EaseOut(_ratio); }
};

class Ease_InOutQuad : public EaseFunc_Quad, public IEasing
{
public:
	Ease_InOutQuad() {}
	~Ease_InOutQuad() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quad::EaseInOut(_ratio); }
};

class  EaseFunc_Cubic : public EaseFunc_Base
{
public:
	EaseFunc_Cubic() {}
	~EaseFunc_Cubic() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InCubic : public EaseFunc_Cubic, public IEasing
{
public:
	Ease_InCubic() {}
	~Ease_InCubic() {}

	inline float Get(float _ratio) const override { return EaseFunc_Cubic::EaseIn(_ratio); }
};

class Ease_OutCubic : public EaseFunc_Cubic, public IEasing
{
public:
	Ease_OutCubic() {}
	~Ease_OutCubic() {}

	inline float Get(float _ratio) const override { return EaseFunc_Cubic::EaseOut(_ratio); }
};

class Ease_InOutCubic : public EaseFunc_Cubic, public IEasing
{
public:
	Ease_InOutCubic() {}
	~Ease_InOutCubic() {}

	inline float Get(float _ratio) const override { return EaseFunc_Cubic::EaseInOut(_ratio); }
};

class  EaseFunc_Quart : public EaseFunc_Base
{
public:
	EaseFunc_Quart() {};
	~EaseFunc_Quart() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InQuart : public EaseFunc_Quart, public IEasing
{
public:
	Ease_InQuart() {}
	~Ease_InQuart() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quart::EaseIn(_ratio); }
};

class Ease_OutQuart : public EaseFunc_Quart, public IEasing
{
public:
	Ease_OutQuart() {}
	~Ease_OutQuart() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quart::EaseOut(_ratio); }
};

class Ease_InOutQuart : public EaseFunc_Quart, public IEasing
{
public:
	Ease_InOutQuart() {}
	~Ease_InOutQuart() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quart::EaseInOut(_ratio); }
};

class  EaseFunc_Quint : public EaseFunc_Base
{
public:
	EaseFunc_Quint() {};
	~EaseFunc_Quint() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InQuint : public EaseFunc_Quint, public IEasing
{
public:
	Ease_InQuint() {}
	~Ease_InQuint() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quint::EaseIn(_ratio); }
};

class Ease_OutQuint : public EaseFunc_Quint, public IEasing
{
public:
	Ease_OutQuint() {}
	~Ease_OutQuint() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quint::EaseOut(_ratio); }
};

class Ease_InOutQuint : public EaseFunc_Quint, public IEasing
{
public:
	Ease_InOutQuint() {}
	~Ease_InOutQuint() {}

	inline float Get(float _ratio) const override { return EaseFunc_Quint::EaseInOut(_ratio); }
};

class  EaseFunc_Expo : public EaseFunc_Base
{
public:
	EaseFunc_Expo() {};
	~EaseFunc_Expo() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InExpo : public EaseFunc_Expo, public IEasing
{
public:
	Ease_InExpo() {}
	~Ease_InExpo() {}

	inline float Get(float _ratio) const override { return EaseFunc_Expo::EaseIn(_ratio); }
};

class Ease_OutExpo : public EaseFunc_Expo, public IEasing
{
public:
	Ease_OutExpo() {}
	~Ease_OutExpo() {}

	inline float Get(float _ratio) const override { return EaseFunc_Expo::EaseOut(_ratio); }
};

class Ease_InOutExpo : public EaseFunc_Expo, public IEasing
{
public:
	Ease_InOutExpo() {}
	~Ease_InOutExpo() {}

	inline float Get(float _ratio) const override { return EaseFunc_Expo::EaseInOut(_ratio); }
};

class  EaseFunc_Circ : public EaseFunc_Base
{
public:
	EaseFunc_Circ() {};
	~EaseFunc_Circ() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InCirc : public EaseFunc_Circ, public IEasing
{
public:
	Ease_InCirc() {}
	~Ease_InCirc() {}

	inline float Get(float _ratio) const override { return EaseFunc_Circ::EaseIn(_ratio); }
};

class Ease_OutCirc : public EaseFunc_Circ, public IEasing
{
public:
	Ease_OutCirc() {}
	~Ease_OutCirc() {}

	inline float Get(float _ratio) const override { return EaseFunc_Circ::EaseOut(_ratio); }
};

class Ease_InOutCirc : public EaseFunc_Circ, public IEasing
{
public:
	Ease_InOutCirc() {}
	~Ease_InOutCirc() {}

	inline float Get(float _ratio) const override { return EaseFunc_Circ::EaseInOut(_ratio); }
};

class  EaseFunc_Back : public EaseFunc_Base
{
public:
	EaseFunc_Back() {};
	~EaseFunc_Back() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InBack : public EaseFunc_Back, public IEasing
{
public:
	Ease_InBack() {}
	~Ease_InBack() {}

	inline float Get(float _ratio) const override { return EaseFunc_Back::EaseIn(_ratio); }
};

class Ease_OutBack : public EaseFunc_Back, public IEasing
{
public:
	Ease_OutBack() {}
	~Ease_OutBack() {}

	inline float Get(float _ratio) const override { return EaseFunc_Back::EaseOut(_ratio); }
};

class Ease_InOutBack : public EaseFunc_Back, public IEasing
{
public:
	Ease_InOutBack() {}
	~Ease_InOutBack() {}

	inline float Get(float _ratio) const override { return EaseFunc_Back::EaseInOut(_ratio); }
};

class  EaseFunc_Elastic : public EaseFunc_Base
{
public:
	EaseFunc_Elastic() {};
	~EaseFunc_Elastic() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InElastic : public EaseFunc_Elastic, public IEasing
{
public:
	Ease_InElastic() {}
	~Ease_InElastic() {}

	inline float Get(float _ratio) const override { return EaseFunc_Elastic::EaseIn(_ratio); }
};

class Ease_OutElastic : public EaseFunc_Elastic, public IEasing
{
public:
	Ease_OutElastic() {}
	~Ease_OutElastic() {}

	inline float Get(float _ratio) const override { return EaseFunc_Elastic::EaseOut(_ratio); }
};

class Ease_InOutElastic : public EaseFunc_Elastic, public IEasing
{
public:
	Ease_InOutElastic() {}
	~Ease_InOutElastic() {}

	inline float Get(float _ratio) const override { return EaseFunc_Elastic::EaseInOut(_ratio); }
};

class  EaseFunc_Bounce : public EaseFunc_Base
{
public:
	EaseFunc_Bounce() {};
	~EaseFunc_Bounce() {}

	inline float EaseIn(float _ratio) const override;
	inline float EaseOut(float _ratio) const override;
	inline float EaseInOut(float _ratio) const override;
};

class Ease_InBounce : public EaseFunc_Bounce, public IEasing
{
public:
	Ease_InBounce() {}
	~Ease_InBounce() {}

	inline float Get(float _ratio) const override { return EaseFunc_Bounce::EaseIn(_ratio); }
};

class Ease_OutBounce : public EaseFunc_Bounce, public IEasing
{
public:
	Ease_OutBounce() {}
	~Ease_OutBounce() {}

	inline float Get(float _ratio) const override { return EaseFunc_Bounce::EaseOut(_ratio); }
};

class Ease_InOutBounce : public EaseFunc_Bounce, public IEasing
{
public:
	Ease_InOutBounce() {}
	~Ease_InOutBounce() {}

	inline float Get(float _ratio) const override { return EaseFunc_Bounce::EaseInOut(_ratio); }
};

inline float EaseFunc_Linear::EaseIn(float _ratio) const
{
	return _ratio;
}

inline float EaseFunc_Sine::EaseIn(float _ratio) const
{
	return 1.0f - cos((_ratio * Mathf::PI) * 0.5f);
}

inline float EaseFunc_Sine::EaseOut(float _ratio) const
{
	return sin((_ratio * Mathf::PI) * 0.5f);
}

inline float EaseFunc_Sine::EaseInOut(float _ratio) const
{
	return -(cos(Mathf::PI * _ratio) - 1.0f) * 0.5f;
}

inline float EaseFunc_Quad::EaseIn(float _ratio) const
{
	return _ratio * _ratio;
}

inline float EaseFunc_Quad::EaseOut(float _ratio) const
{
	float v = 1.0f - _ratio;
	return 1.0f - v * v;
}

inline float EaseFunc_Quad::EaseInOut(float _ratio) const
{
	float v = -2.0f * _ratio + 2.0f;
	return _ratio < 0.5f ? 2.0f * _ratio * _ratio : 1.0f - v * v * 0.5f;
}

inline float EaseFunc_Cubic::EaseIn(float _ratio) const
{
	return _ratio * _ratio * _ratio;
}

inline float EaseFunc_Cubic::EaseOut(float _ratio) const
{
	float v = 1 - _ratio;
	return 1.0f - v * v * v;
}

inline float EaseFunc_Cubic::EaseInOut(float _ratio) const
{
	float v = -2.0f * _ratio + 2.0f;
	return _ratio < 0.5f ? 4.0f * _ratio * _ratio * _ratio : 1.0f - v * v * v * 0.5f;
}

inline float EaseFunc_Quart::EaseIn(float _ratio) const
{
	return _ratio * _ratio * _ratio * _ratio;
}

inline float EaseFunc_Quart::EaseOut(float _ratio) const
{
	float v = 1.0f - _ratio;
	return 1.0f - v * v * v * v;
}

inline float EaseFunc_Quart::EaseInOut(float _ratio) const
{
	float r1 = 8.0f * _ratio * _ratio * _ratio * _ratio;

	float v = -2.0f * _ratio + 2.0f;
	float r2 = 1.0f - v * v * v * v * 0.5f;

	return _ratio < 0.5f ? r1 : r2;
}

inline float EaseFunc_Quint::EaseIn(float _ratio) const
{
	return _ratio * _ratio * _ratio * _ratio * _ratio;
}

inline float EaseFunc_Quint::EaseOut(float _ratio) const
{
	float v1 = 1.0f - _ratio;
	return 1.0f - v1 * v1 * v1 * v1 * v1;
}

inline float EaseFunc_Quint::EaseInOut(float _ratio) const
{
	float v = -2.0f * _ratio + 2.0f;

	return _ratio < 0.5f ?
		16.0f * _ratio * _ratio * _ratio * _ratio * _ratio :
		1.0f - v * v * v * v * v / 2.0f;
}

inline float EaseFunc_Expo::EaseIn(float _ratio) const
{
	return _ratio == 0.0f ?
		0.0f :
		pow(2.0f, 10.0f * _ratio - 10.0f);
}

inline float EaseFunc_Expo::EaseOut(float _ratio) const
{
	return  _ratio == 1.0f ?
		1.0f :
		1.0f - pow(2.0f, -10.0f * _ratio);
}

inline float EaseFunc_Expo::EaseInOut(float _ratio) const
{
	return _ratio == 0.0f
		? 0.0f
		: _ratio == 1.0f
		? 1.0f
		: _ratio < 0.5f ? pow(2.0f, 20.0f * _ratio - 10.0f) / 2.0f
		: (2.0f - pow(2.0f, -20.0f * _ratio + 10.0f)) / 2.0f;
}

inline float EaseFunc_Circ::EaseIn(float _ratio) const
{
	return 1.0f - sqrt(1.0f - _ratio * _ratio);
}

inline float EaseFunc_Circ::EaseOut(float _ratio) const
{
	float v1 = _ratio - 1.0f;
	return sqrt(1.0f - v1 * v1);
}

inline float EaseFunc_Circ::EaseInOut(float _ratio) const
{
	float v1 = 2.0f * _ratio;
	float v2 = -2.0f * _ratio + 2.0f;

	return _ratio < 0.5f
		? (1 - sqrt(1 - v1 * v1)) / 2.0f
		: (sqrt(1 - v2 * v2) + 1.0f) / 2.0f;
}

inline float EaseFunc_Back::EaseIn(float _ratio) const
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * _ratio * _ratio * _ratio - c1 * _ratio * _ratio;
}

inline float EaseFunc_Back::EaseOut(float _ratio) const
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	float v = _ratio - 1.0f;

	return 1.0f + c3 * v * v * v + c1 * v * v;
}

inline float EaseFunc_Back::EaseInOut(float _ratio) const
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	float v1 = 2 * _ratio;
	float v2 = 2 * _ratio - 2.0f;
	return _ratio < 0.5f
		? (v1 * v1 * ((c2 + 1.0f) * 2.0f * _ratio - c2)) * 0.5f
		: (v2 * v2 * ((c2 + 1.0f) * (_ratio * 2.0f - 2.0f) + c2) + 2.0f) * 0.5f;
}

inline float EaseFunc_Elastic::EaseIn(float _ratio) const
{
	const float c4 = (2.0f * Mathf::PI) / 3.0f;

	return _ratio == 0.0f
		? 0.0f
		: _ratio == 1.0f
		? 1.0f
		: -pow(2.0f, 10.0f * _ratio - 10.0f) * sin((_ratio * 10.0f - 10.75f) * c4);
}

inline float EaseFunc_Elastic::EaseOut(float _ratio) const
{
	const float c4 = (2.0f * Mathf::PI) / 3.0f;

	return _ratio == 0.0f
		? 0.0f
		: _ratio == 1.0f
		? 1.0f
		: pow(2.0f, -10.0f * _ratio) * sin((_ratio * 10.0f - 0.75f) * c4) + 1.0f;
}

inline float EaseFunc_Elastic::EaseInOut(float _ratio) const
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

inline float EaseFunc_Bounce::EaseIn(float _ratio) const
{
	return 1.0f - EaseOut(1.0f - _ratio);
}

inline float EaseFunc_Bounce::EaseOut(float _ratio) const
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (_ratio < 1.0f / d1) {
		return n1 * _ratio * _ratio;
	}
	else if (_ratio < 2.0f / d1) {
		_ratio -= 1.5f / d1;
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

inline float EaseFunc_Bounce::EaseInOut(float _ratio) const
{
	return _ratio < 0.5f
		? (1.0f - EaseOut(1.0f - 2.0f * _ratio)) * 0.5f
		: (1.0f + EaseOut(2.0f * _ratio - 1.0f)) * 0.5f;
}