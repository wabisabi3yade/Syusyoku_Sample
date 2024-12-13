#pragma once
#include "SingletonComponent.h"
#include "CP_VisualEffect.h"

/// @brief �G�t�F�N�g�쐬�N���X
class CP_EffectCreater : public SingletonComponent<CP_EffectCreater>
{
	friend class SingletonComponent<CP_EffectCreater>;

	// �G�t�F�N�g�}�l�[�W���[
	DX11EffecseerManager* pEffectManager;

public:
	void Init() override;

	/// @brief �G�t�F�N�g���쐬
	/// @param _vfxName �G�t�F�N�g�̖��O
	/// @param _playSpeed �Đ����x
	/// @param _pos ���W
	/// @param _scale �X�P�[��
	/// @param _angle �p�x
	void Create(const std::string& _vfxName,
		float _playSpeed,
		const DirectX::SimpleMath::Vector3& _pos,
		const DirectX::SimpleMath::Vector3& _scale = { 1.0f, 1.0f, 1.0f },
		const DirectX::SimpleMath::Vector3& _angle = { 0.0f, 0.0f, 0.0f });
};

