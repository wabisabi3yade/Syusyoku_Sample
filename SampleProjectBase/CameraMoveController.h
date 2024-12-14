#pragma once
#include "CameraMoveState_Base.h"
#include "ITargetAccepter.h"

/// @brief カメラの動き
class CameraMoveController :
	public HashiTaku::StateMachine_Base<CameraMoveState_Base::CameraState>,
	public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	using CameraState = CameraMoveState_Base::CameraState;

public:
	/// @brief カメラの揺れの強さ
	enum class ShakeLevel
	{
		Low,
		Mid,
		High,
		SuperHigh,
		MaxNum
	};

	/// @brief カメラの揺れの強さ
	enum class ShakeType
	{
		Vertical,	// 縦揺れ
		Horizon,	// 横揺れ
		Both,	// 両方揺れ
		MaxNum
	};

	/// @brief レベルごとに設定するパラメータ
	struct CameraShakeParameter
	{
		float power{ 0.2f };	// 揺れの強さ
		float duration{ 0.3f };	// 揺れの間隔時間(s)
	};
private:

	/// @brief 各レベルの揺れの強さ
	std::array<CameraShakeParameter, static_cast<u_int>(ShakeLevel::MaxNum)> shakeLevelParameter;

	/// @brief カメラの現在の座標(カメラ揺れを実装するのでオブジェクトと別で持っておく)
	DirectX::SimpleMath::Vector3 curBaseCameraPos;

	/// @brief 現在のカメラ揺れでずらしているオフセット座標
	DirectX::SimpleMath::Vector3 curShakeOffset;

	/// @brief 前フレームのフォローしている座標
	DirectX::SimpleMath::Vector3 prevFollowPos;

	/// @brief カメラコンポーネント
	CP_Camera* pCamera;

	/// @brief カメラの追従オブジェクト
	const Transform* pFollowTransform;

	/// @brief カメラの注視オブジェクト
	const ITargetAccepter* pLookAtTransform;

	/// @brief 揺らしてる強さ
	ShakeLevel curShakeLevel;

	/// @brief 揺らしてる種類
	ShakeType curShakeType;

	/// @brief 揺らす時間
	float shakeTime;

	/// @brief 揺らしてからの経過時間
	float shakeElapsedTime;

	/// @brief 前揺れたときの時間
	float prevShakeElapsedTime;

	/// @brief 揺れているか？
	bool isShaking;

	ShakeLevel testLevel = ShakeLevel::Low;
	ShakeType testType = ShakeType::Both;
	float testTime = 0.0f;
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
	void SetFollowTransform(const Transform* _pTransform);

	/// @brief 注視するオブジェクトをセット（外したい場合nullptr）
	/// @param _pTransform ゲームオブジェクト 
	void SetLookAtObject(const ITargetAccepter* _pTransform);

	/// @brief 現在のカメラのベース座標を更新
	/// @param _camPos ベースとなるカメラの座標
	void SetCameraBasePos(const DirectX::SimpleMath::Vector3& _camPos);

	/// @brief 視野角の角度をセット(単位：deg)
	/// @param _setDegree 視野角
	void SetFov(float _setDegree);

	/// @brief 揺れを開始する
	/// @param _level 揺れの強さ
	/// @param _type 揺れの種類
	/// @param _time 揺れの時間
	void BeginShake(ShakeLevel _level, ShakeType _type, float _time);

	/// @brief プレイヤー勝利時の処理
	/// @param _targetTransform ターゲットのトランスフォーム
	void OnPlayerWin(const Transform& _targetTransform);

	/// @brief 視野角を取得(単位：deg)
	/// @return 視野角
	float GetFov() const;

	/// @brief カメラを取得
	/// @return カメラコンポーネント
	CP_Camera& GetCamera();

	/// @brief カメラ揺れを考慮しないカメラの座標を取得
	/// @return カメラ揺れを考慮しないカメラ座標
	const DirectX::SimpleMath::Vector3& GetCameraBasePos();

	/// @brief 前フレームの追従先座標を取得
	/// @return 前フレームの追従先座標
	const DirectX::SimpleMath::Vector3& GetPrevFollowPos() const;

	/// @brief 追従先のオブジェクトを所持しているか取得
	/// @return 所持しているか？
	bool GetHasFollowObject() const;

	/// @brief 対象オブジェクトを取得
	/// @return 対象オブジェクト
	const Transform* GetFollowTransform() const;

	/// @brief ターゲットがあるか取得
	/// @return ターゲットがあるか？
	bool GetHaveTarget() const;

	/// @brief 注視先オブジェクトを取得
	/// @return 注視先オブジェクトのワールド座標
	const DirectX::SimpleMath::Vector3& GetLookAtWorldPos() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief カメラの座標を初期化
	void InitCameraPos();

	/// @brief 更新処理
	void Update() override;

	/// @brief カメラ揺れの更新
	void ShakeUpdate();

	/// @brief 最終の座標を更新する
	void UpdateFinalPos();

	/// @brief カメラのステートを作成
	/// @tparam T 対応したノード
	/// @param _stateType ステートの種類
	template<class T> void CreateState(CameraMoveState_Base::CameraState _stateType);

	/// @brief 揺れの終了処理
	void OnEndShake();

	/// @brief カメラの列挙型を文字列に
	/// @param _state 変更したい列挙型
	/// @return 文字列
	std::string GetStateName(CameraMoveState_Base::CameraState _state);

	/// @brief 揺れのオフセット値を取得する
	/// @param _level レベル
	/// @param _type 種類
	/// @return オフセット値
	DirectX::SimpleMath::Vector3 CalcShakeOffset(ShakeLevel _level, ShakeType _type);

	/// @brief カメラ行動にキャストする
	/// @param _stateBase ステートノードの基底
	/// @return カメラステートクラス
	CameraMoveState_Base& CastCamState(HashiTaku::StateNode_Base& _stateBase);

	void TestShakeUpdate();

	void ImGuiDebug() override;

	// 揺れのレベル強さ
	void ImGuiShakeLevel();

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
