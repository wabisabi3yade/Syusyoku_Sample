#pragma once
#include "Component.h"

#include "PlayerAnimController.h"
#include "PlayerActionController.h"
#include "PlayerAnimObserver.h"

class CP_Player : public Component
{
	PlayerAnimController* pAnimController;

	/// @brief アクションコントローラー
	std::unique_ptr<PlayerActionController> pActionController;

	/// @brief アニメーションオブザーバー
	std::unique_ptr<PlayerAnimObserver> pAnimObserver;
public:
	CP_Player() {}
	CP_Player(const CP_Player& _other);
	~CP_Player() {}

	CP_Player& operator=(const CP_Player& _other);

	void Awake() override;

	void Start() override;

	void Update() override;

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Copy(const CP_Player& _other);
};

