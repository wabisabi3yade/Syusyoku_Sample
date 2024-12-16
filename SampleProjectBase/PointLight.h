#pragma once
#include "Light.h"

namespace HashiTaku
{
	struct PointLParameter
	{
		LightParameter base;
		float range{ 5.0f };
		float dummy[3] = {};
	};

	// ポイントライト
	class PointLight :
		public Light
	{
	private:
		// 光の範囲
		float lightRange;

		void Copy(const PointLight& _other);
	public:
		PointLight() : lightRange(5.0f) { type = Type::Point; };
		PointLight(const PointLight& _other);
		~PointLight() {}

		PointLight& operator=(const PointLight& _other);

		/// @brief 範囲を設定
		/// @param _range 範囲
		void SetRange(float _range);

		/// @brief 光の範囲を取得する
		/// @return 光の範囲
		float GetRange() const;
		PointLParameter GetParameter();
	};
}