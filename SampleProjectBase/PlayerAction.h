#pragma once
#include "PlayerActionController_Base.h"
#include "ITargetAccepter.h"
#include "IGroundNotifyer.h"

namespace HashiTaku
{
	class CP_Player;
	class CP_GroundCheck;

	/// @brief プレイヤーアクションクラス
	class PlayerAction :
		public StateMachine_Base<PlayerActionController_Base::ActionPlace>,
		public IObjectTargeter,
		public IImGuiUser,
		public ISaveLoad,
		public IGroundNotifyer
	{
		using ActionPlace = PlayerActionController_Base::ActionPlace;
	public:
		/// @brief プレイヤーから見た入力の方向
		enum class InputVector
		{
			Forward,	// 前方向
			Back	// 後ろ
		};

		// 空中　→　地上
		enum class AirToGround
		{
			AirToLanding,	// 着地
			AirToGroundKnock,	// 地上ノックへ
		};

		// 地上　→　空中
		enum class GroundToAir
		{
			ToAirMove,	// 空中移動へ	
		};

	private:
		/// @brief 接地チェッカーオブジェクト名
		std::string groundCheckerName;

		/// @brief 現在のアクションコントローラー
		PlayerActionController_Base* pCurActionController;

		/// @brief 地面判定を確認するコンポーネント
		CP_GroundCheck* pGroundChecker;

		/// @brief 入力クラス
		GameInput* pInput;

		/// @brief バトルマネジャー
		CP_BattleManager* pBattleManager;

		/// @brief ターゲットしているオブジェクト先
		ITargetAccepter* pTargetAccepter;

		/// @brief ターゲット先をカメラに伝える
		IObjectTargeter* pTargetCamera;

		/// @brief プレイヤー
		CP_Player* pPlayer;

		/// @brief カメラ
		CP_Camera* pCamera;

		/// @brief アニメーション
		CP_Animation* pAnimation;

		/// @brief キャンセルフラグのポインタ
		const bool* pIsCanCancel;

		/// @brief 入力可能フラグのポインタ
		const bool* pIsCanInput;

		/// @brief 攻撃可能のポインタ
		const bool* pIsCanAttack;

		/// @brief 移動可能のポインタ
		const bool* pIsCanMove;

		/// @brief 空中に変更したときにどの状態へ繋げるか変数
		GroundToAir reserveAirTransition;

		/// @brief 地上に変更したときにどの状態へ繋げるか変数
		AirToGround reserveGroundTransition;

		/// @brief 地上かどうか？
		bool isGround;

		/// @brief 1フレーム前の接地状態
		bool prevIsGround;

		/// @brief ターゲット中かどうか
		bool isTargeting;

		/// @brief 1フレーム前のターゲット状態
		bool prevIsTargeting;
	public:
		/// @brief コンストラクタ
		/// @param _player プレイヤー
		PlayerAction(CP_Player& _player);
		~PlayerAction();

		/// @brief 初期化処理
		/// @param _pAnimation アニメーションコンポーネント
		/// @param _pRigidBody Rb
		void Init(CP_Animation* _pAnimation,
			CP_RigidBody* _pRigidBody);

		/// @brief 更新処理
		void Update() override;

		/// @brief ノードを変更する
		/// @param _changeKey 変更するノードのキー
		/// @param _isForceChange 強制的に変更する
		/// @return 遷移成功したか？
		bool ChangeNode(const ActionPlace& _changeKey,
			bool _isForceChange = false) override;

		/// @brief カメラ取得
		/// @return カメラ
		CP_Camera& GetCamera();

		/// @brief ターゲット先のオブジェクトを取得
		/// @return ターゲット先のオブジェクト(nullptr)
		const ITargetAccepter* GetTargetAccepter() const;

		/// @brief コントローラーの左スティックの入力を取得
		/// @return 左スティックの入力
		const DXSimp::Vector2& GetInputLeftStick() const;

		/// @brief その方向に入力できているか確認する
		/// @param _checkVector 確認したい方向
		/// @return できているか？
		bool IsInputVector(InputVector _checkVector);

		/// @brief ターゲット中か取得する
		/// @return ターゲット中か？
		bool GetIsTargeting() const;

		/// @brief 先行入力できるかを取得
		/// @return 先行入力できるか？
		bool GetCanInput() const;

		/// @brief キャンセルできるか取得
		/// @return キャンセルできるか？
		bool GetCanCancel() const;

		/// @brief コンビネーション攻撃できるかを取得
		/// @return コンビネーション攻撃できるか？
		bool GetCanAttack() const;

		/// @brief 移動できるかを取得
		/// @return 移動できるか？
		bool GetCanMove() const;

		/// @brief 接地フラグを取得
		/// @return 接地フラグ
		bool GetIsGround() const;

		/// @brief ターゲットオブジェクトを取得する
		/// @param _targetObject ターゲットオブジェクト
		void SetTargetObject(ITargetAccepter& _targetObject) override;

		/// @brief ターゲットオブジェクトが死んだときの更新処理
		/// @param _deathTargetObj 死んだターゲットオブジェクト
		void RemoveNotify(const ITargetAccepter& _deathTargetObj) override;

		/// @brief ダメージ受けたときのアクション処理
		/// @param _atkInfo 攻撃情報
		/// @param _pAcceptDamage ダメージ受けたか？
		void OnDamage(AttackInformation& _atkInfo,
			bool* _pAcceptDamage = nullptr);

		/// @brief 地面に付いた瞬間の処理
		void OnGroundEnter() override;

		/// @brief 地面から離れた瞬間の処理
		void OnGroundExit()	override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 接地チェッカーを探して取得
		void FindGroundChecker();

		/// @brief  ターゲット開始時のお処理
		void OnBeginTargeting();

		/// @brief ターゲット終了時の処理
		void OnEndTargeting();

		/// @brief ターゲットの更新処理
		void UpdateTargeting();

		/// @brief 接地状態の更新処理
		void UpdateGround();

		/// @brief 地上から空中への処理
		void OnGroundToAir();

		/// @brief 空中から地上への処理
		void OnAirToGround();

		/// @brief 指定したアクションコントローラーを取得
		/// @param _place 指定する場所
		/// @return アクションコントローラー
		PlayerActionController_Base* GetActionController(ActionPlace _place);

		void ImGuiDebug() override;

		// 接地チェッカー
		void ImGuiGroundChecker();
	};
}