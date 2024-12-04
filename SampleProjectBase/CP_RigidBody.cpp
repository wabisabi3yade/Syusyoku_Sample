#include "pch.h"
#include "CP_RigidBody.h"
#include "CP_Collider.h"
#include "InSceneSystemManager.h"

using namespace HashiTaku;
namespace DXSimp = DirectX::SimpleMath;

constexpr float SLEEP_LINEAR(0.01f);	// これ以下の移動速度なら静的オブジェクトに変更する
constexpr float SLEEP_ANGLE(0.01f);	// これ以下の回転速度なら静的オブジェクトに変更する

CP_RigidBody::CP_RigidBody()
	: mass(1.0f), friction(0.5f), isAwaking(false), isTrigger(false), isKinematic(false)
{
	isFreezeRotation = { false, false, false };
}

void CP_RigidBody::Init()
{
	// コライダー系コンポーネントがあるか探す
	FindSetCollider();
}

void CP_RigidBody::OnDestroy()
{
	DX11BulletPhisics* pEngine = DX11BulletPhisics::GetInstance();

	if (collider)
		pEngine->RemoveCollObj(*this);
}

void CP_RigidBody::OnEnableTrue()
{
	// ワールドに追加
	if (!collider) return;
	DX11BulletPhisics::GetInstance()->AddCollObj(*this, 0);
}

void CP_RigidBody::OnEnableFalse()
{
	if (!collider) return;
	DX11BulletPhisics::GetInstance()->RemoveCollObj(*this);
}

void CP_RigidBody::SetColliderShape(CP_Collider& _setCollider)
{
	if (collider) return;

	// 衝突パラメータを作成
	collider = std::make_unique<CollPair>();
	collider->pColliderComp = &_setCollider;
	// 衝突判定タイプを作成
	collider->pColTypeJudge = std::make_unique<CollisionTypeJudge>();

	if (isTrigger)
		CreateGhost();
	else
		CreateRigidBody();
}

void CP_RigidBody::RemoveColliderShape(CP_Collider& _removeCollider)
{
	if (!collider) return;
	if (collider->pColliderComp != &_removeCollider) return;

	// ワールド空間からこの当たり判定を削除する
	DX11BulletPhisics::GetInstance()->RemoveCollObj(*this);
	collider.reset();
}

void CP_RigidBody::SetMass(float _mass)
{
	// 質量をセット
	mass = _mass;

	if (!collider || isTrigger) return;

	// 慣性を再計算する
	bool isDynamic = mass >= Mathf::epsilon;
	btCollisionShape& shape = *collider->pCollisionObject->getCollisionShape();
	collider->inertia.setZero();
	if (mass != 0.0f)
	{
		btVector3 localInertia;
		shape.calculateLocalInertia(mass, localInertia);  // 慣性モーメントを計算
		collider->inertia = localInertia;
	}
	btRigidBody& rigid = CastRigidBody();

	// 代入
	rigid.setMassProps(mass, collider->inertia);
	rigid.updateInertiaTensor();
}

void CP_RigidBody::SetFriction(float _friction)
{
	friction = std::max(_friction, 0.0f);

	if (collider && !isTrigger)
		CastRigidBody().setFriction(friction);
}

void CP_RigidBody::SetIsAwake(bool _isAwake)
{
	isAwaking = _isAwake;

	if (!collider) return;

	// Awake状態に剛体を合わせる
	if (isAwaking)
		collider->pCollisionObject->setActivationState(DISABLE_DEACTIVATION);
	else
		ReCreateCollider();
}

void CP_RigidBody::SetIsTrigger(bool _isTrigger)
{
	isTrigger = _isTrigger;
	ReCreateCollider();
}

void CP_RigidBody::SetIsKinematic(bool _isKinematic)
{
	isKinematic = _isKinematic;

	if (!collider || isTrigger) return;

	btRigidBody& btRigid = CastRigidBody();
	if (isKinematic)
	{
		// キネマティックに変更
		btRigid.setCollisionFlags(btRigid.getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		btRigid.setActivationState(DISABLE_DEACTIVATION);
	}
	else
	{
		// キネマティックを解除
		btRigid.setCollisionFlags(btRigid.getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
	}
}

void CP_RigidBody::SetToBtTransform()
{
	assert(collider && "コライダーがありません");

	Transform& transform = GetTransform();
	btVector3 dxOrigin = Bullet::ToBtVector3(transform.GetPosition());
	btQuaternion rot = Bullet::ToBtQuaeternion(transform.GetRotation());

	btTransform bulleTransform;
	bulleTransform.setOrigin(dxOrigin);
	bulleTransform.setRotation(rot);

	if (isTrigger)	// すり抜けなら
	{
		collider->pCollisionObject->setWorldTransform(bulleTransform);
	}
	else
	{
		btRigidBody& rigidBody = CastRigidBody();
		btVector3 linearVelocity = rigidBody.getLinearVelocity();
		btVector3 angularVelocity = rigidBody.getAngularVelocity();

		// DirectX側のワールド行列から新しい位置を取得
		rigidBody.getMotionState()->setWorldTransform(bulleTransform);
		rigidBody.setCenterOfMassTransform(bulleTransform);
		rigidBody.setLinearVelocity(linearVelocity);
		rigidBody.setAngularVelocity(angularVelocity);
	}
}

void CP_RigidBody::SetToDXTransform()
{
	btTransform bulletTransform;
	GetBtTransform(bulletTransform);

	Transform& dxTransform = GetTransform();
	dxTransform.SetPosition(Bullet::ToDXVector3(bulletTransform.getOrigin()));
	dxTransform.SetRotation(Bullet::ToDXQuaternion(bulletTransform.getRotation()));
}

void CP_RigidBody::SetVelocity(const DirectX::SimpleMath::Vector3& _velocity)
{
	if (!collider || isTrigger) return;

	// 速度を代入
	CastRigidBody().setLinearVelocity(Bullet::ToBtVector3(_velocity));
}

CollisionTypeJudge& CP_RigidBody::GetColTypeJudge() const
{
	assert(collider && "colliderが作成されていません");
	return *collider->pColTypeJudge;
}

DirectX::SimpleMath::Vector3 CP_RigidBody::GetVelocity() const
{
	if (!collider || isTrigger) return DXSimp::Vector3::Zero;

	btRigidBody& btRb = static_cast<btRigidBody&>(*collider->pCollisionObject);
	return Bullet::ToDXVector3(btRb.getLinearVelocity());
}

void CP_RigidBody::ImGuiDebug()
{
	if (ImGui::Button("SetShape"))
		FindSetCollider();

	// 質量
	float imFloat = mass;
	if (ImGui::DragFloat("Mass", &imFloat, 0.01f, 0.0f, 1000.0f))
		SetMass(imFloat);

	imFloat = friction;
	if (ImGui::DragFloat("Friction", &imFloat, 0.01f, 0.0f, 1000.0f))
		SetFriction(imFloat);

	// 静的オブジェクトにしない
	bool isImBool = isAwaking;
	if (ImGui::Checkbox("IsAwake", &isImBool))
		SetIsAwake(isImBool);

	isImBool = isTrigger;
	if (ImGui::Checkbox("IsTrigger", &isImBool))
		SetIsTrigger(isImBool);

	isImBool = isKinematic;
	if (ImGui::Checkbox("IsKinematic", &isImBool))
		SetIsKinematic(isImBool);

	ImGuiFreezeRot();
}

nlohmann::json CP_RigidBody::Save()
{
	auto data = Component::Save();
	data["mass"] = mass;
	data["friction"] = friction;
	data["isAwake"] = isAwaking;
	data["isTrigger"] = isTrigger;
	data["isKinematic"] = isKinematic;

	for (short a_i = 0; a_i < AXIS_CNT; a_i++)
	{
		data["isFreezeRot"][a_i] = isFreezeRotation[a_i];
	}

	return data;
}

void CP_RigidBody::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadJsonFloat("mass", mass, _data);
	LoadJsonFloat("friction", friction, _data);
	LoadJsonBoolean("isAwake", isAwaking, _data);
	LoadJsonBoolean("isTrigger", isTrigger, _data);
	LoadJsonBoolean("isKinematic", isKinematic, _data);

	if (HashiTaku::IsJsonContains(_data, "isFreezeRot"))
	{
		for (short a_i = 0; a_i < AXIS_CNT; a_i++)
			isFreezeRotation[a_i] = _data["isFreezeRot"][a_i];
	}

	ReCreateCollider();
}

void CP_RigidBody::UpdateFreezeRotation()
{
	if (isTrigger) return;

	btVector3 axisFreezeVal;
	axisFreezeVal.setZero();

	// 固定していない軸は回転するように設定
	for (short a_i = 0; a_i < AXIS_CNT; a_i++)
	{
		if (!isFreezeRotation[a_i])
			axisFreezeVal[a_i] = 1;
	}

	// セットする
	CastRigidBody().setAngularFactor(axisFreezeVal);
}

void CP_RigidBody::GetBtTransform(btTransform& _btTrans)
{
	if (!collider) return;
	_btTrans = collider->pCollisionObject->getWorldTransform();
}

void CP_RigidBody::ImGuiFreezeRot()
{
	ImGui::Text("FreezeRot");
	if (ImGui::Checkbox("X", &isFreezeRotation[0]))
	{
		UpdateFreezeRotation();
		return;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Y", &isFreezeRotation[1]))
	{
		UpdateFreezeRotation();
		return;
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Z", &isFreezeRotation[2]))
	{
		UpdateFreezeRotation();
	}
}

btCollisionObject& CP_RigidBody::GetCollisionObject()
{
	assert(collider && "colliderがないので取得できません");
	return *collider->pCollisionObject;
}

void CP_RigidBody::FindSetCollider()
{
	CP_Collider* pCol = GetGameObject().GetComponent<CP_Collider>();
	if (!pCol) return;
	if (!pCol->GetIsCreateCompound()) return;

	SetColliderShape(*pCol);
}

void CP_RigidBody::CreateRigidBody()
{
	btTransform bulletTrans;
	CastDxToBtTransform(bulletTrans);

	bool isDynamic = mass > Mathf::epsilon;

	// 慣性を計算する
	collider->inertia.setZero();
	if (isDynamic)
		collider->pColliderComp->GetColliderShape().calculateLocalInertia(mass, collider->inertia);

	collider->pMotionState = std::make_unique<btDefaultMotionState>(bulletTrans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(
		Bullet::ToBtScalar(mass),
		collider->pMotionState.get(),
		&collider->pColliderComp->GetColliderShape(),
		collider->inertia);
	collider->pCollisionObject = std::make_unique<btRigidBody>(rbInfo);

	btRigidBody& btRigid = CastRigidBody();
	btRigid.setFriction(friction);
	btRigid.setDamping(0.9f, 0.9f);  // 線形ダンピングと回転ダンピングを追加
	btRigid.setRestitution(0.0f);


	if (isAwaking)	// 常に計算するならスリーブ状態にしない
		collider->pCollisionObject->setActivationState(DISABLE_DEACTIVATION);

	// キネマティック
	SetIsKinematic(isKinematic);

	// 回転軸固定を適用する
	UpdateFreezeRotation();

	// 共通処理
	CommonCreateColObj();
}

void CP_RigidBody::CreateGhost()
{
	btTransform bulletTrans;
	CastDxToBtTransform(bulletTrans);

	// 判定のみの衝突オブジェクトを作成
	std::unique_ptr<btGhostObject> createGhost = std::make_unique<btGhostObject>();
	createGhost->setCollisionShape(&collider->pColliderComp->GetColliderShape());

	// 位置設定
	createGhost->setWorldTransform(bulletTrans);
	createGhost->setCollisionFlags(createGhost->getCollisionFlags()
		| btCollisionObject::CF_NO_CONTACT_RESPONSE);

	collider->pCollisionObject = std::move(createGhost);
	// 共通処理
	CommonCreateColObj();
}

void CP_RigidBody::CommonCreateColObj()
{
	// RigidBodyコンポーネントをセット
	collider->pCollisionObject->setUserPointer(this);

	// ワールドに追加
	DX11BulletPhisics::GetInstance()->AddCollObj(*this, 0);
}

btRigidBody& CP_RigidBody::CastRigidBody()
{
	assert(collider && "コライダーが作成されていません");
	assert(!isTrigger && "RigidBodyではありません");

	return static_cast<btRigidBody&>(*collider->pCollisionObject);
}

void CP_RigidBody::CastDxToBtTransform(btTransform& _outBtTransform)
{
	Transform& transform = GetTransform();
	btVector3 origin = Bullet::ToBtVector3(transform.GetPosition());
	btQuaternion rot = Bullet::ToBtQuaeternion(transform.GetRotation());
	_outBtTransform.setIdentity();
	_outBtTransform.setOrigin(origin);
	_outBtTransform.setRotation(rot);
}

void CP_RigidBody::ReCreateCollider()
{
	if (!collider) return;

	CP_Collider& colliderComp = *collider->pColliderComp;

	// 既にあるならワールドから削除する
	RemoveColliderShape(colliderComp);

	// 現在のパラメータで作り直す
	SetColliderShape(colliderComp);
}
