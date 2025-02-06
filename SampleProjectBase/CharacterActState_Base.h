#pragma once
#include "StateMachine.h"
#include "IActionController.h"

#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace HashiTaku
{
	// アクションコントローラー派生したかのコンセプト
	template<typename T>
	concept ActionControllerConcept = std::is_base_of_v<IActionController, T>;

	/// @brief キャラクタのアクションステート基底
	class CharacterActState_Base :
		public StateNode_Base,
		public IImGuiUser,
		public ISaveLoad
	{
		/// @brief キャラクターのアクションコントローラー
		IActionController* pActionController;
	public:
		CharacterActState_Base();
		virtual ~CharacterActState_Base() {}

		/// @brief 初期化
		/// @param _actionController アクションコントローラーインターフェース
		void Init(IActionController& _actionController);

		/// @brief アニメーション終了時の行動
		/// @param _fromAnimNodeName 遷移元のアニメーションノード名
		/// @param _toAnimNodeName 遷移先のアニメーションノード名
		virtual void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
		{}

		/// @brief デバッグ時で描画したいときの処理
		virtual void DebugDisplay() {}

		virtual json Save() = 0;
		virtual void Load(const json& _data) = 0;
	protected:
		/// @brief 速度をセット
		/// @param _velocity 速度
		void SetVelocity(const DXSimp::Vector3& _velocity);

		/// @brief オブジェクトのΔtを取得
		/// @return Δt
		float DeltaTime() const;

		/// @brief アクションコントローラーのインターフェースを取得
		/// @return アクションコントローラー
		IActionController& GetActionController();

		/// @brief 自身のトランスフォームを取得する
		/// @return 自身のトランスフォーム
		Transform& GetMyTransform();

		/// @brief 指定したアクションコントローラーを取得する
		/// @tparam T アクションコントローラーの種類
		/// @return 取得するアクションコントローラー
		template<ActionControllerConcept T>
		T& GetDeliverActionController();

		virtual void ImGuiDebug() = 0;
	};

	template<ActionControllerConcept T>
	inline T& CharacterActState_Base::GetDeliverActionController()
	{
#ifdef EDIT
		T* pRetCon = dynamic_cast<T*>(pActionController);
		assert(pRetCon && "正常にアクションコントローラーを変換できませんでした");

		return *pRetCon;
#endif // EDIT

		return static_cast<T&>(*pActionController);
	}
}