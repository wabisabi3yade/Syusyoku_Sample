#pragma once
#include "AssetPath_Base.h"

/// @brief �G�t�F�N�g�N���X
class VisualEffect : public AssetPath_Base
{
	/// @brief  �G�t�F�N�V�A�ōĐ�����G�t�F�N�g�ϐ�
	Effekseer::EffectRef effect;

	/// @brief ���[�h���̃X�P�[���l
	float loadScale;
public:
	VisualEffect();
	/// @brief �R���X�g���N�^
	/// @param _effect �G�t�F�N�V�A�ōĐ�����G�t�F�N�g
	/// @param _loadScale ���[�h���̃X�P�[��
	VisualEffect(const Effekseer::EffectRef& _effect, float _loadScale);
	~VisualEffect();

	/// @brief �G�t�F�N�V�A�ōĐ�����G�t�F�N�g���擾
	/// @return �G�t�F�N�g
	const Effekseer::EffectRef& GetEffect() const;


	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) override;
};

