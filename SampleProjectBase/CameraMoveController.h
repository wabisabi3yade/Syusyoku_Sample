#pragma once
#include "CameraMoveState_Base.h"
#include "CalcPerlinShakeVector.h"
#include "ITargetAccepter.h"

namespace HashiTaku
{
	/// @brief �J�����̓���
	class CameraMoveController :
		public StateMachine_Base<CameraMoveState_Base::CameraState>,
		public IImGuiUser,
		public ISaveLoad
	{
		using CameraState = CameraMoveState_Base::CameraState;
	private:
		/// @brief �p�[�����V�F�C�N
		PerlinShake perlinShake;

		/// @brief �J�����̌��݂̍��W(�J�����h�����������̂ŃI�u�W�F�N�g�ƕʂŎ����Ă���)
		DXSimp::Vector3 curBaseCameraPos;

		/// @brief �O�t���[���̃t�H���[���Ă�����W
		DXSimp::Vector3 prevFollowPos;

		/// @brief �J�����R���|�[�l���g
		CP_Camera* pCamera;

		/// @brief �J�����̒Ǐ]�I�u�W�F�N�g
		const Transform* pFollowTransform;

		/// @brief �J�����̒����I�u�W�F�N�g
		const ITargetAccepter* pLookAtTransform;
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

		/// @brief �h����J�n����
		/// @param _shakeParam �J�����h��p�����[�^
		void BeginShake(const PerlinShakeParameter& _shakeParam);

		/// @brief �V�F�C�N���~�߂�
		void StopShake();

		/// @brief �^�[�Q�b�g�Ƃ���I�u�W�F�N�g���Z�b�g�i�O�������ꍇnullptr�j
		/// @param _pTransform �Q�[���I�u�W�F�N�g 
		void SetFollowTransform(const Transform* _pTransform);

		/// @brief ��������I�u�W�F�N�g���Z�b�g�i�O�������ꍇnullptr�j
		/// @param _pTransform �Q�[���I�u�W�F�N�g 
		void SetLookAtObject(const ITargetAccepter* _pTransform);

		/// @brief ���݂̃J�����̃x�[�X���W���X�V
		/// @param _camPos �x�[�X�ƂȂ�J�����̍��W
		void SetCameraBasePos(const DXSimp::Vector3& _camPos);

		/// @brief ����p�̊p�x���Z�b�g(�P�ʁFdeg)
		/// @param _setDegree ����p
		void SetFov(float _setDegree);

		void OnPlayerWin(const Transform& _targetTransform);

		/// @brief ����p���擾(�P�ʁFdeg)
		/// @return ����p
		float GetFov() const;

		/// @brief �J�������擾
		/// @return �J�����R���|�[�l���g
		CP_Camera& GetCamera();

		/// @brief �J�����h����l�����Ȃ��J�����̍��W���擾
		/// @return �J�����h����l�����Ȃ��J�������W
		const DXSimp::Vector3& GetCameraBasePos();

		/// @brief �O�t���[���̒Ǐ]����W���擾
		/// @return �O�t���[���̒Ǐ]����W
		const DXSimp::Vector3& GetPrevFollowPos() const;

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
		const DXSimp::Vector3& GetLookAtWorldPos() const;

		json Save() override;
		void Load(const json& _data) override;
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

		/// @brief �J�����̗񋓌^�𕶎����
		/// @param _state �ύX�������񋓌^
		/// @return ������
		std::string GetStateName(CameraMoveState_Base::CameraState _state);

		/// @brief �J�����s���ɃL���X�g����
		/// @param _stateBase �X�e�[�g�m�[�h�̊��
		/// @return �J�����X�e�[�g�N���X
		CameraMoveState_Base& CastCamState(StateNode_Base& _stateBase);

		void ImGuiDebug() override;

		// �S�X�e�[�g�����[�h����
		void LoadStates(const json& _data);
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
}