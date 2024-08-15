#include "pch.h"
#include "GameObject.h"
// コンポーネント
#include "Component.h"
#include "CP_Collider.h"

// システム
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

void GameObject::ActiveProcess()
{
	CollisionChecker& collisionChecker = InSceneSystemManager::GetInstance()->
		GetCollisonChecker();

	for (auto& c : pComponents)
	{
		// 当たり判定チェッククラスに追加
		CP_Collider* col = dynamic_cast<CP_Collider*>(c.get());
		if (col != nullptr)
		{
			collisionChecker.AddCollider(*col);	// 追加
		}
	}
}

void GameObject::NotActiveProcess()
{
	CollisionChecker& collisionChecker = InSceneSystemManager::GetInstance()->
		GetCollisonChecker();

	for (auto& c : pComponents)
	{
		// 当たり判定チェッククラスから削除
		CP_Collider* col = dynamic_cast<CP_Collider*>(c.get());
		if (col != nullptr)
		{
			collisionChecker.PopCollider(*col);	// 削除
		}
	}
}

GameObject& GameObject::Copy(const GameObject& _other)
{
	// 同じなら処理しない
	if (this == &_other) return *this;

	// パラメータ代入
	transform = _other.transform;
	isActive = _other.isActive;
	name = _other.name;
	tag = _other.tag;
	layer = _other.layer;

	// コンポーネントをコピー
	std::list<std::unique_ptr<Component>> pComponents;

	return *this;
}

bool GameObject::IsExistComponent(const Component& _pCheckComponent)
{
	auto itr = std::find_if(pComponents.begin(), pComponents.end(), [&](const std::unique_ptr<Component>& comp)
		{
			return comp.get() == &_pCheckComponent;
		});

	if (itr != pComponents.end()) return true;

	return false;
}

bool GameObject::IsExistActiveComponent(const Component& _pCheckComponent)
{
	auto itr = std::find_if(pActiveComponents.begin(), pActiveComponents.end(), [&](Component* comp)
		{
			return comp == &_pCheckComponent;
		});

	if (itr != pActiveComponents.end()) return true;

	return false;
}

GameObject::GameObject() : isActive(true), name("")
{
	/*saveValues = std::make_unique<SaveJsonValue>();*/
}

GameObject::GameObject(const GameObject& _other)
{
	Copy(_other);
}

GameObject& GameObject::operator=(const GameObject& _other)
{
	return Copy(_other);
}

void GameObject::UpdateBase()
{
	if (!isActive) return;

	for (auto& itr : pActiveComponents)
	{
		itr->Update();
	}
}

void GameObject::LateUpdateBase()
{
	if (!isActive) return;

	for (auto& itr : pActiveComponents)
	{
		itr->LateUpdate();
	}
}

void GameObject::DrawBase()
{
	if (!isActive) return;

	// 方向ベクトルを更新する
	transform.UpdateVector();

	for (auto& itr : pActiveComponents)
	{
		itr->Draw();
	}
}

void GameObject::Destroy()
{
	// シーンオブジェクトから自身を削除する
	SceneObjects& sceneObjects = InSceneSystemManager::GetInstance()->
		GetSceneObjects();
	sceneObjects.DeleteObj(*this);
}

void GameObject::RemoveActiveComponent(Component& _removeComonent)
{
	pActiveComponents.remove(&_removeComonent);
}

void GameObject::AddActiveComponent(Component& _addComonent)
{
	// 所持していないなら
	if (!IsExistComponent(_addComonent))
	{
		HASHI_DEBUG_LOG(_addComonent.name + ":オブジェクトにコンポーネントを所持できていません");
		return;
	}

	// アクティブ配列にあるなら
	if (IsExistActiveComponent(_addComonent))
	{
		HASHI_DEBUG_LOG(_addComonent.name + ":既にアクティブ状態です");
		return;
	}

	// 追加
	pActiveComponents.push_back(&_addComonent);
}

void GameObject::ImGuiSet()
{
#ifdef EDIT
	if (ImGui::TreeNode(name.c_str()))	// 名前Tree
	{
		ImGui::Checkbox("isActive", &isActive);
		ImGuiMethod::DragFloat3(transform.position, "pos");
		ImGuiMethod::DragFloat3(transform.scale, "scale");
		Vector3 angles = transform.GetEularAngles();
		ImGuiMethod::DragFloat3(angles, "rot");
		transform.SetEularAngles(angles);

		for (auto& itr : pComponents)
		{
			if (!ImGui::TreeNode(itr->name.c_str())) continue;

			std::string text = "isEnable:";
			std::string status = "true";
			if (!itr->GetIsEnable())
				status = "false";

			text += status;

			ImGui::Text(text.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Change"))
				itr->TransitionEnable();

			itr->ImGuiSetting();
			ImGui::TreePop();

		}

		ImGui::TreePop();
	}
#endif // EDIT
}


void GameObject::SetName(const std::string& _name)
{
	if (_name == "") return;
	name = _name;
}

void GameObject::SetActive(bool _isActive)
{
	if (isActive == _isActive) return;	// 同じ状態に変えようとするなら終わる

	if (_isActive)
		ActiveProcess();	// アクティブ時の処理をする
	else
		NotActiveProcess();	// 非アクティブ時の処理をする
}
