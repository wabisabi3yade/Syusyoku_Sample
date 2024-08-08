#pragma once

// �����o�ϐ�
#include "Tag.h"

// �R���|�[�l���g����
#include "ComponentFactory.h"

// �V�[���֐�
#include "SF_Define.h"

// Json
#include "SaveJson.h"
#include "LoadJson.h"
#include "SaveJsonValue.h"

// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
class GameObject
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

	// �Z�[�u������ϐ����X�g
	//std::unique_ptr<SaveJsonValue> saveValues;	
public:
	/// @brief �g�����X�t�H�[��
	Transform transform;	

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

	// �R���|�[�l���g���A�^�b�`
	template<typename T> T* AddComponent();	

	// �R���|�[�l���g���擾
	template<typename T> T* GetComponent();	

	// �A�N�e�B�u�z�񂩂�O���E�ǉ�����
	void RemoveActiveComponent(Component& _removeComonent);
	void AddActiveComponent(Component& _addComonentComonent);

	// ImGui�̐ݒ�
	virtual void ImGuiSet();	

	// Json�t�@�C���ɏ�������
	//void ToJsonBase(); 
	
	// Json�t�@�C�����烍�[�h����
	//void FromJsonBase(const nlohmann::json& _jsonData);	

	void SetName(const std::string& _name);
	void SetActive(bool _isActive);

	const std::string& GetName() { return name; }const
	bool GetIsActive() const { return isActive; }
	const Tag& GetTag() { return tag; }
	const Layer& GetLayer() { return layer; }

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
};

template<typename T>
inline T* GameObject::AddComponent()
{
	// �R���|�[�l���g�t�@�N�g���[����擾
	std::unique_ptr<Component> createComp = ComponentFactory::Create<T>();
	createComp->gameObject = this;

	// �߂�l���擾
	T& comp = static_cast<T&>(*createComp.get());

	// ���X�g�ɒǉ�
	pComponents.push_back(std::move(createComp));
	pActiveComponents.push_back(&comp);

	// ��������
	comp.Init();	

	return &comp;
}

template<typename T>
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
