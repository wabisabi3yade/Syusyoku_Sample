#pragma once

// �����o�ϐ�
#include "Tag.h"

// �R���|�[�l���g����
#include "ComponentFactory.h"

// �V�[���֐�
#include "SF_Include.h"

// Json
#include "SaveJson.h"
#include "LoadJson.h"
#include "SaveJsonValue.h"

class SceneObjects;
// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
class GameObject
{
protected:
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

	// �Z�[�u������ϐ����X�g
	//std::unique_ptr<SaveJsonValue> saveValues;	

	// �A�N�e�B�u�ύX������
	void ActiveProcess();
	void NotActiveProcess();

	// �e�I�u�W�F�N�g�̏���
	virtual void Update() {};	
	virtual void LateUpdate(){};
	virtual void Draw() {};

	// Json���烍�[�h
	virtual void FromJson(const nlohmann::json& _jsonData) {};

	virtual GameObject& Copy(const GameObject& _other);
public:
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
};

template<typename T>
inline T* GameObject::AddComponent()
{
	// �R���|�[�l���g�t�@�N�g���[����擾
	std::unique_ptr<Component> createComp = ComponentFactory::Create<T>();
	createComp->gameObject = this;

	// ��������
	createComp->Init();	

	// �߂�l���擾
	T* retPtr = static_cast<T*>(createComp.get());	

	// ���X�g�ɒǉ�
	pComponents.push_back(std::move(createComp));	

	return retPtr;
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
