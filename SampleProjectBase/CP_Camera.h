#pragma once
#include "CloneComponent.h"

// �Q�[�����J�����N���X
class CP_Camera : public Component, public CloneComponent<CP_Camera>
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
	~CP_Camera() {}

	void Init() override;

	void LateUpdate() override;

	void Draw() override;

	void ImGuiSetting() override;

	/// @brief �r���[�ϊ��s��
	void UpdateViewMatrix();	

	/// @brief �������e�ɕύX
	void SetPerspective();

	/// @brief ���s���e�ɕύX
	void SetOrthographic();

	// �r���[�|�[�g�ԍ����Z�b�g
	void SetViewportSlot(u_int _slot);

	/// @brief �Z�[�u����
	/// @param _sceneData �Z�[�u�f�[�^
	nlohmann::json Save();

	/// @brief ���[�h����
	/// @param _sceneData ���[�h����f�[�^ 
	void Load(const nlohmann::json& _data);

private:
	/// @brief �������e�s����X�V
	void UpdatePerspective();

	/// @brief ���s���e���Z�b�g����
	void UpdateOrthographic();
};

