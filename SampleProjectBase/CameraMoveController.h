#pragma once
#include "CameraMoveState_Base.h"

/// @brief カメラの動き
class CameraMoveController : 
	public HashiTaku::StateMachine_Base<CameraMoveState_Base::CameraState>,
	public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief カメラコンポーネント
	CP_Camera* pCamera;

	/// @brief カメラの対象オブジェクト
	const Transform* pTargetTransform;
public:
	CameraMoveController();
	~CameraMoveController() {}

	/// @brief 開始処理
	/// @param _camera カメラコンポーネント
	void Begin(CP_Camera& _camera);

	/// @brief ステートを変更
	/// @param _cameraState 変更先のステート
	/// @param _isForce 強制的に変更するか
	void ChangeState(CameraMoveState_Base::CameraState _cameraState, bool _isForce = false);

	/// @brief ターゲットとするオブジェクトをセット（外したい場合nullptr）
	/// @param _pTransform ゲームオブジェクト 
	void SetTargetTransform(const Transform* _pTransform);

	/// @brief カメラを取得
	/// @return カメラコンポーネント
	CP_Camera& GetCamera();

	/// @brief 対象オブジェクトを取得
	/// @return 対象オブジェクト
	const Transform* GetTargetTransform() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 更新処理
	void Update() override;

	/// @brief カメラのステートを作成
	/// @tparam T 対応したノード
	/// @param _stateType ステートの種類
	template<class T> void CreateState(CameraMoveState_Base::CameraState _stateType);

	/// @brief カメラの列挙型を文字列に
	/// @param _state 変更したい列挙型
	/// @return 文字列
	std::string GetStateName(CameraMoveState_Base::CameraState _state);

	/// @brief カメラ行動にキャストする
	/// @param _stateBase ステートノードの基底
	/// @return カメラステートクラス
	CameraMoveState_Base& CastCamState(HashiTaku::StateNode_Base& _stateBase);

	void ImGuiDebug() override;

	// 全ステートをロードする
	void LoadStates(const nlohmann::json& _data);
};

template<class T>
inline void CameraMoveController::CreateState(CameraMoveState_Base::CameraState _stateType)
{
	// ステートを作成
	std::unique_ptr<CameraMoveState_Base> pCameraMove =
		std::make_unique<T>();

	pCameraMove->Init(*this);

	// 追加
	AddNode(_stateType, std::move(pCameraMove));
}
