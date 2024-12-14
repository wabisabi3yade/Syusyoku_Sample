#pragma once
#include "Singleton_Base.h"

// �R���|�[�l���g�w�b�_�[
#include "ComponentDefine.h"
#include "ComponentRespawner.h"

class GameObject;

/// @brief �R���|�[�l���g�̃t�@�N�g���[���\�b�h
class ComponentFactory : public Singleton_Base<ComponentFactory>
{
	friend class Singleton_Base<ComponentFactory>;

	std::map<std::string, std::unique_ptr<ComponentRespawner_Base>> pComponents;
public:
	/// @brief ����������
	void Init();

	/// @brief �R���|�[�l���g�쐬
	/// @tparam T �R���|�[�l���g�̌^
	/// @return �R���|�[�l���g�̃��j�[�N�|�C���^
	template<HashiTaku::ComponentConcept T> std::unique_ptr<T> Create();

	/// @brief �R���|�[�l���g�쐬
	/// @param _componentName �R���|�[�l���g�� 
	/// @return �쐬�����R���|�[�l���g
	std::unique_ptr<Component> CreateByName(const std::string& _componentName);

	/// @brief ImGui�ŃR���{�{�b�N�X���琶��
	/// @param _targetObject �R���|�[�l���g�ǉ��������I�u�W�F�N�g 
	void CreateImGuiCombo(GameObject& _targetObject);
private:
	ComponentFactory() {}
	~ComponentFactory() {};

	std::vector<const std::string*> GetComponentsName();

	template<HashiTaku::ComponentConcept T> void ResistComponnent();

	template<HashiTaku::ComponentConcept T> static std::string GetComponentName();
};

/// @brief �R���|�[�l���g�쐬
/// @tparam T �R���|�[�l���g�̌^
/// @return �R���|�[�l���g�̃��j�[�N�|�C���^
template<HashiTaku::ComponentConcept T>
inline std::unique_ptr<T> ComponentFactory::Create()
{
	std::unique_ptr<Component> createBaseComp = CreateByName(GetComponentName<T>());

	// �_�E���L���X�g����
	std::unique_ptr<T> createComp = std::unique_ptr<T>(static_cast<T*>(createBaseComp.release()));

	// ���O�Z�b�g
	Component& comp = *createComp;
	comp.SetName(GetComponentName<T>());

	return std::move(createComp);
}

template<HashiTaku::ComponentConcept T>
inline void ComponentFactory::ResistComponnent()
{
	std::string name = GetComponentName<T>();
	HASHI_DEBUG_LOG(name + " �쐬");

	pComponents[name] = std::make_unique<ComponentRespawner<T>>();
}

template<HashiTaku::ComponentConcept T>
inline std::string ComponentFactory::GetComponentName()
{
	std::string componentName = typeid(T).name();

	constexpr u_int length = 9;

	if (componentName.find("class CP_") == 0) {
		// 'CP_' ����菜��
		componentName = componentName.substr(length);
	}

	return componentName;
}
