#pragma once
#include "PlayerActionController_Base.h"
#include "ITargetAccepter.h"

/// @brief プレイヤーアクションクラス
class PlayerAction : 
	public IObjectTargeter,
	public HashiTaku::IImGuiUser,
	public HashiTaku::ISaveLoad
{
public:
	/// @brief プレイヤーから見た入力の方向
	enum class InputVector
	{
		Forward,	// 前方向
		Back	// 後ろ
	};

private:
	/// @brief 地上行動のアクションコントローラー
	std::unique_ptr<PlayerActionController>
		pGroundContoroller;

	/// @brief 空中行動のアクションコントローラー
	std::unique_ptr<PlayerActionController>
		pAirContoroller;

	/// @brief 入力クラス
	GameInput* pInput;

	/// @brief バトルマネジャー
	CP_BattleManager* pBattleManager;

	/// @brief ターゲットしているオブジェクト先
	ITargetAccepter* pTargetObject;

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

	/// @brief ターゲット中かどうか
	bool isTargeting;

	/// @brief 1フレーム前のターゲット状態
	bool prevIsTargeting;
public:
	PlayerAction();
	~PlayerAction();

	void Init(CP_Animation* _pAnimation,
		CP_RigidBody* _pRigidBody);

	/// @brief 更新処理
	void Update();

	/// @brief カメラ取得
	/// @return カメラ
	CP_Camera& GetCamera();

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
	bool GetIsCanCancel() const;

	/// @brief コンビネーション攻撃できるかを取得
	/// @return コンビネーション攻撃できるか？
	bool GetCanAttack() const;

	/// @brief 移動できるかを取得
	/// @return 移動できるか？
	bool GetCanMove() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief  ターゲット開始時のお処理
	void OnBeginTargeting();

	/// @brief ターゲット終了時の処理
	void OnEndTargeting();

	/// @brief ターゲットの更新処理
	void UpdateTargeting();

	void ImGuiDebug();
};

