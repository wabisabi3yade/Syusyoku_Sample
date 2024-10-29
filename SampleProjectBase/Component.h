#pragma once
#include "ISaveLoad.h"

class GameObject;

namespace HashiTaku
{
	struct CollisionInfo;
}

// �Q�[���̃T�u�V�X�e���ƂȂ�R���|�[�l���g�̊��N���X
class Component : public HashiTaku::ISaveLoad, public HashiTaku::IImGuiUser
{
	friend class GameObject;

	/// @brief ���O
	std::string name;

	/// @brief Awake�������s�������ǂ����H
	bool isAlreadyAwake;

	/// @brief Start�������s�������ǂ����H
	bool isAlreadyStart;
protected:
	/// @brief �������
	bool isEnable;

	/// @brief ���̃R���|�[�l���g�̏����I�u�W�F�N�g
	GameObject* gameObject;
public:
	Component() : name(""), isEnable(true), isAlreadyAwake(false), isAlreadyStart(false), gameObject(nullptr) {}
	virtual ~Component() {};

	virtual void Init() {};

	/// @brief �O������Ăяo��Awake�����Ăяo��
	void AwakeCall();

	/// @brief �O������Ăяo��Start�����Ăяo��
	void StartCall();

	/// @brief �O������Ăяo��Update�����Ăяo��
	void UpdateCall();

	/// @brief �O������Ăяo��LateUpdate�����Ăяo��
	void LateUpdateCall();

	/// @brief �O������Ăяo��Draw�����Ăяo��
	void DrawCall();

	/// @brief �폜���ꂽ�Ƃ��ɏ�������
	virtual void OnDestroy() {}
	/// @brief �g�����X�t�H�[���̈ړ����W���ύX�������̏���
	virtual void OnChangePosition() {}
	/// @brief �g�����X�t�H�[���̃X�P�[���l���ύX�������̏���
	virtual void OnChangeScale() {}
	/// @brief �g�����X�t�H�[���̉�]�p�x���ύX�������̏���
	virtual void OnChangeRotation() {}

	/// @brief �ՓˊJ�n���̏����Ăяo��
	virtual void OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo) {}
	/// @brief �Փ˒��̏����Ăяo��
	virtual void OnCollisionStay(const HashiTaku::CollisionInfo& _otherColInfo) {}
	/// @brief �ՓˏI�����̏����Ăяo��
	virtual void OnCollisionExit(const HashiTaku::CollisionInfo& _otherColInfo) {}

	/// @brief ���O���Z�b�g����
	/// @param _name ���O
	void SetName(const std::string& _name);

	// ������Ԃ��Z�b�g
	void SetEnable(bool _enable);

	// ���O���擾
	std::string GetName() const;

	// �I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform& GetTransform() const;

	// �Q�[���I�u�W�F�N�g�擾	
	GameObject& GetGameObject() const { return *gameObject; }

	// �����t���O�擾
	bool GetIsEnable() const;

	// Awake�����s�������擾
	bool GetIsAlreadyAwake() const;

	// Start�����s�������擾
	bool GetIsAlreadyStart() const;

	/// @brief ImGui�Ńp�����[�^��ς��鏈��
	void ImGuiSettingCall();

	/// @brief �Z�[�u����
	/// @param _data �Z�[�u�V�[���f�[�^
	nlohmann::json Save() override;

	/// @brief ���[�h����
	/// @param _data ���[�h����V�[���f�[�^ 
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief ������Ԃ�true�ɕύX�����Ƃ��̏���
	void OnEnableTrueCall();

	/// @brief ������Ԃ�false�ɕύX�����Ƃ��̏���
	void OnEnableFalseCall();
protected:
	/// @brief �쐬���ɌĂ΂�鏈��
	virtual void Awake() {}

	/// @brief 1�t���[���ڂōs������
	virtual void Start() {};

	/// @brief ���t���[���X�V����
	virtual void Update() {};

	/// @brief Update�̌�ɍs���X�V����
	virtual void LateUpdate() {};

	/// @brief �`�揈��
	virtual void Draw() {};

	// ������Ԃ�؂�ւ��鎞�̏���
	virtual void OnEnableTrue() {}
	virtual void OnEnableFalse() {}

	/// @brief �Q�[���I�u�W�F�N�g�ƃR���|�[�l���g�̊�����Ԃ��擾
	/// @return �ǂ�����������Ă��邩�H
	bool GetIsActive();

	void ImGuiSetting() override {}
};
