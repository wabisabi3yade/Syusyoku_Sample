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
class GameObject : public ISaveLoad, public HashiTaku::IImGuiUser
{
private:
	// ���̃I�u�W�F�N�g�̖��O
	std::string name;	

	/// @brief �g�����X�t�H�[��
	std::unique_ptr<Transform> pTransform;

	// �R���|�[�l���g���X�g
	std::list<std::unique_ptr<Component>> pComponents;

	/// @brief �A�N�e�B�u��Ԃ̃R���|�[�l���g
	std::list<Component*> pActiveComponents;

	/// @brief Awake�������s���R���|�[�l���g
	std::list<Component*> pAwakeComponents;

	/// @brief Start�������s���R���|�[�l���g
	std::list<Component*> pStartComponents;

	// �^�O
	Tag tag;

	// ���C���[
	Layer layer;

	// �A�N�e�B�u��Ԃ��ǂ���
	bool isActive;
public:
	GameObject();
	GameObject(const GameObject& _other);
	GameObject& operator=(const GameObject& _other);
	~GameObject();

	// �X�V����
	void AwakeCall();
	void StartCall();
	void UpdateCall();	
	void LateUpdateCall();
	void DrawCall();

	// ���g���폜
	void Destroy();

	/// @brief �g�����X�t�H�[�����ύX�������̃R�[���o�b�N
	void OnChangeTransform();

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

	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _sceneData) override;

	/// @brief �I�u�W�F�N�g�֌W�����[�h����(�V�[�����̃I�u�W�F�N�g���K�v)
	/// @param _sceneData ���[�h����V�[���f�[�^ 
	void LateLode(const nlohmann::json& _sceneData);

	void SetName(const std::string& _name);
	void SetActive(bool _isActive);

	Transform& GetTransform();
	const std::string& GetName() const { return name; }
	bool GetIsActive() const { return isActive; }
	Tag GetTag() const { return tag; }
	Layer GetLayer() const  { return layer; }
private:
	virtual GameObject& Copy(const GameObject& _other);

	// �A�N�e�B�u��Ԃ�؂�ւ������ɋN��������
	void OnActiveTrue();
	void OnActiveFalse();

	void ImGuiSetParent();

	/// @brief �������Ă���z��ɃR���|�[�l���g�����݂��邩
	/// @param _pCheckComponent �m�F����R���|�[�l���g
	/// @return ���݂������H
	bool IsExistComponent(const Component& _pCheckComponent);

	/// @brief	�A�N�e�B�u��Ԃ̃R���|�[�l���g���ɂ����݂��邩
	/// @param _pCheckComponent �m�F����R���|�[�l���g
	/// @return ���݂������H
	bool IsExistActiveComponent(const Component& _pCheckComponent);

	/// @brief Awake�z��ɑ��݂��邩
	/// @param _pCheckComponent �m�F����R���|�[�l���g
	/// @return ���݂������H
	bool IsExistAwakeComponent(const Component& _pCheckComponent);

	/// @brief Start�z��ɑ��݂��邩
	/// @param _pCheckComponent �m�F����R���|�[�l���g
	/// @return ���݂������H
	bool IsExistStartComponent(const Component& _pCheckComponent);

	/// @brief �R���|�[�l���g���d�����Ă��邩
	/// @tparam  �R���|�[�l���g
	/// @return �d�����Ă��邩�H
	template<HashiTaku::ComponentConcept T> bool isDuplicateCompoent();

	/// @brief �R���|�[�l���g�̃��[�h����
	/// @param _componentData ���̃I�u�W�F�N�g�S�ẴR���|�[�l���g�f�[�^
	void LoadComponent(const nlohmann::json& _componentsData);

	// ImGui�̐ݒ�
	void ImGuiSetting() override;
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
		if (typeid(T) != typeid(*comp)) continue;

		T* retPtr = static_cast<T*>(comp.get());	
		return retPtr; 
	}

	return nullptr;
}

template<HashiTaku::ComponentConcept T>
inline bool GameObject::isDuplicateCompoent()
{
	// �w�肵���^���Ɠ����R���|�[�l���g�����邩�m�F
	for (auto& comp : pComponents)
	{
		if (typeid(T) == typeid(*comp))
			return true;
	}

	return false;
}
