#pragma once
#include "Component.h"

#include "DX11BulletPhisics.h"
class CP_TestCollider : public Component
{
	btCollisionShape* shape{ nullptr };
	btDefaultMotionState* mot{ nullptr };
	btRigidBody* rigid{ nullptr };

	float hei;
	float mass;

	bool isGra;
public:
	CP_TestCollider();
	~CP_TestCollider();

	void Init() override;

	void Start() override;

	void SetIsGravity(bool a);

	void ImGuiDebug() override;

	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
};

