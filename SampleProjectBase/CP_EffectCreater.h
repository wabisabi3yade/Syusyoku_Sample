#pragma once
#include "SingletonComponent.h"
#include "CP_VisualEffect.h"

/// @brief エフェクト作成クラス
class CP_EffectCreater : public SingletonComponent<CP_EffectCreater>
{
	friend class SingletonComponent<CP_EffectCreater>;

public:
	void Create(const std::string& _vfxName,
		const DirectX::SimpleMath::Vector3& _pos,
		const DirectX::SimpleMath::Vector3& _scale = { 1.0f, 1.0f, 1.0f },
		const DirectX::SimpleMath::Vector3& _angle = { 0.0f, 0.0f, 0.0f });
};

