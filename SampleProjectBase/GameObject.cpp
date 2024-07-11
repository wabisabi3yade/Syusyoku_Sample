#include "pch.h"
#include "GameObject.h"
// コンポーネント
#include "Component.h"
#include "CP_Collider.h"

// システム
#include "InSceneSystemManager.h"

using json = nlohmann::json;

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

	for (auto& itr : pComponents)
	{
		if (!itr->isEnable) continue;
		itr->Update();
	}
}

void GameObject::LateUpdateBase()
{
	if (!isActive) return;

	LateUpdate();

	for (auto& itr : pComponents)
	{
		if (!itr->isEnable) continue;
		itr->LateUpdate();
	}
}

void GameObject::DrawBase()
{
	if (!isActive) return;

	// 方向ベクトルを更新する
	transform.UpdateVector();

	Draw();

	for (auto& itr : pComponents)
	{
		if (!itr->isEnable) continue;
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

void GameObject::ImGuiSet()
{
#ifdef EDIT
	if (ImGui::TreeNode(name.c_str()))	// 名前Tree
	{
		ImGui::Checkbox("isActive", &isActive);
		ImGuiMethod::DragFloat3(transform.position, "pos");
		ImGuiMethod::DragFloat3(transform.rotation, "rot");
		ImGuiMethod::DragFloat3(transform.scale, "scale");

		for (auto& itr : pComponents)
		{
			if (ImGui::TreeNode(itr->name.c_str()))
			{
				ImGui::Checkbox("isEnabled", &itr->isEnable);
				itr->ImGuiSetting();
				ImGui::TreePop();
			}
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
