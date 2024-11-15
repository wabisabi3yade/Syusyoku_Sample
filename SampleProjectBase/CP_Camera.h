#pragma once
#include "Component.h"

// �Q�[�����J�����N���X
class CP_Camera : public Component
{
	/// @brief �J�����̏�x�N�g��
	DirectX::SimpleMath::Vector3 camUp{ Vec3::Up };

	/// @brief ����p
	float fov;
	
	/// @brief �`�拗��
	float distance;

	/// @brief ���s���e�ɂ��邩�H
	bool isOrthographic{ false };

	// �r���[�|�[�g�̔ԍ�
	u_int viewPortSlot;
public:
	CP_Camera();
	~CP_Camera() {};

	void Init() override;
	void OnDestroy() override;

	/// @brief �r���[�ϊ��s��
	void UpdateViewMatrix();	

	/// @brief �������e�ɕύX
	void SetPerspective();

	/// @brief ���s���e�ɕύX
	void SetOrthographic();

	// fov�Z�b�g
	void SetFov(float _setFov);

	// �r���[�|�[�g�ԍ����Z�b�g
	void SetViewportSlot(u_int _slot);

	void ImGuiDebug() override;

	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�f�[�^
	nlohmann::json Save();

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data);

private:
	/// @brief ���e�s����X�V����
	void UpdateProjection();

	/// @brief �������e�s����X�V
	void UpdatePerspective();

	/// @brief ���s���e���Z�b�g����
	void UpdateOrthographic();
};

/// @brief CP_Camera��Null�I�u�W�F�N�g
class CP_CameraNull : public CP_Camera
{
public:
	CP_CameraNull() {}
	~CP_CameraNull() {}

	void Init() override {}

	void LateUpdate() override {}

	void Draw() override {}

	void ImGuiDebug() override {}

	/// @brief �r���[�ϊ��s��
	void UpdateViewMatrix() {}

	/// @brief �������e�ɕύX
	void SetPerspective() {}

	/// @brief ���s���e�ɕύX
	void SetOrthographic() {}

	// fov�Z�b�g
	void SetFov(float _setFov) {}

	// �r���[�|�[�g�ԍ����Z�b�g
	void SetViewportSlot(u_int _slot) {}

	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�f�[�^
	nlohmann::json Save() { return nlohmann::json(); }

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data) {}
};