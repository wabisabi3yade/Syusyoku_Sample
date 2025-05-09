#include "pch.h"
#include "PerlinNoise.h"

HashiTaku::PerlinNoise::PerlinNoise(const std::uint_fast32_t _seed)
{
	SetSeed(_seed);
}

void HashiTaku::PerlinNoise::SetSeed(const std::uint_fast32_t _seed)
{
	for (std::size_t i{}; i < 256; ++i)
		this->p[i] = static_cast<Pint>(i);

	std::shuffle(this->p.begin(), this->p.begin() + 256, std::default_random_engine(_seed));

	for (std::size_t i{}; i < 256; ++i)
		this->p[256 + i] = this->p[i];
}

double HashiTaku::PerlinNoise::Noise(double _x, double _y, double _z) const noexcept
{
	return SetNoise(_x, _y, _z) * 0.5 + 0.5;
}

constexpr double HashiTaku::PerlinNoise::GetFade(const double _t) const noexcept
{
	return _t * _t * _t * (_t * (_t * 6 - 15) + 10);
}

constexpr double HashiTaku::PerlinNoise::GetLerp(const double _t, 
	const double _a, 
	const double _b) const noexcept
{
	return _a + _t * (_b - _a);
}

constexpr double HashiTaku::PerlinNoise::MakeGrad(const Pint _hash, 
	const double _u, 
	const double _v) const noexcept
{
	return (((_hash & 1) == 0) ? _u : -_u) + (((_hash & 2) == 0) ? _v : -_v);
}

constexpr double HashiTaku::PerlinNoise::MakeGrad(const Pint _hash, 
	const double _x, 
	const double _y, 
	const double _z) const noexcept
{
	return this->MakeGrad(_hash, 
		_hash < 8 ? _x : _y, 
		_hash < 4 ? _y : _hash == 12 || _hash == 14 ? _x : _z);
}

constexpr double HashiTaku::PerlinNoise::GetGrad(const Pint _hash,
	const double _x, 
	const double _y,
	const double _z) const noexcept
{
	return this->MakeGrad(_hash & 15, _x, _y, _z);
}

double HashiTaku::PerlinNoise::SetNoise(double _x, double _y, double _z) const noexcept
{
	const std::size_t _xint{ static_cast<std::size_t>(static_cast<std::size_t>(std::floor(_x)) & 255) };
	const std::size_t _yint{ static_cast<std::size_t>(static_cast<std::size_t>(std::floor(_y)) & 255) };
	const std::size_t _zint{ static_cast<std::size_t>(static_cast<std::size_t>(std::floor(_z)) & 255) };
	_x -= std::floor(_x);
	_y -= std::floor(_y);
	_z -= std::floor(_z);
	const double u{ this->GetFade(_x) };
	const double v{ this->GetFade(_y) };
	const double w{ this->GetFade(_z) };
	const std::size_t a0{ static_cast<std::size_t>(this->p[_xint] + _yint) };
	const std::size_t a1{ static_cast<std::size_t>(this->p[a0] + _zint) };
	const std::size_t a2{ static_cast<std::size_t>(this->p[a0 + 1] + _zint) };
	const std::size_t b0{ static_cast<std::size_t>(this->p[_xint + 1] + _yint) };
	const std::size_t b1{ static_cast<std::size_t>(this->p[b0] + _zint) };
	const std::size_t b2{ static_cast<std::size_t>(this->p[b0 + 1] + _zint) };

	return this->GetLerp(w,
		this->GetLerp(v,
			this->GetLerp(u, this->GetGrad(this->p[a1], _x, _y, _z), this->GetGrad(this->p[b1], _x - 1, _y, _z)),
			this->GetLerp(u, this->GetGrad(this->p[a2], _x, _y - 1, _z), this->GetGrad(this->p[b2], _x - 1, _y - 1, _z))),
		this->GetLerp(v,
			this->GetLerp(u, this->GetGrad(this->p[a1 + 1], _x, _y, _z - 1), this->GetGrad(this->p[b1 + 1], _x - 1, _y, _z - 1)),
			this->GetLerp(u, this->GetGrad(this->p[a2 + 1], _x, _y - 1, _z - 1), this->GetGrad(this->p[b2 + 1], _x - 1, _y - 1, _z - 1))));
}
