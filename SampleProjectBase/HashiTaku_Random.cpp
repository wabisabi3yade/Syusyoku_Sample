#include "pch.h"
#include "HashiTaku_Random.h"

namespace HashiTaku
{
	std::mt19937 Random::gen;
	PerlinNoise Random::perlinNoise;

	constexpr std::uint_fast32_t SEED(0);

	double Random::GetPerlinNoise(double _x, double _y, double _z) noexcept
	{
		return perlinNoise.Noise(_x, _y, _z);
	}

	void Random::Init()
	{
		// �V�[�h�𐶐����A�ݒ�
		std::random_device rd;
		gen.seed(rd());

		// �p�[�����m�C�Y�̃V�[�h�l���Z�b�g
		perlinNoise.SetSeed(SEED);
	}
}