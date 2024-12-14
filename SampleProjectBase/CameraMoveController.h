#pragma once
#include "CameraMoveState_Base.h"
#include "ITargetAccepter.h"

/// @brief �J�����̓���
class CameraMoveController :
	public HashiTaku::StateMachine_Base<CameraMoveState_Base::CameraState>,
	public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	using CameraState = CameraMoveState_Base::CameraState;

public:
	/// @brief �J�����̗h��̋���
	enum class ShakeLevel
	{
		Low,
		Mid,
		High,
		SuperHigh,
		MaxNum
	};

	/// @brief �J�����̗h��̋���
	enum class ShakeType
	{
		Vertical,	// �c�h��
		Horizon,	// ���h��
		Both,	// �����h��
		MaxNum
	};

	/// @brief ���x�����Ƃɐݒ肷��p�����[�^
	struct CameraShakeParameter
	{
		float power{ 0.2f };	// �h��̋���
		float duration{ 0.3f };	// �h��̊Ԋu����(s)
	};
private:

	/// @brief �e���x���̗h��̋���
	std::array<CameraShakeParameter, static_cast<u_int>(ShakeLevel::MaxNum)> shakeLevelParameter;

	/// @brief �J�����̌��݂̍��W(�J�����h�����������̂ŃI�u�W�F�N�g�ƕʂŎ����Ă���)
	DirectX::SimpleMath::Vector3 curBaseCameraPos;

	/// @brief ���݂̃J�����h��ł��炵�Ă���I�t�Z�b�g���W
	DirectX::SimpleMath::Vector3 curShakeOffset;

	/// @brief �O�t���[���̃t�H���[���Ă�����W
	DirectX::SimpleMath::Vector3 prevFollowPos;

	/// @brief �J�����R���|�[�l���g
	CP_Camera* pCamera;

	/// @brief �J�����̒Ǐ]�I�u�W�F�N�g
	const Transform* pFollowTransform;

	/// @brief �J�����̒����I�u�W�F�N�g
	const ITargetAccepter* pLookAtTransform;

	/// @brief �h�炵�Ă鋭��
	ShakeLevel curShakeLevel;

	/// @brief �h�炵�Ă���
	ShakeType curShakeType;

	/// @brief �h�炷����
	float shakeTime;

	/// @brief �h�炵�Ă���̌o�ߎ���
	float shakeElapsedTime;

	/// @brief �O�h�ꂽ�Ƃ��̎���
	float prevShakeElapsedTime;

	/// @brief �h��Ă��邩�H
	bool isShaking;

	ShakeLevel testLevel = ShakeLevel::Low;
	ShakeType testType = ShakeType::Both;
	float testTime = 0.0f;
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
	void SetFollowTransform(const Transform* _pTransform);

	/// @brief ��������I�u�W�F�N�g���Z�b�g�i�O�������ꍇnullptr�j
	/// @param _pTransform �Q�[���I�u�W�F�N�g 
	void SetLookAtObject(const ITargetAccepter* _pTransform);

	/// @brief ���݂̃J�����̃x�[�X���W���X�V
	/// @param _camPos �x�[�X�ƂȂ�J�����̍��W
	void SetCameraBasePos(const DirectX::SimpleMath::Vector3& _camPos);

	/// @brief ����p�̊p�x���Z�b�g(�P�ʁFdeg)
	/// @param _setDegree ����p
	void SetFov(float _setDegree);

	/// @brief �h����J�n����
	/// @param _level �h��̋���
	/// @param _type �h��̎��
	/// @param _time �h��̎���
	void BeginShake(ShakeLevel _level, ShakeType _type, float _time);

	/// @brief �v���C���[�������̏���
	/// @param _targetTransform �^�[�Q�b�g�̃g�����X�t�H�[��
	void OnPlayerWin(const Transform& _targetTransform);

	/// @brief ����p���擾(�P�ʁFdeg)
	/// @return ����p
	float GetFov() const;

	/// @brief �J�������擾
	/// @return �J�����R���|�[�l���g
	CP_Camera& GetCamera();

	/// @brief �J�����h����l�����Ȃ��J�����̍��W���擾
	/// @return �J�����h����l�����Ȃ��J�������W
	const DirectX::SimpleMath::Vector3& GetCameraBasePos();

	/// @brief �O�t���[���̒Ǐ]����W���擾
	/// @return �O�t���[���̒Ǐ]����W
	const DirectX::SimpleMath::Vector3& GetPrevFollowPos() const;

	/// @brief �Ǐ]��̃I�u�W�F�N�g���������Ă��邩�擾
	/// @return �������Ă��邩�H
	bool GetHasFollowObject() const;

	/// @brief �ΏۃI�u�W�F�N�g���擾
	/// @return �ΏۃI�u�W�F�N�g
	const Transform* GetFollowTransform() const;

	/// @brief �^�[�Q�b�g�����邩�擾
	/// @return �^�[�Q�b�g�����邩�H
	bool GetHaveTarget() const;

	/// @brief ������I�u�W�F�N�g���擾
	/// @return ������I�u�W�F�N�g�̃��[���h���W
	const DirectX::SimpleMath::Vector3& GetLookAtWorldPos() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �J�����̍��W��������
	void InitCameraPos();

	/// @brief �X�V����
	void Update() override;

	/// @brief �J�����h��̍X�V
	void ShakeUpdate();

	/// @brief �ŏI�̍��W���X�V����
	void UpdateFinalPos();

	/// @brief �J�����̃X�e�[�g���쐬
	/// @tparam T �Ή������m�[�h
	/// @param _stateType �X�e�[�g�̎��
	template<class T> void CreateState(CameraMoveState_Base::CameraState _stateType);

	/// @brief �h��̏I������
	void OnEndShake();

	/// @brief �J�����̗񋓌^�𕶎����
	/// @param _state �ύX�������񋓌^
	/// @return ������
	std::string GetStateName(CameraMoveState_Base::CameraState _state);

	/// @brief �h��̃I�t�Z�b�g�l���擾����
	/// @param _level ���x��
	/// @param _type ���
	/// @return �I�t�Z�b�g�l
	DirectX::SimpleMath::Vector3 CalcShakeOffset(ShakeLevel _level, ShakeType _type);

	/// @brief �J�����s���ɃL���X�g����
	/// @param _stateBase �X�e�[�g�m�[�h�̊��
	/// @return �J�����X�e�[�g�N���X
	CameraMoveState_Base& CastCamState(HashiTaku::StateNode_Base& _stateBase);

	void TestShakeUpdate();

	void ImGuiDebug() override;

	// �h��̃��x������
	void ImGuiShakeLevel();

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
