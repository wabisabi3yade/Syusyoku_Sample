#pragma once
#include "Component.h"
#include "VisualEffect.h"

/// @brief エフェクトを再生するコンポーネント
class CP_VisualEffect : public Component
{
	/// @brief 再生するエフェクト
	const VisualEffect* pVFX;

	/// @brief 再生するエフェクトのハンドル
	Effekseer::Handle vfxHandle;

	/// @brief 再生中か？
	bool isPlaying;
public:
	CP_VisualEffect();
	~CP_VisualEffect() {}

	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	void Start() override;

	/// @brief エフェクトを再生開始処理
	void BeginPlayVFX();

	/// @brief エフェクト再生停止する
	void StopPlayVFX();

	void ImGuiDebug() override;
};

