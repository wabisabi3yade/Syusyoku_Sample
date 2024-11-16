#pragma once
#include "CameraMoveState_Base.h"

/// @brief �J�����̓���
class CameraMoveController : 
	public HashiTaku::StateMachine_Base<CameraMoveState_Base::CameraState>,
	public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief �J�����R���|�[�l���g
	CP_Camera* pCamera;

	/// @brief �J�����̑ΏۃI�u�W�F�N�g
	const Transform* pTargetTransform;
public:
	CameraMoveController();
	~CameraMoveController() {}

	/// @brief �J�n����
	/// @param _camera �J�����R���|�[�l���g
	void Begin(CP_Camera& _camera);

	/// @brief �X�e�[�g��ύX
	/// @param _cameraState �ύX��̃X�e�[�g
	/// @param _isForce �����I�ɕύX���邩
	void ChangeState(CameraMoveState_Base::CameraState _cameraState, bool _isForce = false);

	/// @brief �^�[�Q�b�g�Ƃ���I�u�W�F�N�g���Z�b�g�i�O�������ꍇnullptr�j
	/// @param _pTransform �Q�[���I�u�W�F�N�g 
	void SetTargetTransform(const Transform* _pTransform);

	/// @brief �J�������擾
	/// @return �J�����R���|�[�l���g
	CP_Camera& GetCamera();

	/// @brief �ΏۃI�u�W�F�N�g���擾
	/// @return �ΏۃI�u�W�F�N�g
	const Transform* GetTargetTransform() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �X�V����
	void Update() override;

	/// @brief �J�����̃X�e�[�g���쐬
	/// @tparam T �Ή������m�[�h
	/// @param _stateType �X�e�[�g�̎��
	template<class T> void CreateState(CameraMoveState_Base::CameraState _stateType);

	/// @brief �J�����̗񋓌^�𕶎����
	/// @param _state �ύX�������񋓌^
	/// @return ������
	std::string GetStateName(CameraMoveState_Base::CameraState _state);

	/// @brief �J�����s���ɃL���X�g����
	/// @param _stateBase �X�e�[�g�m�[�h�̊��
	/// @return �J�����X�e�[�g�N���X
	CameraMoveState_Base& CastCamState(HashiTaku::StateNode_Base& _stateBase);

	void ImGuiDebug() override;

	// �S�X�e�[�g�����[�h����
	void LoadStates(const nlohmann::json& _data);
};

template<class T>
inline void CameraMoveController::CreateState(CameraMoveState_Base::CameraState _stateType)
{
	// �X�e�[�g���쐬
	std::unique_ptr<CameraMoveState_Base> pCameraMove =
		std::make_unique<T>();

	pCameraMove->Init(*this);

	// �ǉ�
	AddNode(_stateType, std::move(pCameraMove));
}
