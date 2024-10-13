#include "Transform.h"
#include "NullTransform.h"

#include "InSceneSystemManager.h"

#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace HashiTaku;

Transform::Transform(GameObject* _pGameObject, bool _isInit)
	: pParent(nullptr), scale(Vector3::One), localScale(Vector3::One), up(Vec3::Up), right(Vec3::Right), forward(Vec3::Forward), isHaveParent(false)
{
	if (_isInit)
		pParent = new NullTransform();

	pGameObject = _pGameObject;
}

Transform::Transform(const Transform& _other)
	: pParent(nullptr), scale(Vector3::One), localScale(Vector3::One), up(Vec3::Up), right(Vec3::Right), forward(Vec3::Forward), isHaveParent(false)
{
	Copy(_other);
}

Transform::~Transform()
{
	// 親トランスフォームがNullTransformなら
	if (!isHaveParent)
		CLASS_DELETE(pParent);
}

Transform& Transform::operator=(const Transform& _other)
{
	Copy(_other);

	return *this;
}

void Transform::UpdateVector()
{
	// クォータニオンから回転行列を求める
	Matrix rotateMatrix(Matrix::CreateFromQuaternion(rotation));

	// 方向ベクトルを更新
	right = Mtx::GetRightVector(rotateMatrix);
	up = Mtx::GetUpVector(rotateMatrix);
	forward = Mtx::GetForwardVector(rotateMatrix);
}

void Transform::LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector)
{
	Vector3 vec = _worldPos - position;
	Quaternion q = Quat::RotateToVector(vec);

	SetRotation(q);
}

void Transform::RemoveChild(Transform& _removeTransform)
{
	pChilds.remove(&_removeTransform);
	_removeTransform.RemoveParent();
}

void Transform::SetParent(Transform& _parent)
{
	_parent.SetChild(*this);
}

void Transform::SetChild(Transform& _child)
{
	if (std::find(pChilds.begin(), pChilds.end(), &_child) != pChilds.end())
	{
		HASHI_DEBUG_LOG("既に子オブジェクトです");
		return;
	}

	pChilds.push_back(&_child);

	if (!_child.isHaveParent)	// 親トランスフォームがnullなら
		CLASS_DELETE(_child.pParent);

	_child.pParent = this;
	_child.isHaveParent = true;

	// ローカルパラメータに反映
	Vector3 diffPos = (_child.position - position) / GetScale();
	_child.SetLocalPosition(diffPos);

	Vector3 diffScale = _child.scale / scale;
	_child.SetLocalScale(diffScale);

	Quaternion diffRot = Quat::RotationDifference(_child.GetRotation(), pParent->GetRotation());
	_child.SetLocalRotation(diffRot);
}

void Transform::SetPosition(const DirectX::SimpleMath::Vector3& _pos)
{
	position = _pos;
	localPosition = position - pParent->position;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}

void Transform::SetScale(const DirectX::SimpleMath::Vector3& _scale)
{
	scale = _scale;
	localScale = scale / pParent->scale;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}

void Transform::SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	eularAngles = _eularAngles;
	localEularAngles = eularAngles - pParent->eularAngles;

	// クォータニオンに反映させる
	rotation = Quat::ToQuaternion(eularAngles);
	Quaternion a = pParent->GetRotation();
	a = XMQuaternionInverse(a);
	localRotation = DirectX::XMQuaternionMultiply(rotation, a);

	//// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}

void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	rotation = _quaternion;
	Quaternion a = pParent->GetRotation();
	a =  XMQuaternionInverse(a);
	localRotation = DirectX::XMQuaternionMultiply(rotation, a);

	eularAngles = Quat::ToEulerAngles(rotation);
	localEularAngles = eularAngles - pParent->GetEularAngles();

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}


void Transform::SetLocalPosition(const DirectX::SimpleMath::Vector3& _localPos)
{
	localPosition = _localPos;
	position = pParent->position;
	position += pParent->Right() * localPosition.x * pParent->scale.x;
	position += pParent->Up() * localPosition.y * pParent->scale.y;
	position += pParent->Forward() * localPosition.z * pParent->scale.z;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}

void Transform::SetLocalScale(const DirectX::SimpleMath::Vector3& _scale)
{
	localScale = _scale;
	scale = pParent->scale * localScale;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}

void Transform::SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	localEularAngles = _eularAngles;
	// クォータニオンに反映させる
	localRotation = Quat::ToQuaternion(localEularAngles);

	eularAngles = pParent->eularAngles + localEularAngles;
	rotation = Quat::Multiply(localRotation, pParent->GetRotation());

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}

void Transform::SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	localRotation = _quaternion;

	rotation = Quat::Multiply(localRotation, pParent->GetRotation());
	eularAngles = Quat::ToEulerAngles(rotation);
	localEularAngles = eularAngles - pParent->GetEularAngles();

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();

	// オブジェクト側に変更したことを伝える
	pGameObject->OnChangeTransform();
}

void Transform::SetLoadPosition(const DirectX::SimpleMath::Vector3& _position)
{
	localPosition = _position;
	position = pParent->position;
	position += pParent->Right() * localPosition.x * pParent->scale.x;
	position += pParent->Up() * localPosition.y * pParent->scale.y;
	position += pParent->Forward() * localPosition.z * pParent->scale.z;
}


void Transform::SetGameObject(GameObject& _go)
{
	pGameObject = &_go;
}

DirectX::SimpleMath::Vector3 Transform::GetPosition() const
{
	return position;
}

DirectX::SimpleMath::Vector3 Transform::GetScale() const
{
	return scale;
}

DirectX::SimpleMath::Vector3 Transform::GetEularAngles() const
{
	return eularAngles;
}

DirectX::SimpleMath::Quaternion Transform::GetRotation() const
{
	return rotation;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalPosition() const
{
	return localPosition;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalScale() const
{
	return localScale;
}

DirectX::SimpleMath::Vector3 Transform::GetLocalEularAngles() const
{
	return localEularAngles;
}

DirectX::SimpleMath::Quaternion Transform::GetLocalRotation() const
{
	return localRotation;
}

GameObject& Transform::GetGameObject()
{
	return *pGameObject;
}

Transform* Transform::GetParent()
{
	if (!isHaveParent)
		return nullptr;

	return pParent;
}

Transform* Transform::GetChild(u_int _childIdx)
{
	if (_childIdx >= GetChildCnt()) return nullptr;

	auto itr = pChilds.begin();
	for (u_int i = 0; i < _childIdx; i++)
		itr++;

	return (*itr);
}

u_int Transform::GetChildCnt() const
{
	return static_cast<u_int>(pChilds.size());
}

nlohmann::json Transform::Save()
{
	nlohmann::json transformData;

	SaveJsonVector3("pos", position, transformData);
	SaveJsonVector3("scale", scale, transformData);
	rotation.Normalize();
	SaveJsonVector4("rotation", rotation, transformData);
	auto& childData = transformData["child"];
	for (auto& child : pChilds)
	{
		childData.push_back(child->GetGameObject().GetName());
	}

	return transformData;
}

void Transform::Load(const nlohmann::json& _transformData)
{
	Quaternion rot = Quaternion::Identity;
	LoadJsonQuaternion("rotation", rot, _transformData);
	SetRotation(rot);
	UpdateVector();

	Vector3 v;
	LoadJsonVector3("pos", v, _transformData);
	SetPosition(v);

	if (pGameObject->GetName() == "Weapon_Model")
	{
		v = { 0.3f, 0.08f, 0.05f };
		SetLocalPosition(v);
	}

	LoadJsonVector3("scale", v, _transformData);
	SetScale(v);
}

void Transform::LoadChildTransform(const nlohmann::json& _transformData)
{
	// 親子トランスフォーム取得
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();

	if (IsJsonContains(_transformData, "child"))
	{
		const auto& childData = _transformData["child"];
		u_int childNum = static_cast<u_int>(childData.size());
		for (u_int c_i = 0; c_i < childNum; c_i++)
		{
			GameObject* go = sceneObjects.GetSceneObject(childData[c_i]);

			if (!go) continue;
			SetChild(go->GetTransform());
		}
	}
}

void Transform::Copy(const Transform& _other)
{
	if (this == &_other) return;

	position = _other.position;
	scale = _other.scale;
	rotation = _other.rotation;
}

void Transform::UpdateHierarchyPositions()
{
	position = pParent->position;
	position += pParent->Right() * localPosition.x * pParent->scale.x;
	position += pParent->Up() * localPosition.y * pParent->scale.y;
	position += pParent->Forward() * localPosition.z * pParent->scale.z;

	// 再帰で呼び出す
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();
}

void Transform::UpdateHierarchyScales()
{
	scale = pParent->scale * localScale;

	// 再帰で呼び出す
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();
}

void Transform::UpdateHierarchyRotations()
{
	std::string name = pGameObject->GetName();

	rotation = Quat::Multiply(localRotation, pParent->GetRotation());
	eularAngles = Quat::ToEulerAngles(rotation);

	// 再帰で呼び出す
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}

void Transform::RemoveParent()
{
	pParent = new NullTransform();
	isHaveParent = false;
}
