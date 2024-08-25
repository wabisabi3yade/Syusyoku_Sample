#include "pch.h"
#include "Transform.h"
#include "NullTransform.h"

#include "InSceneSystemManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace HashiTaku;

Transform::Transform(bool _isInit)
	: pParent(nullptr), scale(Vector3::One), localScale(Vector3::One), up(Vec3::Up), right(Vec3::Right), forward(Vec3::Forward), isHaveParent(false)
{
	if (_isInit)
		pParent = new NullTransform();
}

Transform::~Transform()
{
	// 親トランスフォームがNullTransformなら
	if (!isHaveParent)
		CLASS_DELETE(pParent);
}

void Transform::UpdateVector()
{
	// クォータニオンから回転行列を求める
	Matrix rotateMatrix(Matrix::CreateFromQuaternion(rotation));

	// 方向ベクトルを更新
	right = Mtx::GetRightVector(rotateMatrix);
	up = Mtx::GetUpVector(rotateMatrix);
	forward = Mtx::GetForwardVector(rotateMatrix);

	/*forward = XMVector3Rotate(Vec3::Forward, rotation);
	right = XMVector3Rotate(Vec3::Right , rotation);
	up = XMVector3Rotate(Vec3::Up, rotation);*/
}

void Transform::LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector)
{
	Vector3 vec = _worldPos - position;
	Quaternion q = Quat::RotateToVector(vec);

	SetRotation(q);
}

void Transform::RemoveParent()
{
	pParent = new NullTransform();
	isHaveParent = false;
}

void Transform::SetName(const std::string& _name)
{
	name = _name;
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
	Vector3 diffPos = _child.position - position;
	_child.SetLocalPos(diffPos);

	Vector3 diffScale = _child.scale / scale;
	_child.SetLocalScale(diffScale);

	Quaternion invRot;
	rotation.Inverse(invRot);
	Quaternion diffRot = Quat::Multiply(_child.rotation, invRot);
	_child.SetLocalRotation(diffRot);
}

void Transform::SetPosition(const DirectX::SimpleMath::Vector3& _pos)
{
	position = _pos;
	localPosition = position - pParent->position;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();
}

void Transform::SetScale(const DirectX::SimpleMath::Vector3& _scale)
{
	scale = _scale;
	localScale = scale / pParent->scale;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();
}

void Transform::SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	eularAngles = _eularAngles;
	localEularAngles = eularAngles - pParent->eularAngles;

	// クォータニオンに反映させる
	rotation = Quat::ToQuaternion(eularAngles);
	localRotation = Quat::RotationDifference(pParent->rotation, rotation);

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}

void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	rotation = _quaternion;
	localRotation = Quat::RotationDifference(pParent->rotation, rotation);

	eularAngles = Quat::ToEulerAngles(rotation);
	localEularAngles = eularAngles - pParent->eularAngles;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}


void Transform::SetLocalPos(const DirectX::SimpleMath::Vector3& _localPos)
{
	localPosition = _localPos;
	position = pParent->position;
	position += pParent->Right() * localPosition.x * pParent->scale.x;
	position += pParent->Up() * localPosition.y * pParent->scale.y;
	position += pParent->Forward() * localPosition.z * pParent->scale.z;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyPositions();
}

void Transform::SetLocalScale(const DirectX::SimpleMath::Vector3& _scale)
{
	localScale = _scale;
	scale = pParent->scale * localScale;

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyScales();
}

void Transform::SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	localEularAngles = _eularAngles;

	// クォータニオンに反映させる
	localRotation = Quat::ToQuaternion(localEularAngles);

	eularAngles = pParent->eularAngles + localEularAngles;
	rotation = Quat::Multiply(pParent->rotation, localRotation);

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}

void Transform::SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	localRotation = _quaternion;

	// 角度に反映させる
	localEularAngles = Quat::ToEulerAngles(localRotation);

	eularAngles = pParent->eularAngles + localEularAngles;
	rotation = Quat::Multiply(localRotation, pParent->rotation);

	// 子トランスフォームに反映
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
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

std::string Transform::GetName() const
{
	return name;
}

u_int Transform::GetChilidCnt() const
{
	return static_cast<u_int>(pChilds.size());
}

nlohmann::json Transform::Save()
{
	nlohmann::json transformData;

	SaveJsonVector3("pos", position, transformData);
	SaveJsonVector3("scale", scale, transformData);
	SaveJsonVector3("angle", eularAngles, transformData);

	transformData["havePare"] = isHaveParent;

	// 親子関係は名前をセーブする
	transformData["parent"] = pParent->GetName();

	auto& childData = transformData["child"];
	for (auto& child : pChilds)
	{
		childData.push_back(child->GetName());
	}

	return transformData;
}

void Transform::Load(const nlohmann::json& _transformData)
{
	Vector3 v;
	LoadJsonVector3("pos", v, _transformData);
	SetPosition(v);

	LoadJsonVector3("scale", v, _transformData);
	SetScale(v);

	LoadJsonVector3("angle", v, _transformData);
	SetEularAngles(v);

	LoadJsonBoolean("havePare", isHaveParent, _transformData);

	// 親子トランスフォーム取得
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->GetSceneObjects();
	if (isHaveParent && IsJsonContains(_transformData, "parent"))
	{
		std::string parentName;
		LoadJsonString("parent", parentName, _transformData);
		GameObject* go = sceneObjects.GetSceneObject(parentName);

		if (go)
		{
			pParent = &go->transform;
		}
	}

	if (IsJsonContains(_transformData, "child"))
	{
		const auto& childData = _transformData["child"];
		u_int childNum = static_cast<u_int>(childData.size());
		for (u_int c_i = 0; c_i < childNum; c_i++)
		{
			GameObject* go = sceneObjects.GetSceneObject(childData[c_i]);

			if (!go) continue;
			pChilds.push_back(&go->transform);
		}
	}
	
}

void Transform::UpdateHierarchyPositions()
{
	position = pParent->position;
	position += pParent->Right() * localPosition.x;
	position += pParent->Up() * localPosition.y;
	position += pParent->Forward() * localPosition.z;

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
	rotation = Quat::Multiply(localRotation, pParent->rotation);
	eularAngles = pParent->eularAngles + localEularAngles;

	// 再帰で呼び出す
	for (auto& child : pChilds)
		child->UpdateHierarchyRotations();
}