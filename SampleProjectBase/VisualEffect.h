#pragma once
#include "AssetPath_Base.h"

namespace HashiTaku
{
	/// @brief エフェクトクラス
	class VisualEffect : public AssetPath_Base
	{
		/// @brief エフェクト色
		DXSimp::Color vfxColor;

		/// @brief  エフェクシアで再生するエフェクト変数
		Effekseer::EffectRef effect;

		/// @brief ロード時のスケール値
		float loadScale;
	public:
		VisualEffect();
		/// @brief コンストラクタ
		/// @param _effect エフェクシアで再生するエフェクト
		/// @param _loadScale ロード時のスケール
		VisualEffect(const Effekseer::EffectRef& _effect, float _loadScale);
		~VisualEffect();

		/// @brief エフェクト色をセット
		/// @param _color 色
		void SetVfxColor(const DXSimp::Color& _color);

		/// @brief エフェクシアで再生するエフェクトを取得
		/// @return エフェクト
		const Effekseer::EffectRef& GetEffect() const;

		/// @brief エフェクト色を取得
		/// @return 色
		const DXSimp::Color& GetVfxColor() const;

		/// @brief セーブする
		/// @param _sceneData セーブデータ
		json Save() override;

		/// @brief ロードする
		/// @param _sceneData ロードするデータ 
		void Load(const json& _data) override;
	};
}