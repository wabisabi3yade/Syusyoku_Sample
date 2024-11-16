#pragma once
#include "StateMachine.h"
#include "CP_Camera.h"

class CameraMoveController;

/// @brief �J�����̍s�����N���X
class CameraMoveState_Base : public HashiTaku::StateNode_Base,
	public HashiTaku::ISaveLoad, public HashiTaku::IImGuiUser
{
public:
	enum  class CameraState
	{
		Move,	// �ړ���
		Target,	// �^�[�Q�b�g��
		None
	};

private:
	/// @brief �J�����̍s���R���g���[��
	CameraMoveController* pCamController;

public:
	CameraMoveState_Base();
	virtual ~CameraMoveState_Base() {}

	/// @brief ����������
	/// @param _cameraController �J�����R���g���[�� 
	void Init(CameraMoveController& _cameraController);

	/// @brief ��Ԑ؂�ւ��J�n����
	void OnStart() override;

	/// @brief �X�V����
	void Update() override;

	/// @brief ��Ԑ؂�ւ��I������
	void OnEnd() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override {};
protected:
	/// @brief �J�n�����̐U�镑��
	virtual void OnStartBehavior() {}

	/// @brief �X�V�����̐U�镑��
	virtual void UpdateBehavior() {}

	/// @brief �I�������̐U�镑��
	virtual void OnEndBehavior() {}

	/// @brief �J�����R���|�[�l���g���擾
	/// @return �J����
	CP_Camera& GetCamera();

	/// @brief �^�[�Q�b�g�Ƃ�����W���擾����
	/// @return �^�[�Q�b�g�̃��[���h�����W
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const;

	/// @brief �I�u�W�F�N�g�̌o�ߎ��Ԃ��擾
	/// @return �o�ߎ���
	float DeltaTime() const;

	void ImGuiDebug() override {}
};

