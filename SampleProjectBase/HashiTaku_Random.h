#pragma once
#include "PerlinNoise.h"

namespace HashiTaku
{
	class Random
	{
		friend class MainApplication;	// ������������N���X

		/// @brief Mersenne Twister �G���W��
		static std::mt19937 gen;

		/// @brief �p�[�����m�C�Y
		static PerlinNoise perlinNoise;
	public:

		/// @brief �͈͓��̒l�𗐐��Ő�������
		/// @tparam T float or double
		/// @param min �ŏ��l
		/// @param max �ő�l
		/// @return �����Ő��������l
		template <typename T> requires std::same_as<T, float> || std::same_as<T, double>
		static T Range(T min, T max);

		/// @brief �͈͓��̒l�𗐐��Ő�������(int)
		/// @tparam T int or u_int
		/// @param min �ŏ��l
		/// @param max �ő�l
		/// @return �����Ő��������l
		template <typename T> requires std::same_as<T, int> || std::same_as<T, u_int>
		static int RangeInt(int _min, int _max);

		/// @brief �p�[�����m�C�Y���擾
		/// @param _x X
		/// @param _y Y
		/// @param _z Z
		/// @return �p�[�����m�C�Y�ŋ��߂��l
		static double GetPerlinNoise(double _x = 0.0, double _y = 0.0, double _z = 0.0) noexcept;
	private:
		static void Init();	// �������֐�
	};

	template<typename T>
		requires std::same_as<T, float> || std::same_as<T, double>
	inline T Random::Range(T min, T max)
	{
		std::uniform_real_distribution<T> dist(min, max);
		return dist(gen);
	}

	template<typename T>
		requires std::same_as<T, int> || std::same_as<T, u_int>
	inline int Random::RangeInt(int _min, int _max)
	{
		std::uniform_int_distribution<T> dist(_min, _max);
		return dist(gen);
	}
}


