#pragma once
#include "AssetPath_Base.h"

/// @brief エフェクトクラス
class VisualEffect : public AssetPath_Base
{
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

	/// @brief エフェクシアで再生するエフェクトを取得
	/// @return エフェクト
	const Effekseer::EffectRef& GetEffect() const;


	/// @brief セーブする
	/// @param _sceneData セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _sceneData ロードするデータ 
	void Load(const nlohmann::json& _data) override;
};

