#pragma once
#include "StateMachine.h"
#include "CP_Camera.h"

class CameraMoveController;

/// @brief カメラの行動基底クラス
class CameraMoveState_Base : public HashiTaku::StateNode_Base,
	public HashiTaku::ISaveLoad, public HashiTaku::IImGuiUser
{
public:
	enum  class CameraState
	{
		Move,	// 移動時
		Target,	// ターゲット時
		None
	};

private:
	/// @brief カメラの行動コントローラ
	CameraMoveController* pCamController;

public:
	CameraMoveState_Base();
	virtual ~CameraMoveState_Base() {}

	/// @brief 初期化処理
	/// @param _cameraController カメラコントローラ 
	void Init(CameraMoveController& _cameraController);

	/// @brief 状態切り替え開始処理
	void OnStart() override;

	/// @brief 更新処理
	void Update() override;

	/// @brief 状態切り替え終了処理
	void OnEnd() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override {};
protected:
	/// @brief 開始処理の振る舞い
	virtual void OnStartBehavior() {}

	/// @brief 更新処理の振る舞い
	virtual void UpdateBehavior() {}

	/// @brief 終了処理の振る舞い
	virtual void OnEndBehavior() {}

	/// @brief カメラコンポーネントを取得
	/// @return カメラ
	CP_Camera& GetCamera();

	/// @brief ターゲットとする座標を取得する
	/// @return ターゲットのワールド座座標
	const DirectX::SimpleMath::Vector3& GetTargetPosition() const;

	/// @brief オブジェクトの経過時間を取得
	/// @return 経過時間
	float DeltaTime() const;

	void ImGuiDebug() override {}
};

