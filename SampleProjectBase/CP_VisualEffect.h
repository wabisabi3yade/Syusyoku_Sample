#pragma once
#include "Component.h"
#include "VisualEffect.h"

/// @brief �G�t�F�N�g���Đ�����R���|�[�l���g
class CP_VisualEffect : public Component
{
	/// @brief �Đ�����G�t�F�N�g
	const VisualEffect* pVFX;

	/// @brief �Đ�����G�t�F�N�g�̃n���h��
	Effekseer::Handle vfxHandle;

	/// @brief �Đ������H
	bool isPlaying;
public:
	CP_VisualEffect();
	~CP_VisualEffect() {}

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	void Start() override;

	/// @brief �G�t�F�N�g���Đ��J�n����
	void BeginPlayVFX();

	/// @brief �G�t�F�N�g�Đ���~����
	void StopPlayVFX();

	void ImGuiDebug() override;
};

