#pragma once
#include "CP_Enemy.h"
#include "BossActionController.h"


/// @brief �{�X�̃R���|�[�l���g
class CP_Boss : public CP_Enemy
{
	std::unique_ptr<BossActionController> pActionController;

	/// @brief �A�j���[�V����
	CP_Animation* pAnimation;
	
	float hitStopBeforeAnimSpeed;
public:
	CP_Boss();
	~CP_Boss() {}

	void Init() override;

	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
private:
	void Awake() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	/// @brief �X�V�ł��邩�H
	/// @return �X�V�ł��邩�H
	bool GetCanUpdate();

	/// @brief �q�b�g�X�g�b�v�J�n����
	void OnHitStopBegin() override;

	/// @brief �q�b�g�X�g�b�v�I������
	void OnHitStopEnd() override;

	void ImGuiDebug() override;
};

