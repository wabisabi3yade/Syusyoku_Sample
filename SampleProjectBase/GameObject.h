#pragma once

// �����o�ϐ�
#include "Tag.h"

// �R���|�[�l���g����
#include "ComponentFactory.h"

// �V�[���֐�
#include "SF_Define.h"

// �Z�[�u�E���[�h
#include "ISaveLoad.h"

// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
class GameObject : public ISaveLoad
{
private:
	// �A�N�e�B�u��Ԃ��ǂ���
	bool isActive;	

	// ���̃I�u�W�F�N�g�̖��O
	std::string name;	

	// �^�O
	Tag tag;

	// ���C���[
	Layer layer;	

	// �R���|�[�l���g���X�g
	std::list<std::unique_ptr<Component>> pComponents;

	/// @brief �A�N�e�B�u��Ԃ̃R���|�[�l���g
	std::list<Component*> pActiveComponents;

	/// @brief Start�������s���R���|�[�l���g
	std::list<Component*> pStartComponents;
public:
	/// @brief �g�����X�t�H�[��
	Transform transform;	

	bool isSave = true;

	GameObject();
	GameObject(const GameObject& _other);
	GameObject& operator=(const GameObject& _other);
	virtual ~GameObject() {};

	// �X�V����
	void UpdateBase();	
	void LateUpdateBase();
	void DrawBase();

	// ���g���폜
	void Destroy();

	/// @brief �R���|�[�l���g���Z�b�g����Ƃ��̏���
	/// @param _pSetComponent �Z�b�g����R���|�[�l���g
	void SetComponent(std::unique_ptr<Component> _pSetComponent);

	// �R���|�[�l���g���A�^�b�`
	template<HashiTaku::ComponentConcept T> T* AddComponent();

	// �R���|�[�l���g���폜����
	void DeleteComponent(Component& _deleteComonent);

	// �R���|�[�l���g���擾
	template<HashiTaku::ComponentConcept T> T* GetComponent();

	// �A�N�e�B�u�z�񂩂�O���E�ǉ�����
	void RemoveActiveComponent(Component& _removeComonent);
	void AddActiveComponent(Component& _addComonentComonent);

	// Start�z��ɓ����

	// ImGui�̐ݒ�
	virtual void ImGuiSet();	

	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _sceneData) override;

	void SetName(const std::string& _name);
	void SetActive(bool _isActive);

	const std::string& GetName() const { return name; }
	bool GetIsActive() const { return isActive; }
	const Tag& GetTag() const { return tag; }
	const Layer& GetLayer() const  { return layer; }

private:
	// �A�N�e�B�u�ύX������
	void ActiveProcess();
	void NotActiveProcess();

	// Json���烍�[�h
	virtual void FromJson(const nlohmann::json& _jsonData) {};

	virtual GameObject& Copy(const GameObject& _other);

	/// @brief �������Ă���z��ɃR���|�[�l���g�����݂��邩
	/// @param _pCheckComponent �m�F����R���|�[�l���g
	/// @return ���݂������H
	bool IsExistComponent(const Component& _pCheckComponent);

	/// @brief	�A�N�e�B�u��Ԃ̃R���|�[�l���g���ɂ����݂��邩
	/// @param _pCheckComponent �m�F����R���|�[�l���g
	/// @return ���݂������H
	bool IsExistActiveComponent(const Component& _pCheckComponent);

	/// @brief Start�z��ɑ��݂��邩
	/// @param _pCheckComponent �m�F����R���|�[�l���g
	/// @return ���݂������H
	bool IsExistStartComponent(const Component& _pCheckComponent);

	/// @brief �R���|�[�l���g�̃��[�h����
	/// @param _componentData ���̃I�u�W�F�N�g�S�ẴR���|�[�l���g�f�[�^
	void LoadComponent(const nlohmann::json& _componentsData);
};

template<HashiTaku::ComponentConcept T>
inline T* GameObject::AddComponent()
{
	// �R���|�[�l���g�t�@�N�g���[����擾
	std::unique_ptr<T> createComp = ComponentFactory::GetInstance()->Create<T>();
	T& comp = *createComp;
	
	// �R���|�[�l���g�ǉ�����
	SetComponent(std::move(createComp));

	return &comp;
}

template<HashiTaku::ComponentConcept T>
inline T* GameObject::GetComponent()
{
	// �w�肵���^���Ɠ����R���|�[�l���g�����邩�m�F
	for (auto& comp : pComponents)
	{
		if (typeid(T) != typeid(*comp.get())) continue;
		T* retPtr = static_cast<T*>(comp.get());	
		return retPtr; 
	}

	return nullptr;
}
