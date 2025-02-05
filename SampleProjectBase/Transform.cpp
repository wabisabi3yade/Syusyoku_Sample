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

		// 親トランスフォームがNullTransformなら
		if (!isHaveParent)
		{
			CLASS_DELETE(pParent);
			return;
		}
	}

	void Transform::UpdateVector()
	{
		// クォータニオンから回転行列を求める
		DXSimp::Matrix rotateMatrix(DXSimp::Matrix::CreateFromQuaternion(rotation));

		// 方向ベクトルを更新
		right = Mtx::GetRightVector(rotateMatrix);
		up = Mtx::GetUpVector(rotateMatrix);
		forward = Mtx::GetForwardVector(rotateMatrix);
	}

	void Transform::LookAt(const DXSimp::Vector3& _worldPos, const DXSimp::Vector3& _upVector)
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
			HASHI_DEBUG_LOG("既に子オブジェクトです");
			return;
		}

		childTransforms.push_back(&_child);

		if (!_child.isHaveParent)	// 親トランスフォームがnullなら
			CLASS_DELETE(_child.pParent);

		_child.pParent = this;
		_child.isHaveParent = true;

		// ワールド座標は変えずにローカル行列を求める
		_child.UpdateLocalMatrixFromWorld();
	}


	void Transform::SetPosition(const DXSimp::Vector3& _pos)
	{
		// ローカル行列を求める為にローカル座標を更新
		localPosition = DXSimp::Vector3::Transform(_pos, pParent->GetWorldMatrix().Invert());

		// ローカル行列を更新
		UpdateLocalMatrix();

		// ワールド行列を更新
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangePosNotify();

		// UiだったらシーンオブジェクトにZ座標ソートしてもらう
		if (pGameObject->GetLayer() != Layer::UI) return;
		InSceneSystemManager::GetInstance()->GetSceneObjects().SortUiList();
	}

	void Transform::SetScale(const DXSimp::Vector3& _scale)
	{
		localScale = _scale / pParent->GetScale();

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangeScaleNotify();
	}

	void Transform::SetEularAngles(const DXSimp::Vector3& _eularAngles)
	{
		// ローカル回転量を求める
		DXSimp::Quaternion invParentRot = XMQuaternionInverse(pParent->GetRotation());
		localRotation = Quat::Multiply(Quat::ToQuaternion(_eularAngles), invParentRot);

		localEularAngles = localRotation.ToEuler() * Mathf::radToDeg;

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangeRotNotify();
	}

	void Transform::SetRotation(const DXSimp::Quaternion& _quaternion)
	{
		DXSimp::Quaternion invParentRot = XMQuaternionInverse(pParent->GetRotation());
		localRotation = Quat::Multiply(_quaternion, invParentRot);

		localEularAngles = localRotation.ToEuler() * Mathf::radToDeg;

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangeRotNotify();
	}


	void Transform::SetLocalPosition(const DXSimp::Vector3& _localPos)
	{
		localPosition = _localPos;

		UpdateLocalMatrix();

		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangePosNotify();

		// UiだったらシーンオブジェクトにZ座標ソートしてもらう
		if (pGameObject->GetLayer() != Layer::UI) return;
		InSceneSystemManager::GetInstance()->GetSceneObjects().SortUiList();
	}

	void Transform::SetLocalScale(const DXSimp::Vector3& _scale)
	{
		localScale = _scale;

		UpdateLocalMatrix();
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangeScaleNotify();
	}

	void Transform::SetLocalEularAngles(const DXSimp::Vector3& _eularAngles)
	{
		localEularAngles = _eularAngles;
		// クォータニオンに反映させる
		localRotation = Quat::ToQuaternion(localEularAngles);

		UpdateLocalMatrix();
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangeRotNotify();
	}

	void Transform::SetLocalRotation(const DXSimp::Quaternion& _quaternion)
	{
		localRotation = _quaternion;
		localEularAngles = localRotation.ToEuler() * Mathf::radToDeg;

		UpdateLocalMatrix();
		UpdateWorldMatrix(pParent->GetWorldMatrix());

		// オブジェクト側に変更したことを伝える
		ChangeRotNotify();
	}

	const DXSimp::Vector3& Transform::GetPosition() const
	{
		return position;
	}

	const DXSimp::Vector3& Transform::GetScale() const
	{
		return scale;
	}

	const DXSimp::Vector3& Transform::GetEularAngles() const
	{
		return eularAngles;
	}

	const DXSimp::Quaternion& Transform::GetRotation() const
	{
		return rotation;
	}

	const DXSimp::Matrix& Transform::GetWorldMatrix() const
	{
		return worldMatrix;
	}

	const DXSimp::Vector3& Transform::GetLocalPosition() const
	{
		return localPosition;
	}

	const DXSimp::Vector3& Transform::GetLocalScale() const
	{
		return localScale;
	}

	const DXSimp::Vector3& Transform::GetLocalEularAngles() const
	{
		return localEularAngles;
	}

	const DXSimp::Quaternion& Transform::GetLocalRotation() const
	{
		return localRotation;
	}

	const DXSimp::Matrix& Transform::GetLocalMatrix() const
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

	json Transform::Save()
	{
		json transformData;

		// ローカル座標をセーブする
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

	void Transform::Load(const json& _transformData)
	{
		// ワールド座標をロードする
		LoadJsonQuaternion("rotation", rotation, _transformData);
		eularAngles = rotation.ToEuler() * Mathf::radToDeg;

		LoadJsonVector3("pos", position, _transformData);
		LoadJsonVector3("scale", scale, _transformData);

		// ワールド行列を求める
		Mtx::CreateTransformMatrix(position, scale, rotation, worldMatrix);

		// ワールドからローカル行列を求める
		UpdateLocalMatrixFromWorld();
	}

	void Transform::LoadChildTransform(const json& _transformData)
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

	void Transform::UpdateWorldMatrix(const DXSimp::Matrix& _parentWorldMtx)
	{
		// ワールド行列を更新
		worldMatrix = localMatrix * _parentWorldMtx;

		// パラメータに反映
		Mtx::GetTransformFromWldMtx(worldMatrix, position, scale, rotation);
		eularAngles = rotation.ToEuler() * Mathf::radToDeg;

		// 方向ベクトルを更新
		UpdateVector();

		// 子オブジェクトにも反映させる
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
		// ワールド行列からローカル行列を求める
		localMatrix = worldMatrix * pParent->GetWorldMatrix().Invert();

		// パラメータに反映する
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