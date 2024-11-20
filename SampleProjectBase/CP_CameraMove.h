#pragma once
#include "Component.h"
#include "CameraMoveController.h"

class CP_Camera;

/// @brief �J�����ړ��N���X
class CP_CameraMove : public Component
{
	/// @brief �J�����̓����𓝐�����R���g���[���[
	std::unique_ptr<CameraMoveController> pMoveController;

	/// @brief �Ǐ]��̃I�u�W�F�N�g��
	std::string followObjName;

	/// @brief �Ǐ]�Ώ�
	const Transform* pTargetTransform;

	/// @brief �J����
	CP_Camera* pCamera;
public:
	CP_CameraMove();
	~CP_CameraMove() {}
	
	void Init() override;

	/// @brief �ΏۂƂ���Q�[���I�u�W�F�N�g���Z�b�g
	/// @param _targetTransform �Ώۂ̃I�u�W�F�N�g(nullptr�ŊO��)
	void SetTargetTransform(const Transform* _targetTransform);

	virtual void ImGuiDebug() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void Start() override;
	void LateUpdate() override;

	/// @brief �X�V�ł��邩�m�F����
	/// @return �X�V�ł��邩�H
	bool IsCanUpdate();

	/// @brief �Ǐ]��̃I�u�W�F�N�g����T��
	void FindFollowObject();

	// �^�[�Q�b�g�I�u�W�F�N�g��ImGui��
	void ImGuiSetTarget();
};

