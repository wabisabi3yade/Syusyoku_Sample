#pragma once
#include "Tag.h"
#include "Component.h"
#include "MaterialClass.h"

// �V�[���֐�
#include "SF_Include.h"

// Json
#include "SaveJson.h"
#include "LoadJson.h"
#include "SaveJsonValue.h"

// ImGui
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

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

	// �A�N�e�B�u�ύX���̏���
	void ActiveProcess();

	// ��A�N�e�B�u�ύX���̏���
	void NotActiveProcess();

	// �X�V����
	virtual void Update() {};	

	// Json���烍�[�h
	virtual void FromJson(const nlohmann::json& _jsonData) {};

	virtual GameObject& Copy(const GameObject& _other);
public:
	Transform transform;	

	GameObject();
	GameObject(const GameObject& _other);	
	GameObject& operator=(const GameObject& _other);
	virtual ~GameObject() {};

	// �ǂ̃I�u�W�F�N�g���s�������͂���
	void UpdateBase();	

	// Update���s�������Ƃ̍X�V����
	virtual void LateUpdate();

	void DrawBase();
	// �`�揈��
	virtual void Draw(){};

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

	void SetName(const std::string& _name) { name = _name; }
	void SetActive(bool _isActive);

	const std::string& GetName() { return name; }
	bool GetIsActive() { return isActive; }
	const Tag& GetTag() { return tag; }
	const Layer& GetLayer() { return layer; }
};

template<typename T>
inline T* GameObject::AddComponent()
{
	// �R���|�[�l���g���쐬
	auto t = this;
	std::unique_ptr<T> addComp = std::make_unique<T>(t);

	Component* comp = dynamic_cast<Component*>(addComp.get());
	if (comp == nullptr)	// �R���|�[�l���g�N���X���p�����Ă邩�`�F�b�N
	{
		ImGuiDebugLog::Add("AddComponent�FComponent���p�����Ă��Ȃ��̂ŏ����ł��܂���");
		return nullptr;
	}
	comp->Init();	// ��������
	T* retPtr = addComp.get();	// �Ԃ��|�C���^���擾���Ă���

	pComponents.push_back(std::move(addComp));	// ���X�g�ɒǉ�

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
