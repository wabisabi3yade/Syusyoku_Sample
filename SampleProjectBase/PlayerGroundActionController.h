#pragma once
#include "PlayerActionController_Base.h"
#include "PlayerGroundState.h"

namespace HashiTaku
{
	/// @brief プレイヤーの地上行動を継承しているかコンセプト
	template<typename T>
	concept PlayerGroundConcept =
		std::is_base_of_v<PlayerGroundState, T>;

	/// @brief プレイヤーの地上行動コントローラー
	class PlayerGroundActionController : public PlayerActionController_Base
	{
	public:
		using GroundState = PlayerGroundState::PlayerState;

		/// @brief コンストラクタ
		/// @param _player プレイヤーコンポーネント
		PlayerGroundActionController(PlayerAction& _pAction, CP_Player& _player);
		~PlayerGroundActionController();

		/// @brief 開始処理
		/// @param _animation アニメーション
		/// @param _pRigidBody リジッドボディ
		/// @param ターゲット時に一緒に見るカメラ
		void Init(CP_Animation* _animation, CP_RigidBody* _pRigidBody) override;

		/// @brief 現在の更新処理
		void Update() override;

		/// @brief 行動状態を切り替える
		/// @param _nextActionName 次の状態の名前
		/// @param _isForce 強制で切り替えするか？
		bool ChangeGroundState(const GroundState& _nextActionState, bool _isForce = false);

		/// @brief ダメージ受けたときのアクション処理
		/// @param _atkInfo 攻撃情報
		/// @param _pAcceptDamage ダメージ受けたか？
		void OnDamage(AttackInformation& _atkInfo,
			bool* _pAcceptDamage = nullptr) override;

		/// @brief ターゲット中か取得する
		/// @return ターゲット中か？
		bool GetIsTargeting() const;

		/// @brief カメラ取得
		/// @return カメラ
		CP_Camera& GetCamera();

		/// @brief 現在の地上行動のステートを取得する
		/// @return 地上行動のステート
		PlayerGroundState* GetCurrentGroundState();


		/// @brief 現在のプレイヤーの状態タイプを取得する
		/// @return 現在のプレイヤーの状態タイプ
		PlayerGroundState::PlayerState GetCurrentStateType() const;

		/// @brief 各Stateの文字列を取得する
		/// @param _stateId 状態のID
		/// @return 文字列
		std::string GetStateStr(int _stateId) override;

		/// @brief 各StateのIDを取得する
		/// @param _stateName　状態名
		/// @return 状態のID
		int GetStateId(const std::string& _stateName);
	private:
		/// @brief 新しくStateを生成
		/// @tparam T 対応している行動クラス
		/// @param _actionState アクション名
		/// @param _cancelType キャンセルタイプ
		template <PlayerGroundConcept T> void CreateState(GroundState _actionState,
			CancelType _cancelType);

		/// @brief 更新できるか取得
		/// @return 更新できるか？
		bool GetCanUpdate();

		/// @brief ノック状態に遷移
		/// @param _atkInfo 攻撃情報
		/// @param _attackerPos 攻撃敵の座標
		void ChangeKnockState(const AttackInformation& _atkInfo);

		void ImGuiDebug() override;
	private:
		/// @brief アニメーションコントローラ内のプレイヤー名
		static constexpr  auto STATEANIM_PARAMNAME{ "state" };
	};

	template<PlayerGroundConcept T>
	inline void PlayerGroundActionController::CreateState(GroundState _actionState,
		CancelType _cancelType)
	{
		PlayerActionController_Base::CreateState<T>(static_cast<int>(_actionState),
			_cancelType);
	}
}