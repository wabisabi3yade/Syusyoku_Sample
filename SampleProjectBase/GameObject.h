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

namespace HashiTaku
{
	// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
	class GameObject : public ISaveLoad, public IImGuiUser
	{
		friend class SceneObjects;

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
		Tag tag;

		/// @brief ���C���[
		Layer layer;

		/// @brief �I�u�W�F�N�g���g�̌o�ߎ��ԑ��x
		float deltaTimeSpeed;

		/// @brief �I�u�W�F�N�g���g�̌o�ߎ���
		float deltaTime;

		/// @brief �A�N�e�B�u��Ԃ��ǂ���
		bool isActive;

		/// @brief �폜���邩�H
		bool isDestroy;
	public:
		GameObject();
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
		void OnCollisionEnter(const CollisionInfo& _otherColInfo);
		/// @brief �Փ˒��̏���
		void OnCollisionStay(const CollisionInfo& _otherColInfo);
		/// @brief �ՓˏI�����̏���
		void OnCollisionExit(const CollisionInfo& _otherColInfo);


		/// @brief �ړ����W���ύX�������̃R�[���o�b�N
		void OnChangePosition();
		/// @brief �ړ����W���ύX�������̃R�[���o�b�N
		void OnChangeScale();
		/// @brief �ړ����W���ύX�������̃R�[���o�b�N
		void OnChangeRotation();

		/// @brief �R���|�[�l���g���Z�b�g����Ƃ��̏���
		/// @param _pSetComponent �Z�b�g����R���|�[�l���g
		void SetComponent(std::unique_ptr<Component> _pSetComponent);

		/// @brief �폜����悤�ɂ���
		void SetDestroy();

		/// @brief �R���|�[�l���g���A�^�b�`
		template<ComponentConcept T> T* AddComponent();

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
		json Save() override;

		/// @brief ���[�h����
		/// @param _sceneData ���[�h����V�[���f�[�^ 
		void Load(const json& _sceneData) override;

		/// @brief �I�u�W�F�N�g�֌W�����[�h����(�V�[�����̃I�u�W�F�N�g���K�v)
		/// @param _sceneData ���[�h����V�[���f�[�^ 
		void LateLode(const json& _sceneData);

		/// @brief �I�u�W�F�N�g�����Z�b�g
		/// @param _name �I�u�W�F�N�g��
		void SetName(const std::string& _name);

		/// @brief ������Ԃ��Z�b�g
		/// @param _isActive �Z�b�g�������������
		void SetActive(bool _isActive);

		/// @brief �^�O���Z�b�g
		/// @param _setType �Z�b�g�������^�O�̎��
		void SetTag(Tag::Type _setType);

		/// @brief ���C���[���Z�b�g
		/// @param _setType �Z�b�g���������C���[�̎��
		void SetLayer(Layer::Type _setType);

		/// @brief �o�ߎ��Ԃ�i�߂鑬�x���Z�b�g
		/// @param _deltaSpeed �o�ߎ��Ԃ�i�߂鑬�x(1.0������)
		void SetDeltaTimeSpeed(float _deltaSpeed);

		/// @brief �g�����X�t�H�[�����擾
		/// @return �g�����X�t�H�[���̎Q��
		Transform& GetTransform();

		/// @brief �g�����X�t�H�[�����擾
		/// @return �g�����X�t�H�[���̎Q��
		const Transform& GetConstTransform() const;

		/// @brief �I�u�W�F�N�g�����擾
		/// @return �I�u�W�F�N�g��
		const std::string& GetName() const;

		/// @brief �I�u�W�F�N�g�̌o�ߎ��Ԃ��擾����
		/// @return �I�u�W�F�N�g�̌o�ߎ���
		float DeltaTime() const;

		/// @brief ��t���x���擾
		/// @return ��t
		float GetDeltaSpeed() const;

		/// @brief ������Ԃ��擾
		/// @return �������
		bool GetIsActive() const;

		/// @brief �^�O�̎�ނ��擾
		/// @return �^�O�̎��
		Tag::Type GetTag() const;

		/// @brief ���C���[�̎�ނ��擾
		/// @return ���C���[�̎��
		Layer::Type GetLayer() const;
	private:
		/// @brief �o�ߎ��Ԃ��X�V����
		void UpdateDeltaTime();

		/// @brief �A�N�e�B�u��Ԃ�؂�ւ������ɋN��������
		void OnActiveTrue();

		/// @brief ��A�N�e�B�u��Ԃ�؂�ւ������ɋN��������
		void OnActiveFalse();

		/// @brief ���g���폜
		void Destroy();

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

		/// @brief �R���|�[�l���g����וς���
		void SortCompPriority();

		/// @brief �R���|�[�l���g�̗D��x���~���ɕ��ׂĂ����\�[�g�֐�
		static bool SortCompPriorityFunc(const Component* _c1, const Component* _c2);

		/// @brief �R���|�[�l���g�������[�h����
		/// @param _componentsData �R���|�[�l���g�f�[�^
		void LoadCreateComponnet(const json& _componentsData);

		/// @brief �R���|�[�l���g�̃��[�h����
		/// @param _componentData ���̃I�u�W�F�N�g�S�ẴR���|�[�l���g�f�[�^
		void LoadComponentParameter(const json& _componentsData);

		// ImGui�̐ݒ�
		void ImGuiDebug() override;

		// ImGui�Őe��Ԃ̐ݒ�
		void ImGuiSetParent();
	};

	template<ComponentConcept T>
	inline T* GameObject::AddComponent()
	{
		// �R���|�[�l���g�t�@�N�g���[����擾
		std::unique_ptr<T> createComp = ComponentFactory::GetInstance()->Create<T>();
		T* comp = createComp.get();

		// �R���|�[�l���g�ǉ�����
		SetComponent(std::move(createComp));

		return comp;
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
}