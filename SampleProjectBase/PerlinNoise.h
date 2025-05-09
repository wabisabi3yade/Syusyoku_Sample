#pragma once

namespace HashiTaku
{
	// 参考コード:https://qiita.com/gis/items/ba7d715901a0e572b0e9
	// 解説サイト:https://postd.cc/understanding-perlin-noise/

	/// @brief パーリンノイズ生成
	class PerlinNoise
	{
		using Pint = std::uint_fast8_t;

		std::array<Pint, 512> p{ {} };

	public:
		PerlinNoise() = default;
		/// @brief コンストラクタ
		/// @param _seed シード値
		explicit PerlinNoise(const std::uint_fast32_t _seed);
		~PerlinNoise() {}

		/// @brief シード値をセット
		/// @param _seed シード値
		void SetSeed(const std::uint_fast32_t _seed);

		/// @brief オクターブ無しノイズを取得する
		/// @return 
	
		/// @brief オクターブ無しノイズを取得する
		/// @param _x X
		/// @param _y Y
		/// @param _z Z
		/// @return オクターブ無しノイズ
		double Noise(double _x = 0.0, double _y = 0.0, double _z = 0.0) const noexcept;

	private:
		constexpr double GetFade(const double _t) const noexcept;

		constexpr double GetLerp(const double _t,
			const double _a,
			const double _b) const noexcept;


		constexpr double MakeGrad(const Pint _hash,
			const double _u,
			const double _v) const noexcept;

		constexpr double MakeGrad(const Pint _hash, 
			const double _x,
			const double _y,
			const double _z) const noexcept;

		constexpr double GetGrad(const Pint _hash,
			const double _x,
			const double _y,
			const double _z) const noexcept;

		double SetNoise(double _x = 0.0, double _y = 0.0, double _z = 0.0) const noexcept;
	};
}

