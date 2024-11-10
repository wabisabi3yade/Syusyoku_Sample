#include "pch.h"
#include "CP_TestCollider.h"


CP_TestCollider::CP_TestCollider() : isGra(true), hei(0.0f)
{
}

CP_TestCollider::~CP_TestCollider()
{
	//DX11BulletPhisics::GetInstance()->RemoveCollObj(*rigid);

	CLASS_DELETE(shape);
	CLASS_DELETE(mot);
	CLASS_DELETE(rigid);
}

void CP_TestCollider::Init()
{
	
}

void CP_TestCollider::Start()
{
	shape = new btBoxShape(btVector3(1.0f / 2, 1.0f / 2, 1.0f / 2));
	btScalar mass = 1.0f;

	if (!isGra)
	{
		mass = 0.0f;
	}

	// 移動するオブジェクトかどうか
	bool is_dynamic = (mass != 0.0f);

	// 慣性モーメント
	btVector3 inertia(0, 0, 0);
	if (is_dynamic) {
		shape->calculateLocalInertia(mass, inertia);
	}

	// 剛体操作
	// 剛体位置の設定/取得など

	btTransform transform;
	transform.setIdentity();  // 単位行列（回転をリセット）
	transform.setOrigin(btVector3(0, 10, hei));  // 中心を(0, 5, 0)に移動
	mot = new btDefaultMotionState(transform);

	// 剛体作成
	btRigidBody::btRigidBodyConstructionInfo rb_info(mass, mot, shape, inertia);
	rigid = new btRigidBody(rb_info);

	// 物理ワールドに追加
	//DX11BulletPhisics::GetInstance()->AddCollObj(*rigid, 0);
}

void CP_TestCollider::SetIsGravity(bool a)
{
	isGra = a;
}

void CP_TestCollider::ImGuiDebug()
{
	ImGui::Checkbox("a", &isGra);

	ImGui::DragFloat("height", &hei);
}

nlohmann::json CP_TestCollider::Save()
{
	auto data = Component::Save();
	data["a"] = isGra;
	data["hei"] = hei;
	return data;
}

void CP_TestCollider::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	isGra = _data["a"];
	HashiTaku::LoadJsonFloat("hei", hei, _data);
}


