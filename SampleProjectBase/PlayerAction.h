#pragma once
#include "PlayerGroundActionController.h"
#include "PlayerAirActionController.h"
#include "ITargetAccepter.h"
#include "IGroundNotifyer.h"

class CP_Player;
class CP_GroundCheck;

/// @brief プレイヤーアクションクラス
class PlayerAction : 
	public IObjectTargeter,
	public HashiTaku::IImGuiUser,
	public HashiTaku::ISaveLoad,
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

	/// @brief 地面判定を確認するコンポーネント
	CP_GroundCheck* pGroundChecker;

	/// @brief 地上行動のアクションコントローラー
	std::unique_ptr<PlayerGroundActionController>
		pGroundController;

	/// @brief 空中行動のアクションコントローラー
	std::unique_ptr<PlayerAirActionController>
		pAirController;

	/// @brief 現在使用しているコントローラークラス
	PlayerActionController_Base* pCurrentController;

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

	/// @brief 現在の場所属性
	ActionPlace currentPlace;

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

	void Init(CP_Animation* _pAnimation,
		CP_RigidBody* _pRigidBody);

	/// @brief 更新処理
	void Update();

	/// @brief アクションの場所を変更する
	/// @param _setPlaceTransition どう変更するか指定する
	void OnChangePlace();

	/// @brief カメラ取得
	/// @return カメラ
	CP_Camera& GetCamera();

	/// @brief ターゲット先のオブジェクトを取得
	/// @return ターゲット先のオブジェクト(nullptr)
	const ITargetAccepter* GetTargetAccepter() const;

	/// @brief コントローラーの左スティックの入力を取得
	/// @return 左スティックの入力
	const DirectX::SimpleMath::Vector2& GetInputLeftStick() const;

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
	/// @param _attackerPos 攻撃委敵の座標
	/// @param _pAcceptDamage ダメージ受けたか？
	void OnDamage(const HashiTaku::AttackInformation& _atkInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos,
		bool* _pAcceptDamage = nullptr);

	/// @brief 地面に付いた瞬間の処理
	void OnGroundEnter() override;

	/// @brief 地面から離れた瞬間の処理
	void OnGroundExit()	override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 接地チェッカーを探して取得
	void FindGroundChecker();

	/// @brief 更新できるか取得
	/// @return 更新できるか？
	bool GetCanUpdate();

	/// @brief アクション更新
	void ActionUpdate();

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

	void ImGuiDebug() override;

	// 接地チェッカー
	void ImGuiGroundChecker();
};

