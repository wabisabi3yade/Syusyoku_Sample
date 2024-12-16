#include "Transform.h"
#include "NullTransform.h"
#include "InSceneSystemManager.h"
#include "GameObject.h"

namespace HashiTaku
{
	Transform::Transform(GameObject* _pGameObject, bool _isInit) :
		pParent(nullptr),
		scale(DXSimp::Vector3::One),
		localScale(DXSimp::Vector3::One),
		up(Vec3::Up),
		right(Vec3::Right),
		forward(Vec3::Forward),
		isHaveParent(false),
		isNull(false)
	{
		if (_isInit)
		{
			pParent = new NullTransform();
			isNull = false;
		}

		pGameObject = _pGameObject;
	}

	Transform::~Transform()
	{
		if (!isNull)
		{
			if (pParent)
				pParent->RemoveChild(*this);

			for (auto& pChild : childTransforms)
				pChild->pParent = nullptr;
		}

		// �e�g�����X�t�H�[����NullTransform�Ȃ�
		if (!isHaveParent)
		{
			CLASS_DELETE(pParent);
			return;
		}
	}

	void Transform::UpdateVector()
	{
		// �N�H�[�^�j�I�������]�s������߂�
		DXSimp::Matrix rotateMatrix(DXSimp::Matrix::CreateFromQuaternion(rotation));

		// �����x�N�g�����X�V
		right = Mtx::GetRightVector(rotateMatrix);
		up = Mtx::GetUpVector(rotateMatrix);
		forward = Mtx::GetForwardVector(rotateMatrix);
	}

	void Transform::LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector)
	{
		DXSimp::Vector3 vec = _worldPos - position;
		DXSimp::Quaternion q = Quat::RotateToVector(vec);

		SetRotation(q);
	}

	void Transform::RemoveChild(Transform& _removeTransform)
	{
		childTransforms.remove(&_removeTransform);
	}

	void Transform::SetParent(Transform& _parent)
	{
		pParent->RemoveChild(*this);

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

		// ���[���h���W�͕ς����Ƀ��[�J���s������߂�
		_child.UpdateLocalMatrixFromWorld();
	}


	void Transform::SetPosition(const DirectX::SimpleMath::Vector3& _pos)
	{
		// ���[�J���s������߂�ׂɃ��[�J�����W���X�V
		localPosition = DXSimp::Vector3::Transform(_pos, pParent->GetWorldMatrix().Invert());

		// ���[�J���s����X�V
		UpdateLocalMatrix();

		// ���[���h�s����X�V
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangePosNotify();

		// Ui��������V�[���I�u�W�F�N�g��Z���W�\�[�g���Ă��炤
		if (pGameObject->GetLayer() != Layer::UI) return;
		InSceneSystemManager::GetInstance()->GetSceneObjects().SortUiList();
	}

	void Transform::SetScale(const DirectX::SimpleMath::Vector3& _scale)
	{
		//scale = _scale;
		localScale = scale / pParent->GetScale();

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangeScaleNotify();
	}

	void Transform::SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
	{
		// ���[�J����]�ʂ����߂�
		DXSimp::Quaternion invParentRot = XMQuaternionInverse(pParent->GetRotation());
		localRotation = Quat::Multiply(Quat::ToQuaternion(_eularAngles), invParentRot);

		localEularAngles = localRotation.ToEuler() * Mathf::radToDeg;

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangeRotNotify();
	}

	void Transform::SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
	{
		DXSimp::Quaternion invParentRot = XMQuaternionInverse(pParent->GetRotation());
		localRotation = Quat::Multiply(_quaternion, invParentRot);

		localEularAngles = localRotation.ToEuler() * Mathf::radToDeg;

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangeRotNotify();
	}


	void Transform::SetLocalPosition(const DirectX::SimpleMath::Vector3& _localPos)
	{
		localPosition = _localPos;

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangePosNotify();

		// Ui��������V�[���I�u�W�F�N�g��Z���W�\�[�g���Ă��炤
		if (pGameObject->GetLayer() != Layer::UI) return;
		InSceneSystemManager::GetInstance()->GetSceneObjects().SortUiList();
	}

	void Transform::SetLocalScale(const DirectX::SimpleMath::Vector3& _scale)
	{
		localScale = _scale;

		UpdateLocalMatrix();
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangeScaleNotify();
	}

	void Transform::SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles)
	{
		localEularAngles = _eularAngles;
		// �N�H�[�^�j�I���ɔ��f������
		localRotation = Quat::ToQuaternion(localEularAngles);

		UpdateLocalMatrix();
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangeRotNotify();
	}

	void Transform::SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion)
	{
		localRotation = _quaternion;
		localEularAngles = localRotation.ToEuler() * Mathf::radToDeg;

		UpdateLocalMatrix();
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// �I�u�W�F�N�g���ɕύX�������Ƃ�`����
		ChangeRotNotify();
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

	const std::string& Transform::GetObjectName() const
	{
		return pGameObject->GetName();
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

		// ���[�J�����W���Z�[�u����
		SaveJsonVector3("pos", position, transformData);
		SaveJsonVector3("scale", scale, transformData);
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
		// ���[���h���W�����[�h����
		LoadJsonQuaternion("rotation", rotation, _transformData);
		eularAngles = rotation.ToEuler() * Mathf::radToDeg;

		LoadJsonVector3("pos", position, _transformData);
		LoadJsonVector3("scale", scale, _transformData);

		// ���[���h�s������߂�
		Mtx::CreateTransformMatrix(position, scale, rotation, worldMatrix);

		// ���[���h���烍�[�J���s������߂�
		UpdateLocalMatrixFromWorld();
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

	void Transform::UpdateWorldMatrix(const DirectX::SimpleMath::Matrix& _parentWorldMtx)
	{
		// ���[���h�s����X�V
		worldMatrix = localMatrix * _parentWorldMtx;

		// �p�����[�^�ɔ��f
		Mtx::GetTransformFromWldMtx(worldMatrix, position, scale, rotation);
		eularAngles = rotation.ToEuler() * Mathf::radToDeg;

		// �����x�N�g�����X�V
		UpdateVector();

		// �q�I�u�W�F�N�g�ɂ����f������
		for (auto& c : childTransforms)
		{
			c->UpdateWorldMatrix(worldMatrix);
		}
	}

	void Transform::UpdateLocalMatrix()
	{
		Mtx::CreateTransformMatrix(localPosition, localScale, localRotation, localMatrix);
	}

	void Transform::UpdateLocalMatrixFromWorld()
	{
		// ���[���h�s�񂩂烍�[�J���s������߂�
		localMatrix = worldMatrix * pParent->GetWorldMatrix().Invert();

		// �p�����[�^�ɔ��f����
		Mtx::GetTransformFromWldMtx(localMatrix, localPosition, localScale, localRotation);
		localEularAngles = localRotation.ToEuler() * Mathf::radToDeg;
	}

	void Transform::ChangePosNotify()
	{
		pGameObject->OnChangePosition();

		for (auto& child : childTransforms)
		{
			child->ChangePosNotify();
		}
	}

	void Transform::ChangeScaleNotify()
	{
		pGameObject->OnChangeScale();

		for (auto& child : childTransforms)
		{
			child->ChangeScaleNotify();
		}
	}

	void Transform::ChangeRotNotify()
	{
		pGameObject->OnChangeRotation();

		for (auto& child : childTransforms)
		{
			child->ChangeRotNotify();
		}
	}

	void Transform::RemoveParent()
	{
		pParent->RemoveChild(*this);

		pParent = new NullTransform();
		isHaveParent = false;

		UpdateLocalMatrixFromWorld();
	}
}