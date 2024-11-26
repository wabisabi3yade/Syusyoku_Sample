#pragma once

namespace HashiTaku
{
	class Random
	{
		friend class MainApplication;	// 初期化をするクラス

		/// @brief Mersenne Twister エンジン
		static std::mt19937 gen;	
	public:

		/// @brief 範囲内の値を乱数で生成する
		/// @tparam T float or double
		/// @param min 最小値
		/// @param max 最大値
		/// @return 乱数で生成した値
		template <typename T> requires std::same_as<T, float> || std::same_as<T, double>
		static T Range(T min, T max);

		/// @brief 範囲内の値を乱数で生成する(int)
		/// @tparam T int or u_int
		/// @param min 最小値
		/// @param max 最大値
		/// @return 乱数で生成した値
		template <typename T> requires std::same_as<T, int> || std::same_as<T, u_int>
		static int RangeInt(int _min, int _max);



	private:
		static void Init();	// 初期化関数
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


