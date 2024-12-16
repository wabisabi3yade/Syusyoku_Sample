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

	/// @brief 戦闘のマネージャーコンポーネント
	class CP_BattleManager : public SingletonComponent<CP_BattleManager>
	{
	public:
		// 敵リスト
		using EnemyList = std::list<CP_Enemy*>;

		/// @brief バトルの状態
		enum class BattleState
		{
			Wait,
			Start,	// 最初の演出
			Battle,	// 戦闘中
			Win,	// 勝利
			Lose	// 負け
		};

		int step = 0;
	private:
		friend class SingletonComponent<CP_BattleManager>;

		/// @brief ゲームスタートの速度カーブ
		AnimationCurve gameStartCurve;

		/// @brief フェードオブジェクト名
		std::string fadeObjName;

		/// @brief バトルスタートオブジェクト名
		std::string startObjName;

		/// @brief Winオブジェクト名
		std::string winObjName;

		/// @brief ゲームオーバー名
		std::string gameOverObjName;

		/// @brief 敵全体リスト
		EnemyList enemyList;

		/// @brief フェード
		CP_Fade* pFade{ nullptr };

		/// @brief バトルスタートオブジェクト
		GameObject* pGameStartObj;

		/// @brief 勝利オブジェクト
		CP_Fade* pWinObj;

		/// @brief 敗北オブジェクト
		CP_Fade* pGameOverObj;

		/// @brief プレイヤーオブジェクト
		CP_Player* pPlayer{ nullptr };

		/// @brief カメラ移動クラス
		CP_CameraMove* pCamMove;

		/// @brief 現在のバトルの状態
		BattleState curBattleState{ BattleState::Wait };

		/// @brief XZ座標のエリア制限
		// x : X_R,	y: X_L, z : Y_T, w : Y_B
		DirectX::SimpleMath::Vector4 moveAreaRect;

		/// @brief 演出の経過時間
		float animationElapsedTime{ 0.0f };

#ifdef EDIT
		bool isUIAnimation{ true };

		bool isDebugDisplay{ false };
#endif // EDIT

	public:
		/// @brief プレイヤーオブジェクトをセット
		/// @param _playerObj プレイヤーオブジェクト
		void SetPlayer(CP_Player& _playerObj);

		/// @brief 敵オブジェクトを追加
		/// @param _enemyObj 敵オブジェクト
		void AddEnemy(CP_Enemy& _enemyObj);

		/// @brief カメラ移動
		/// @param _camMove カメラ移動
		void SetCameraMove(CP_CameraMove& _camMove);

		/// @brief プレイヤーを取り除く
		/// @param _playerObj プレイヤー
		void RemovePlayer(CP_Player& _playerObj);

		/// @brief 敵を取り除く
		/// @param _enemyObj 敵
		void RemoveEnemy(CP_Enemy& _enemyObj);

		/// @brief カメラ移動を取り除く
		/// @param _enemyObj カメラ移動
		void RemoveCamMove(CP_CameraMove& _camMove);

		/// @brief プレイヤーオブジェクトを取得する
		/// @return プレイヤー
		CP_Player* GetPlayerObject();

		/// @brief 敵リストを取得する
		/// @return 
		const EnemyList& GetEnemyList();

		/// @brief 現在のステートを取得
		/// @return 現在のステート
		BattleState GetCurState() const;

		/// @brief プレイヤーが勝利したときの処理
		void OnPlayerWin();

		/// @brief プレイヤーが負けたときの処理
		void OnPlayerLose();

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		void Start() override;
		void Update() override;
		void LateUpdate() override;
		void Draw() override;

		/// @brief オブジェクトを探す
		void FindObject();

		/// @brief フェードを開始する
		void FadeStart();

		/// @brief スタート状態開始時の処理
		void OnBeginStart();

		/// @brief スタート時の更新処理
		void StartUpdate();

		/// @brief バトル状態開始時の処理
		void OnBeginBattle();

		/// @brief 勝利時の更新処理
		void WinUpdate();

		/// @brief 勝利時の更新処理
		void LoseUpdate();


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

		// スタート
		void ImGuiStart();
	};
}