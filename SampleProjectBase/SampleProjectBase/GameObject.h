#pragma once
#include "Tag.h"

// �V�[���Ŏg�p����I�u�W�F�N�g�S�ʂ̊��N���X
class GameObject
{
protected:
	bool isActive;	// �A�N�e�B�u��Ԃ��ǂ���
	std::string name;	// ���̃I�u�W�F�N�g�̖��O

	Tag tag;	// �Q�[���I�u�W�F�N�g�̃^�O
	Layer layer;	// �Q�[���I�u�W�F�N�g�̃��C���[

	DirectX::SimpleMath::Vector2 uvScroll;	// UV�X�N���[���l
public:
	Transform transform;	// Transform�p�����[�^

	GameObject() : isActive(true), name(""), uvScroll(DirectX::SimpleMath::Vector2::Zero) {};
	virtual ~GameObject() {};

	virtual void Update(){};	// �X�V����
	virtual void LateUpdate() {};	// Update���s�������Ƃ̍X�V����
	virtual void Draw() {};	// �`�揈��

	virtual void ImGuiSet();	// ImGui�̐ݒ�

	void SetName(const std::string& _name) { name = _name; }	// ���O
	const std::string& GetName() { return name; }

	// �^�O�E���C���[���擾
	const Tag& GetTag() { return tag; }
	const Layer& GetLayer() { return layer; }
};

