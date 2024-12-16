#pragma once
#include "SingletonComponent.h"
#include "ITargetAccepter.h"
#include "AnimationCurve.h"

namespace HashiTaku
{
	class CP_Player;
	class CP_Enemy;
	class CP_CameraMove;
	class CP_Fade;

	/// @brief �퓬�̃}�l�[�W���[�R���|�[�l���g
	class CP_BattleManager : public SingletonComponent<CP_BattleManager>
	{
	public:
		// �G���X�g
		using EnemyList = std::list<CP_Enemy*>;

		/// @brief �o�g���̏��
		enum class BattleState
		{
			Wait,
			Start,	// �ŏ��̉��o
			Battle,	// �퓬��
			Win,	// ����
			Lose	// ����
		};

		int step = 0;
	private:
		friend class SingletonComponent<CP_BattleManager>;

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

		/// @brief �G�S�̃��X�g
		EnemyList enemyList;

		/// @brief �t�F�[�h
		CP_Fade* pFade{ nullptr };

		/// @brief �o�g���X�^�[�g�I�u�W�F�N�g
		GameObject* pGameStartObj;

		/// @brief �����I�u�W�F�N�g
		CP_Fade* pWinObj;

		/// @brief �s�k�I�u�W�F�N�g
		CP_Fade* pGameOverObj;

		/// @brief �v���C���[�I�u�W�F�N�g
		CP_Player* pPlayer{ nullptr };

		/// @brief �J�����ړ��N���X
		CP_CameraMove* pCamMove;

		/// @brief ���݂̃o�g���̏��
		BattleState curBattleState{ BattleState::Wait };

		/// @brief XZ���W�̃G���A����
		// x : X_R,	y: X_L, z : Y_T, w : Y_B
		DirectX::SimpleMath::Vector4 moveAreaRect;

		/// @brief ���o�̌o�ߎ���
		float animationElapsedTime{ 0.0f };

#ifdef EDIT
		bool isUIAnimation{ true };

		bool isDebugDisplay{ false };
#endif // EDIT

	public:
		/// @brief �v���C���[�I�u�W�F�N�g���Z�b�g
		/// @param _playerObj �v���C���[�I�u�W�F�N�g
		void SetPlayer(CP_Player& _playerObj);

		/// @brief �G�I�u�W�F�N�g��ǉ�
		/// @param _enemyObj �G�I�u�W�F�N�g
		void AddEnemy(CP_Enemy& _enemyObj);

		/// @brief �J�����ړ�
		/// @param _camMove �J�����ړ�
		void SetCameraMove(CP_CameraMove& _camMove);

		/// @brief �v���C���[����菜��
		/// @param _playerObj �v���C���[
		void RemovePlayer(CP_Player& _playerObj);

		/// @brief �G����菜��
		/// @param _enemyObj �G
		void RemoveEnemy(CP_Enemy& _enemyObj);

		/// @brief �J�����ړ�����菜��
		/// @param _enemyObj �J�����ړ�
		void RemoveCamMove(CP_CameraMove& _camMove);

		/// @brief �v���C���[�I�u�W�F�N�g���擾����
		/// @return �v���C���[
		CP_Player* GetPlayerObject();

		/// @brief �G���X�g���擾����
		/// @return 
		const EnemyList& GetEnemyList();

		/// @brief ���݂̃X�e�[�g���擾
		/// @return ���݂̃X�e�[�g
		BattleState GetCurState() const;

		/// @brief �v���C���[�����������Ƃ��̏���
		void OnPlayerWin();

		/// @brief �v���C���[���������Ƃ��̏���
		void OnPlayerLose();

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		void Start() override;
		void Update() override;
		void LateUpdate() override;
		void Draw() override;

		/// @brief �I�u�W�F�N�g��T��
		void FindObject();

		/// @brief �t�F�[�h���J�n����
		void FadeStart();

		/// @brief �X�^�[�g��ԊJ�n���̏���
		void OnBeginStart();

		/// @brief �X�^�[�g���̍X�V����
		void StartUpdate();

		/// @brief �o�g����ԊJ�n���̏���
		void OnBeginBattle();

		/// @brief �������̍X�V����
		void WinUpdate();

		/// @brief �������̍X�V����
		void LoseUpdate();


		/// @brief �L�����N�^�[�̈ړ������X�V����
		void MoveAreaUpdate();

		/// @brief �ړ��͈͂𐧌�����L�����N�^�[�̈ړ��������s��
		/// @param _charaTransform �L�����N�^�[�̃g�����X�t�H�[��
		void PositionClamp(Transform& _charaTransform);

		/// @brief �I�u�W�F�N�g����������悤�ɂ���
		void ActiveObjects();

		/// @brief �I�u�W�F�N�g���~�߂�悤�ɂ���
		void StopObjects();

		void ImGuiDebug() override;

		// �X�^�[�g
		void ImGuiStart();
	};
}