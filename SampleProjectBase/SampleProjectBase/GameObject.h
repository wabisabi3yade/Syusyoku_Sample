#pragma once
#include <typeinfo>
#include "Tag.h"
#include "Component.h"

// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
class GameObject
{
protected:
	bool isActive;	// �A�N�e�B�u��Ԃ��ǂ���
	std::string name;	// ���̃I�u�W�F�N�g�̖��O

	Tag tag;	// �Q�[���I�u�W�F�N�g�̃^�O
	Layer layer;	// �Q�[���I�u�W�F�N�g�̃��C���[

	DirectX::SimpleMath::Vector2 uvScroll;	// UV�X�N���[���l

	std::list<std::unique_ptr<Component>> pComponents;	// �R���|�[�l���g���X�g
public:
	Transform transform;	// Transform�p�����[�^

	GameObject() : isActive(true), name(""), uvScroll(DirectX::SimpleMath::Vector2::Zero) {};
	virtual ~GameObject() {};

	virtual void Update();	// �X�V����
	virtual void LateUpdate();	// Update���s�������Ƃ̍X�V����
	virtual void Draw();	// �`�揈��

	template<typename T> T* AddComponent();	// �R���|�[�l���g������
	template<typename T> T* GetComponent();	// �R���|�[�l���g���擾

	virtual void ImGuiSet();	// ImGui�̐ݒ�

	void SetName(const std::string& _name) { name = _name; }	// ���O
	const std::string& GetName() { return name; }

	// �^�O�E���C���[���擾
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
	for (auto comp : pComponents)
	{
		if (typeid(std::unique_ptr<T>) != typeid(comp)) continue;
		
		return comp.get(); // ����Ȃ�Ԃ�
	}

	return nullptr;	// ����������nullptr
}
