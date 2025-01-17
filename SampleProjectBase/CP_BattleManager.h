#pragma once
#include "SingletonComponent.h"
#include "ITargetAccepter.h"
#include "AnimationCurve.h"
#include "BattleDirection.h"

namespace HashiTaku
{
	class CP_Player;
	class CP_Enemy;
	class CP_CameraMove;
	class CP_BattleButtonGroup;
	class BattleDirectionEndObserver;
	class InSceneSystemManager;

	/// @brief �퓬�̃}�l�[�W���[�R���|�[�l���g
	class CP_BattleManager : public SingletonComponent<CP_BattleManager>
	{
	public:
		friend class BattleDirectionEndObserver;

		// �G���X�g
		using EnemyList = std::list<CP_Enemy*>;
	private:
		friend class SingletonComponent<CP_BattleManager>;

		/// @brief �G�S�̃��X�g
		EnemyList enemyList;

		/// @brief �|�[�Y���ɕ\������{�^���̃I�u�W�F�N�g��
		std::string poseButtonName;

		/// @brief �퓬�̉��o���s��
		std::unique_ptr<BattleDirection> pBattleDirection;

		/// @brief ���o���I�������Ƃ��̃I�u�U�[�o�[
		std::unique_ptr<BattleDirectionEndObserver> pDirectionEndObserver;

		/// @brief �v���C���[�I�u�W�F�N�g
		CP_Player* pPlayer{ nullptr };

		/// @brief �J�����ړ��N���X
		CP_CameraMove* pCamMove{ nullptr };

		/// @brief �|�[�Y���Ƀ{�^����\������
		CP_BattleButtonGroup* pPoseButtons{ nullptr };

		/// @brief �V�[�����}�l�W���[
		InSceneSystemManager* pInSceneManager{ nullptr };

		/// @brief XZ���W�̃G���A����
		// x : X_R,	y: X_L, z : Y_T, w : Y_B
		DXSimp::Vector4 moveAreaRect;

		/// @brief ���o�J�n������
		bool isDirectionStart{ false };

		/// @brief �|�[�Y�����H
		bool isPosing{ false };
#ifdef EDIT
		bool isDebugDisplay{ false };
#endif // EDIT

	public:
		/// @brief ��������
		void Init();

		/// @brief �v���C���[�I�u�W�F�N�g���Z�b�g
		/// @param _playerObj �v���C���[�I�u�W�F�N�g
		void SetPlayer(CP_Player& _playerObj);

		/// @brief �G�I�u�W�F�N�g��ǉ�
		/// @param _enemyObj �G�I�u�W�F�N�g
		void AddEnemy(CP_Enemy& _enemyObj);

		/// @brief �v���C���[����菜��
		/// @param _playerObj �v���C���[
		void RemovePlayer(CP_Player& _playerObj);

		/// @brief �G����菜��
		/// @param _enemyObj �G
		void RemoveEnemy(CP_Enemy& _enemyObj);

		/// @brief �v���C���[�I�u�W�F�N�g���擾����
		/// @return �v���C���[
		CP_Player* GetPlayerObject();

		/// @brief �o�g�����Ƀ|�[�Y�J�n����
		void BeginPose();

		/// @brief �|�[�Y���I������
		void EndPose();

		/// @brief �G���X�g���擾����
		/// @return 
		const EnemyList& GetEnemyList();

		/// @brief �v���C���[�����������Ƃ��̏���
		void OnPlayerWin();

		/// @brief �v���C���[���������Ƃ��̏���
		void OnPlayerLose();

		json Save() override;
		void Load(const json& _data) override;
	private:
		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;
		void Draw() override;

		/// @brief �K�v�ȃI�u�W�F�N�g��T��
		void FindObject();

		/// @brief ���͍X�V
		void InputUpdate();

		/// @brief �t�F�[�h���J�n����
		void FadeStart();

		/// @brief �o�g����ԊJ�n���̏���
		void OnBeginBattle();

		/// @brief �o�g�����I������
		void EndBattle();

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
	};

	/// @brief �퓬���o�I���̃I�u�U�[�o�[�N���X
	class BattleDirectionEndObserver : public IObserver<int>
	{
		CP_BattleManager* pBattleManager;

	public:
		BattleDirectionEndObserver();
		~BattleDirectionEndObserver() {}

		/// @brief �ʒm�������Ƃ��̏���
		void ObserverUpdate(const int& _value);
	};
}