#pragma once
#include "Component.h"

#include "PlayerActionController.h"
#include "PlayerAnimObserver.h"

class CP_Animation;
class AnimationController;

class CP_Player : public Component
{
	/// @brief �A�j���[�V�����R���g���[���[
	CP_Animation* pAnimation;

	/// @brief �A�N�V�����R���g���[���[
	std::unique_ptr<PlayerActionController> pActionController;

	/// @brief �A�j���[�V�����I�u�U�[�o�[
	std::unique_ptr<PlayerAnimObserver> pAnimObserver;
public:
	CP_Player();
	CP_Player(const CP_Player& _other);
	~CP_Player() {}

	CP_Player& operator=(const CP_Player& _other);

	void Init() override;

	void Awake() override;

	void Start() override;

	void Update() override;

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Copy(const CP_Player& _other);
};

