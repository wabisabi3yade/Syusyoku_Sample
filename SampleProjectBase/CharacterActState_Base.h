#pragma once
#include "StateMachine.h"

#ifdef EDIT
#include "Geometory.h"
#endif // EDIT

namespace HashiTaku
{
	/// @brief キャラクタのアクションステート基底
	class CharacterActState_Base :
		public StateNode_Base,
		public IImGuiUser,
		public ISaveLoad
	{
	public:
		CharacterActState_Base() {}
		virtual ~CharacterActState_Base() {}

		/// @brief アニメーション終了時の行動
		/// @param _fromAnimNodeName 遷移元のアニメーションノード名
		/// @param _toAnimNodeName 遷移先のアニメーションノード名
		virtual void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
		{}

		/// @brief デバッグ時で描画したいときの処理
		virtual void DebugDisplay() {}

		virtual nlohmann::json Save() = 0;
		virtual void Load(const nlohmann::json& _data) = 0;
	protected:
		virtual void ImGuiDebug() = 0;
	};
}