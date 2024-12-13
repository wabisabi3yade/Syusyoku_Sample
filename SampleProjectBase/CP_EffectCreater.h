#pragma once
#include "SingletonComponent.h"
#include "CP_VisualEffect.h"

/// @brief エフェクト作成クラス
class CP_EffectCreater : public SingletonComponent<CP_EffectCreater>
{
	friend class SingletonComponent<CP_EffectCreater>;

	// エフェクトマネージャー
	DX11EffecseerManager* pEffectManager;

public:
	void Init() override;

	/// @brief エフェクトを作成
	/// @param _vfxName エフェクトの名前
	/// @param _playSpeed 再生速度
	/// @param _pos 座標
	/// @param _scale スケール
	/// @param _angle 角度
	void Create(const std::string& _vfxName,
		float _playSpeed,
		const DirectX::SimpleMath::Vector3& _pos,
		const DirectX::SimpleMath::Vector3& _scale = { 1.0f, 1.0f, 1.0f },
		const DirectX::SimpleMath::Vector3& _angle = { 0.0f, 0.0f, 0.0f });
};

