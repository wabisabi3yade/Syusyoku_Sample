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

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;
};

