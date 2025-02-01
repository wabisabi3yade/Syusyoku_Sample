#pragma once
#include "CameraMoveState_Base.h"
#include "CalcPerlinShakeVector.h"
#include "ITargetAccepter.h"

namespace HashiTaku
{
	/// @brief カメラの動き
	class CameraMoveController :
		public StateMachine_Base<CameraMoveState_Base::CameraState>,
		public IImGuiUser,
		public ISaveLoad
	{
		using CameraState = CameraMoveState_Base::CameraState;
	private:
		/// @brief パーリンシェイク
		PerlinShake perlinShake;

		/// @brief カメラの現在の座標(カメラ揺れを実装するのでオブジェクトと別で持っておく)
		DXSimp::Vector3 curBaseCameraPos;

		/// @brief 前フレームのフォローしている座標
		DXSimp::Vector3 prevFollowPos;

		/// @brief カメラコンポーネント
		CP_Camera* pCamera;

		/// @brief カメラの追従オブジェクト
		const Transform* pFollowTransform;

		/// @brief カメラの注視オブジェクト
		const ITargetAccepter* pLookAtTransform;
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

		/// @brief 揺れを開始する
		/// @param _shakeParam カメラ揺れパラメータ
		void BeginShake(const PerlinShakeParameter& _shakeParam);

		/// @brief シェイクを止める
		void StopShake();

		/// @brief ターゲットとするオブジェクトをセット（外したい場合nullptr）
		/// @param _pTransform ゲームオブジェクト 
		void SetFollowTransform(const Transform* _pTransform);

		/// @brief 注視するオブジェクトをセット（外したい場合nullptr）
		/// @param _pTransform ゲームオブジェクト 
		void SetLookAtObject(const ITargetAccepter* _pTransform);

		/// @brief 現在のカメラのベース座標を更新
		/// @param _camPos ベースとなるカメラの座標
		void SetCameraBasePos(const DXSimp::Vector3& _camPos);

		/// @brief 視野角の角度をセット(単位：deg)
		/// @param _setDegree 視野角
		void SetFov(float _setDegree);

		void OnPlayerWin(const Transform& _targetTransform);

		/// @brief 視野角を取得(単位：deg)
		/// @return 視野角
		float GetFov() const;

		/// @brief カメラを取得
		/// @return カメラコンポーネント
		CP_Camera& GetCamera();

		/// @brief カメラ揺れを考慮しないカメラの座標を取得
		/// @return カメラ揺れを考慮しないカメラ座標
		const DXSimp::Vector3& GetCameraBasePos();

		/// @brief 前フレームの追従先座標を取得
		/// @return 前フレームの追従先座標
		const DXSimp::Vector3& GetPrevFollowPos() const;

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
		const DXSimp::Vector3& GetLookAtWorldPos() const;

		json Save() override;
		void Load(const json& _data) override;
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

		/// @brief カメラの列挙型を文字列に
		/// @param _state 変更したい列挙型
		/// @return 文字列
		std::string GetStateName(CameraMoveState_Base::CameraState _state);

		/// @brief カメラ行動にキャストする
		/// @param _stateBase ステートノードの基底
		/// @return カメラステートクラス
		CameraMoveState_Base& CastCamState(StateNode_Base& _stateBase);

		void ImGuiDebug() override;

		// 全ステートをロードする
		void LoadStates(const json& _data);
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
}