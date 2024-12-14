#pragma once
#include "Singleton_Base.h"

// コンポーネントヘッダー
#include "ComponentDefine.h"
#include "ComponentRespawner.h"

class GameObject;

/// @brief コンポーネントのファクトリーメソッド
class ComponentFactory : public Singleton_Base<ComponentFactory>
{
	friend class Singleton_Base<ComponentFactory>;

	std::map<std::string, std::unique_ptr<ComponentRespawner_Base>> pComponents;
public:
	/// @brief 初期化処理
	void Init();

	/// @brief コンポーネント作成
	/// @tparam T コンポーネントの型
	/// @return コンポーネントのユニークポインタ
	template<HashiTaku::ComponentConcept T> std::unique_ptr<T> Create();

	/// @brief コンポーネント作成
	/// @param _componentName コンポーネント名 
	/// @return 作成したコンポーネント
	std::unique_ptr<Component> CreateByName(const std::string& _componentName);

	/// @brief ImGuiでコンボボックスから生成
	/// @param _targetObject コンポーネント追加したいオブジェクト 
	void CreateImGuiCombo(GameObject& _targetObject);
private:
	ComponentFactory() {}
	~ComponentFactory() {};

	std::vector<const std::string*> GetComponentsName();

	template<HashiTaku::ComponentConcept T> void ResistComponnent();

	template<HashiTaku::ComponentConcept T> static std::string GetComponentName();
};

/// @brief コンポーネント作成
/// @tparam T コンポーネントの型
/// @return コンポーネントのユニークポインタ
template<HashiTaku::ComponentConcept T>
inline std::unique_ptr<T> ComponentFactory::Create()
{
	std::unique_ptr<Component> createBaseComp = CreateByName(GetComponentName<T>());

	// ダウンキャストする
	std::unique_ptr<T> createComp = std::unique_ptr<T>(static_cast<T*>(createBaseComp.release()));

	// 名前セット
	Component& comp = *createComp;
	comp.SetName(GetComponentName<T>());

	return std::move(createComp);
}

template<HashiTaku::ComponentConcept T>
inline void ComponentFactory::ResistComponnent()
{
	std::string name = GetComponentName<T>();
	HASHI_DEBUG_LOG(name + " 作成");

	pComponents[name] = std::make_unique<ComponentRespawner<T>>();
}

template<HashiTaku::ComponentConcept T>
inline std::string ComponentFactory::GetComponentName()
{
	std::string componentName = typeid(T).name();

	constexpr u_int length = 9;

	if (componentName.find("class CP_") == 0) {
		// 'CP_' を取り除く
		componentName = componentName.substr(length);
	}

	return componentName;
}
