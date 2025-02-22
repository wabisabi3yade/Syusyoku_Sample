#pragma once
#include "Component.h"

namespace HashiTaku
{
	/// @brief ポストエフェクト
	class CP_PostEffect : public Component
	{
	public:
		/// @brief ポストエフェクトの種類
		enum class PostEffectType
		{
			Blur,	// ブラー
			Bloom,	// ブルーム
			MaxNum
		};

	private:
		/// @brief ポストエフェクトの種類
		PostEffectType postEffectType;

	public:
		CP_PostEffect(PostEffectType _postType);
		virtual ~CP_PostEffect() {}

		/// @brief ポストエフェクトのタイプを取得する
		/// @return ポストエフェクトのタイプ
		PostEffectType GetPostEffectType() const;
	};
}
