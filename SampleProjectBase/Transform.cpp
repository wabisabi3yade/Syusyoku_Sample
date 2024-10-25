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
	// �e�g�����X�t�H�[����NullTransform�Ȃ�
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
	// �N�H�[�^�j�I�������]�s������߂�
	Matrix rotateMatrix(Matrix::CreateFromQuaternion(rotation));

	// �����x�N�g�����X�V
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
	childTransforms.remove(&_removeTransform);
	_removeTransform.RemoveParent();
}

void Transform::SetParent(Transform& _parent)
{
	_parent.SetChild(*this);
}

void Transform::SetChild(Transform& _child)
{
	if (std::find(childTransforms.begin(), childTransforms.end(), &_child) != childTransforms.end())
	{
		HASHI_DEBUG_LOG("���Ɏq�I�u�W�F�N�g�ł�");
		return;
	}

	childTransforms.push_back(&_child);

	if (!_child.isHaveParent)	// �e�g�����X�t�H�[����null�Ȃ�
		CLASS_DELETE(_child.pParent);

	_child.pParent = this;
	_child.isHaveParent = true;

	// ���[�J���p�����[�^�ɔ��f
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

	// �e�I�u�W�F�N�g�̕�����K�p����O�̍��W
	Vector3 sub = position - pParent->GetPosition();
	Quaternion rot = pParent->GetRotation(); Quaternion invRot;
	rot.Inverse(invRot);
	localPosition = Vector3::Transform(sub, invRot);

	// �X�P�[���l���l������
	localPosition.x /= pParent->GetScale().x;
	localPosition.y /= pParent->GetScale().y;
	localPosition.z /= pParent->GetScale().z;

	/*localPosition = position - pParent->position;*/
	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyPositions();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangePosition();
}

void Transform::SetScale(const DirectX::SimpleMath::Vector3& _scale)
{
	scale = _scale;
	localScale = scale / pParent->scale;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyScales();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeScale();
}

void Transform::SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	eularAngles = _eularAngles;
	localEularAngles = eularAngles - pParent->GetEularAngles();

	// �N�H�[�^�j�I���ɔ��f������
	rotation = Quat::ToQuaternion(eularAngles);
	Quaternion a = pParent->GetRotation();
	a = XMQuaternionInverse(a);
	localRotation = DirectX::XMQuaternionMultiply(rotation, a);

	UpdateVector();

	//// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyRotations();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeRotation();
}

void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	rotation = _quaternion;
	Quaternion a = pParent->GetRotation();
	a =  XMQuaternionInverse(a);
	localRotation = DirectX::XMQuaternionMultiply(rotation, a);

	eularAngles = Quat::ToEulerAngles(rotation);
	localEularAngles = eularAngles - pParent->GetEularAngles();

	UpdateVector();

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyRotations();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeRotation();
}


void Transform::SetLocalPosition(const DirectX::SimpleMath::Vector3& _localPos)
{
	localPosition = _localPos;
	position = pParent->position;
	position += pParent->Right() * localPosition.x * pParent->scale.x;
	position += pParent->Up() * localPosition.y * pParent->scale.y;
	position += pParent->Forward() * localPosition.z * pParent->scale.z;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyPositions();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangePosition();
}

void Transform::SetLocalScale(const DirectX::SimpleMath::Vector3& _scale)
{
	localScale = _scale;
	scale = pParent->scale * localScale;

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyScales();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeScale();
}

void Transform::SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
{
	localEularAngles = _eularAngles;
	// �N�H�[�^�j�I���ɔ��f������
	localRotation = Quat::ToQuaternion(localEularAngles);

	eularAngles = pParent->GetEularAngles() + localEularAngles;
	rotation = Quat::Multiply(localRotation, pParent->GetRotation());

	UpdateVector();

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyRotations();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeRotation();
}

void Transform::SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
{
	localRotation = _quaternion;

	rotation = Quat::Multiply(localRotation, pParent->GetRotation());
	eularAngles = Quat::ToEulerAngles(rotation);
	localEularAngles = eularAngles - pParent->GetEularAngles();

	UpdateVector();

	// �q�g�����X�t�H�[���ɔ��f
	for (auto& child : childTransforms)
		child->UpdateHierarchyRotations();

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeRotation();
}

const DirectX::SimpleMath::Vector3& Transform::GetPosition() const
{
	return position;
}

const DirectX::SimpleMath::Vector3& Transform::GetScale() const
{
	return scale;
}

const DirectX::SimpleMath::Vector3& Transform::GetEularAngles() const
{
	return eularAngles;
}

const DirectX::SimpleMath::Quaternion& Transform::GetRotation() const
{
	return rotation;
}

const DirectX::SimpleMath::Matrix& Transform::GetWorldMatrix() const
{
	return worldMatrix;
}

const DirectX::SimpleMath::Vector3& Transform::GetLocalPosition() const
{
	return localPosition;
}

const DirectX::SimpleMath::Vector3& Transform::GetLocalScale() const
{
	return localScale;
}

const DirectX::SimpleMath::Vector3& Transform::GetLocalEularAngles() const
{
	return localEularAngles;
}

const DirectX::SimpleMath::Quaternion& Transform::GetLocalRotation() const
{
	return localRotation;
}

const DirectX::SimpleMath::Matrix& Transform::GetLocalMatrix() const
{
	return localMatrix;
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

	auto itr = childTransforms.begin();
	for (u_int i = 0; i < _childIdx; i++)
		itr++;

	return (*itr);
}

u_int Transform::GetChildCnt() const
{
	return static_cast<u_int>(childTransforms.size());
}

nlohmann::json Transform::Save()
{
	nlohmann::json transformData;

	SaveJsonVector3("pos", position, transformData);
	SaveJsonVector3("scale", scale, transformData);
	rotation.Normalize();
	SaveJsonVector4("rotation", rotation, transformData);
	auto& childData = transformData["child"];
	for (auto& child : childTransforms)
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

	LoadJsonVector3("scale", v, _transformData);
	SetScale(v);
}

void Transform::LoadChildTransform(const nlohmann::json& _transformData)
{
	// �e�q�g�����X�t�H�[���擾
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

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangePosition();

	// �ċA�ŌĂяo��
	for (auto& child : childTransforms)
		child->UpdateHierarchyPositions();
}

void Transform::UpdateHierarchyScales()
{
	scale = pParent->scale * localScale;

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeScale();

	// �ċA�ŌĂяo��
	for (auto& child : childTransforms)
		child->UpdateHierarchyScales();
}

void Transform::UpdateHierarchyRotations()
{
	std::string name = pGameObject->GetName();

	rotation = Quat::Multiply(localRotation, pParent->GetRotation());
	eularAngles = Quat::ToEulerAngles(rotation);

	// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
	pGameObject->OnChangeRotation();

	// �ċA�ŌĂяo��
	for (auto& child : childTransforms)
		child->UpdateHierarchyRotations();
}

void Transform::UpdateWorldMatrix(const DirectX::SimpleMath::Matrix& _parentWorldMtx)
{
	//// ���[���h�s����X�V
	//worldMatrix = localMatrix * _parentWorldMtx;

	//Mtx::GetTransformFromWldMtx(worldMatrix, position, scale, rotation);
	//eularAngles = rotation.ToEuler() * Mathf::radToDeg;

	//// �q�I�u�W�F�N�g�ɂ����f������
	//for (auto& c : childTransforms)
	//{
	//	c->UpdateWorldMatrix(worldMatrix);
	//}
}

void Transform::RemoveParent()
{
	pParent = new NullTransform();
	isHaveParent = false;
}
