#pragma once
#include "CharacterActionController.h"
#include "PlayerActState_Base.h"

namespace HashiTaku
{
	class CP_Player;
	class PlayerAction;
	class CP_BattleManager;

	/// @brief プレイヤーの共通アクションコントローラー
	class PlayerActionController_Base : public CharacterActionController
	{
	public:
		/// @brief プレイヤーから見た入力の方向
		enum class ActionPlace
		{
			Ground,	// 地面
			Air	// 空中
		};
	private:
		/// @brief キャンセル可能になったときの変更先のステート(予約)
		int reserveCancelState;

		/// @brief 今フレームで押されたステート
		int curFlameCancelState;
	protected:
		using CancelType = PlayerActState_Base::CancelType;

		/// @brief どの場所のコントローラーか
		ActionPlace place;

		/// @brief プレイヤーアクションのグループ変数
		PlayerAction* pAction;

		/// @brief 入力変数
		const GameInput* pInput;
	public:
		PlayerActionController_Base(PlayerAction& _pAction, 
			CP_Player& _player,
			const std::string& _controllerName);

		virtual ~PlayerActionController_Base() {}

		/// @brief 初期化処理
		/// @param _pAnimation アニメーション
		/// @param _pRigidBody Rb
		void Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody) override;

		/// @brief 更新処理
		void Update() override;

		/// @brief  終了処理
		void OnEnd();

		/// @brief 予約状態をセットする
		/// @param _setState 予約する状態
		void SetReserveState(int _setState);

		/// @brief プレイヤーのステートを取得
		/// @param _playerStateId 取得したいステートのID
		/// @return プレイヤーのアクション
		PlayerActState_Base* GetPlayerAction(int _playerStateId);

		const ITargetAccepter* GetTargetAccepter() const;

		/// @brief プレイヤーにキャストして取得
		/// @return プレイヤー 
		CP_Player& GetPlayer();

		/// @brief カメラを取得する
		/// @return カメラ
		CP_Camera& GetCamera();

		/// @brief ターゲット状態か取得
		/// @return ターゲット状態か？
		bool GetIsTargeting() const;

		/// @brief 入力できるか取得
		/// @return 入力できるか？
		bool GetCanInput() const;

		/// @brief 攻撃できるか取得
		/// @return 攻撃できるか？
		bool GetCanAttack() const;

		/// @brief アクションできるか取得
		/// @return アクションできるか？
		bool GetCanAction() const;

		/// @brief 移動できるか取得
		/// @return 移動できるか？
		bool GetCanMove() const;
	protected:
		/// @brief 新しくStateを生成
		/// @tparam T 対応している行動クラス
		/// @param _actionName アクション名
		/// @param _cancelType キャンセルタイプ
		template <class T> void CreateState(int _actionState, CancelType _cancelType);

		/// @brief 状態を遷移
		/// @param _nextActionState アクションのID
		/// @param _isForce 強制して変更するか？
		bool ChangeState(int _nextActionState, bool _isForce = false);
	private:
		/// @brief 更新できるか取得
		/// @return 更新できるか？
		bool GetCanUpdate();

		/// @brief 指定したステートの優先度を取得
		/// @param _stateId ステートのId
		/// @return ステートの優先度(なければ-99)
		int GetStatePriority(int _stateId) const;

		/// @brief 予約状態からステートを変更する更新処理
		void ChangeStateFromReserve();

		/// @brief 予約状態をリセット
		void ResetReserveState();

		/// @brief 毎ループ初めに予約状態フラグをリセット行う
		void ResetReservedFlag();

		/// @brief 各Stateの文字列を取得する
		/// @param _stateId 状態のID
		/// @return 文字列
		virtual std::string GetStateStr(int _stateId) = 0;

		/// @brief 各StateのIDを取得する
		/// @param _stateName　状態名
		/// @return 状態のID
		virtual int GetStateId(const std::string& _stateName) = 0;
	};

	template<class T>
	inline void PlayerActionController_Base::CreateState(int _actionState, CancelType _cancelType)
	{
		int hasStateCnt = static_cast<int>(stateNodeList.size());

		std::unique_ptr<PlayerActState_Base> createState = std::make_unique<T>();
		// ステートの数から優先度をつけていく
		createState->Init(*this, _cancelType, hasStateCnt);

		AddNode(static_cast<int>(_actionState), std::move(createState));
	}
}