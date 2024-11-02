#pragma once

// �����o�ϐ�
#include "Tag.h"
// �R���|�[�l���g����
#include "ComponentFactory.h"
// �V�[���֐�
#include "SF_Define.h"
// �Z�[�u�E���[�h
#include "ISaveLoad.h"

#include "CollisionInfo.h"

// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
class GameObject : public HashiTaku::ISaveLoad, public HashiTaku::IImGuiUser
{
private:
	/// @brief ���̃I�u�W�F�N�g�̖��O
	std::string name;	

	/// @brief �g�����X�t�H�[��
	std::unique_ptr<Transform> pTransform;

	/// @brief 
	std::unique_ptr<CollisionTypeJudge> pCollisionJudge;

	// �R���|�[�l���g���X�g
	std::list<std::unique_ptr<Component>> components;

	/// @brief �A�N�e�B�u��Ԃ̃R���|�[�l���g
	std::list<Component*> pActiveComponents;

	/// @brief Awake�������s���R���|�[�l���g
	std::list<Component*> pAwakeComponents;

	/// @brief Start�������s���R���|�[�l���g
	std::list<Component*> pStartComponents;

	/// @brief �^�O
	HashiTaku::Tag tag;

	/// @brief ���C���[
	HashiTaku::Layer layer;

	/// @brief �A�N�e�B�u��Ԃ��ǂ���
	bool isActive;
public:
	GameObject();
	GameObject(const GameObject& _other);
	GameObject& operator=(const GameObject& _other);
	~GameObject();

	/// @brief �K���ŏ��ɌĂяo����鏈��
	void AwakeCall();
	/// @brief Awake�̎��ɌĂяo����鏈��
	void StartCall();
	/// @brief ���t���[���X�V���鏈��
	void UpdateCall();	
	/// @brief Update������ɍX�V����鏈��
	void LateUpdateCall();
	/// @brief �`�揈��
	void DrawCall();

	/// @brief �ՓˊJ�n���̏���
	void OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo);
	/// @brief �Փ˒��̏���
	void OnCollisionStay(const HashiTaku::CollisionInfo& _otherColInfo);
	/// @brief �ՓˏI�����̏���
	void OnCollisionExit(const HashiTaku::CollisionInfo& _otherColInfo);

	/// @brief ���g���폜
	void Destroy();

	/// @brief �ړ����W���ύX�������̃R�[���o�b�N
	void OnChangePosition();
	/// @brief �ړ����W���ύX�������̃R�[���o�b�N
	void OnChangeScale();
	/// @brief �ړ����W���ύX�������̃R�[���o�b�N
	void OnChangeRotation();

	/// @brief �R���|�[�l���g���Z�b�g����Ƃ��̏���
	/// @param _pSetComponent �Z�b�g����R���|�[�l���g
	void SetComponent(std::unique_ptr<Component> _pSetComponent);

	/// @brief �R���|�[�l���g���A�^�b�`
	template<HashiTaku::ComponentConcept T> T* AddComponent();

	/// @brief �R���|�[�l���g���폜����
	void DeleteComponent(Component& _deleteComonent);

	/// @brief/ �R���|�[�l���g���擾
	template<class T> T* GetComponent();

	/// @brief �A�N�e�B�u�z�񂩂�O���E�ǉ�����
	void RemoveActiveComponent(Component& _removeComonent);

	/// @brief ������ԃ��X�g�ɒǉ�
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

	/// @brief �I�u�W�F�N�g�����Z�b�g
	/// @param _name �I�u�W�F�N�g��
	void SetName(const std::string& _name);

	/// @brief ������Ԃ��Z�b�g
	/// @param _isActive �Z�b�g�������������
	void SetActive(bool _isActive);

	/// @brief �^�O���Z�b�g
	/// @param _setType �Z�b�g�������^�O�̎��
	void SetTag(HashiTaku::Tag::Type _setType);

	/// @brief ���C���[���Z�b�g
	/// @param _setType �Z�b�g���������C���[�̎��
	void SetLayer(HashiTaku::Layer::Type _setType);

	/// @brief �g�����X�t�H�[�����擾
	/// @return �g�����X�t�H�[���̎Q��
	Transform& GetTransform();

	/// @brief �I�u�W�F�N�g�����擾
	/// @return �I�u�W�F�N�g��
	const std::string& GetName() const;

	/// @brief ������Ԃ��擾
	/// @return �������
	bool GetIsActive() const;
	
	/// @brief �^�O�̎�ނ��擾
	/// @return �^�O�̎��
	HashiTaku::Tag::Type GetTag() const;

	/// @brief ���C���[�̎�ނ��擾
	/// @return ���C���[�̎��
	HashiTaku::Layer::Type GetLayer() const;
private:
	virtual GameObject& Copy(const GameObject& _other);

	/// @brief �A�N�e�B�u��Ԃ�؂�ւ������ɋN��������
	void OnActiveTrue();

	/// @brief ��A�N�e�B�u��Ԃ�؂�ւ������ɋN��������
	void OnActiveFalse();

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
	template<class T> bool isDuplicateCompoent();

	/// @brief �R���|�[�l���g�������[�h����
	/// @param _componentsData �R���|�[�l���g�f�[�^
	void LoadCreateComponnet(const nlohmann::json& _componentsData);

	/// @brief �R���|�[�l���g�̃��[�h����
	/// @param _componentData ���̃I�u�W�F�N�g�S�ẴR���|�[�l���g�f�[�^
	void LoadComponentParameter(const nlohmann::json& _componentsData);

	// ImGui�̐ݒ�
	void ImGuiSetting() override;

	// ImGui�Őe��Ԃ̐ݒ�
	void ImGuiSetParent();
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

template<class T>
inline T* GameObject::GetComponent()
{
	// �w�肵���^���Ɠ����R���|�[�l���g�����邩�m�F
	for (auto& comp : components)
	{
		if (T* getPtr = dynamic_cast<T*>(comp.get()))
		{
			return getPtr;
		}
	}

	return nullptr;
}

template<class T>
inline bool GameObject::isDuplicateCompoent()
{
	// �w�肵���^���Ɠ����R���|�[�l���g�����邩�m�F
	for (auto& comp : components)
	{
		if (typeid(T) == typeid(*comp))
			return true;
	}

	return false;
}
