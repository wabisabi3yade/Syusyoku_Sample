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

	/// @brief 戦闘のマネージャーコンポーネント
	class CP_BattleManager : public SingletonComponent<CP_BattleManager>
	{
	public:
		friend class BattleDirectionEndObserver;

		// 敵リスト
		using EnemyList = std::list<CP_Enemy*>;
	private:
		friend class SingletonComponent<CP_BattleManager>;

		/// @brief 敵全体リスト
		EnemyList enemyList;

		/// @brief ポーズ中に表示するボタンのオブジェクト名
		std::string poseButtonName;

		/// @brief 戦闘の演出を行う
		std::unique_ptr<BattleDirection> pBattleDirection;

		/// @brief 演出が終了したときのオブザーバー
		std::unique_ptr<BattleDirectionEndObserver> pDirectionEndObserver;

		/// @brief プレイヤーオブジェクト
		CP_Player* pPlayer{ nullptr };

		/// @brief カメラ移動クラス
		CP_CameraMove* pCamMove{ nullptr };

		/// @brief ポーズ中にボタンを表示する
		CP_BattleButtonGroup* pPoseButtons{ nullptr };

		/// @brief シーン内マネジャー
		InSceneSystemManager* pInSceneManager{ nullptr };

		/// @brief XZ座標のエリア制限
		// x : X_R,	y: X_L, z : Y_T, w : Y_B
		DXSimp::Vector4 moveAreaRect;

		/// @brief 演出開始したか
		bool isDirectionStart{ false };

		/// @brief ポーズ中か？
		bool isPosing{ false };
#ifdef EDIT
		bool isDebugDisplay{ false };
#endif // EDIT

	public:
		/// @brief 初期処理
		void Init();

		/// @brief プレイヤーオブジェクトをセット
		/// @param _playerObj プレイヤーオブジェクト
		void SetPlayer(CP_Player& _playerObj);

		/// @brief 敵オブジェクトを追加
		/// @param _enemyObj 敵オブジェクト
		void AddEnemy(CP_Enemy& _enemyObj);

		/// @brief プレイヤーを取り除く
		/// @param _playerObj プレイヤー
		void RemovePlayer(CP_Player& _playerObj);

		/// @brief 敵を取り除く
		/// @param _enemyObj 敵
		void RemoveEnemy(CP_Enemy& _enemyObj);

		/// @brief プレイヤーオブジェクトを取得する
		/// @return プレイヤー
		CP_Player* GetPlayerObject();

		/// @brief バトル中にポーズ開始する
		void BeginPose();

		/// @brief ポーズを終了する
		void EndPose();

		/// @brief 敵リストを取得する
		/// @return 
		const EnemyList& GetEnemyList();

		/// @brief プレイヤーが勝利したときの処理
		void OnPlayerWin();

		/// @brief プレイヤーが負けたときの処理
		void OnPlayerLose();

		json Save() override;
		void Load(const json& _data) override;
	private:
		void Awake() override;
		void Start() override;
		void Update() override;
		void LateUpdate() override;
		void Draw() override;

		/// @brief 必要なオブジェクトを探す
		void FindObject();

		/// @brief 入力更新
		void InputUpdate();

		/// @brief フェードを開始する
		void FadeStart();

		/// @brief バトル状態開始時の処理
		void OnBeginBattle();

		/// @brief バトルを終了する
		void EndBattle();

		/// @brief キャラクターの移動制限更新処理
		void MoveAreaUpdate();

		/// @brief 移動範囲を制限するキャラクターの移動制限を行う
		/// @param _charaTransform キャラクターのトランスフォーム
		void PositionClamp(Transform& _charaTransform);

		/// @brief オブジェクトを活動するようにする
		void ActiveObjects();

		/// @brief オブジェクトを止めるようにする
		void StopObjects();

		void ImGuiDebug() override;
	};

	/// @brief 戦闘演出終了のオブザーバークラス
	class BattleDirectionEndObserver : public IObserver<int>
	{
		CP_BattleManager* pBattleManager;

	public:
		BattleDirectionEndObserver();
		~BattleDirectionEndObserver() {}

		/// @brief 通知がきたときの処理
		void ObserverUpdate(const int& _value);
	};
}