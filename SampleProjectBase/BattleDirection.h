#pragma once
#include "Component.h"
#include "AnimationCurve.h"
#include "IObserever.h"

namespace HashiTaku
{
	class CP_Fade;
	class CP_CameraMove;

	/// @brief �퓬�̉��o���s��
	class BattleDirection : public ISaveLoad, public IImGuiUser, public Subject<int>
	{
	public:
		/// @brief �o�g���̏��
		enum class DirectionState
		{
			Wait,
			Start,	// �ŏ��̉��o
			Win,	// ����
			Lose	// ����
		};

	private:
		/// @brief �Q�[���X�^�[�g�̑��x�J�[�u
		AnimationCurve gameStartCurve;

		/// @brief �t�F�[�h�I�u�W�F�N�g��
		std::string fadeObjName;

		/// @brief �o�g���X�^�[�g�I�u�W�F�N�g��
		std::string startObjName;

		/// @brief Win�I�u�W�F�N�g��
		std::string winObjName;

		/// @brief �Q�[���I�[�o�[��
		std::string gameOverObjName;

		/// @brief �t�F�[�h
		CP_Fade* pFade;

		/// @brief �o�g���X�^�[�g�I�u�W�F�N�g
		GameObject* pGameStartObj;

		/// @brief ��������
		CP_Fade* pWinObj;

		/// @brief �s�k����
		CP_Fade* pGameOverObj;

		/// @brief �J�����ړ��N���X
		CP_CameraMove* pCamMove;

		/// @brief ���݂̏��
		DirectionState curState;

		/// @brief ���o�̌o�ߎ���
		float animationElapsedTime;

		/// @brief ���o�̒i�K
		int directionStep;

		/// @brief UI�̃A�j���[�V�������s�����H
		bool isUIAnimation;
	public:
		BattleDirection();
		~BattleDirection() {}

		/// @brief ��������
		void Init(CP_CameraMove* _pCamMove);

		/// @brief �X�V����
		/// @param _deltaTime �o�ߎ���
		void Update(float _deltaTime);

		/// @brief �J�n�̉��o���n�߂�
		void OnBeginStartDirection();

		/// @brief �������̉��o���n�߂�
		/// @param _targetTransform �J�������f���^�[�Q�b�g�̃g�����X�t�H�[��
		void OnBeginWinDirection(const Transform* _targetTransform);

		/// @brief �s�k���̉��o���n�߂�
		/// @param _targetTransform �J�������f���^�[�Q�b�g�̃g�����X�t�H�[��
		void OnBeginLoseDirection(const Transform* _targetTransform);

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;
	private:
		/// @brief �I�u�W�F�N�g��T��
		void FindObject();

		/// @brief �X�^�[�g��ԊJ�n���̏���
		void OnBeginStart();
		
		/// @brief �J�n���̍X�V����
		/// @param _deltaTime �o�ߎ���
		void StartUpdate(float _deltaTime);

		/// @brief �������̍X�V����
		/// @param _deltaTime �o�ߎ���
		void WinUpdate(float _deltaTime);

		/// @brief �s�k���̍X�V����
		/// @param _deltaTime �o�ߎ���
		void LoseUpdate(float _deltaTime);

		/// @brief ���o�I���̏���
		void OnEndDirection();

		void ImGuiDebug() override;
	};
}


