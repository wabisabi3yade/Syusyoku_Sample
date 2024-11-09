#include "pch.h"
#include "CharacterActionController.h"
#include "CharacterActState_Base.h"
#include "CP_Animation.h"

CharacterActionController::CharacterActionController(CP_Character& _character, const std::string& _stateMachineName)
	: StateMachine_Base(_stateMachineName), pCharacter(&_character), pAnimation(nullptr)
{
	pChangeAnimObserver = std::make_unique<CharacterChangeAnimObserver>(*this, _stateMachineName);
}

void CharacterActionController::Init(CP_Animation& _animationController)
{
	// ステートマシン共通開始処理
	StateMachine_Base::Begin();

	// アニメーションコントローラーを各ステートに渡す
	pAnimation = &_animationController;
	
	// アニメーション変更オブザーバーを追加
	pAnimation->AddChangeAnimObserver(*pChangeAnimObserver);
}

CharacterChangeAnimObserver& CharacterActionController::GetChangeAnimObserver()
{
	return *pChangeAnimObserver;
}

#ifdef EDIT
void CharacterActionController::DebugDisplay()
{
	if (!isDebugDisplay) return;

	// デバッグ描画を行う
	if (pCurrentNode)
		static_cast<CharacterActState_Base&>(*pCurrentNode).DebugDisplay();
}
#else
void CharacterActionController::DebugDisplay()
{
}
#endif // EDIT

nlohmann::json CharacterActionController::Save()
{
	return nlohmann::json();
}

CP_Character& CharacterActionController::GetCharacter()
{
	return *pCharacter;
}

void CharacterActionController::ImGuiSetting()
{
	ImGui::Checkbox("DebugDisplay", &isDebugDisplay);
}


CP_Animation& CharacterActionController::GetAnimation()
{
	return *pAnimation;
}

CharacterChangeAnimObserver::CharacterChangeAnimObserver(CharacterActionController& _playerActCon,
	const std::string& _observerName)
	: ChangeAnimObserver(_observerName), pActionController(&_playerActCon)
{
}

void CharacterChangeAnimObserver::ObserverUpdate(const HashiTaku::ChangeAnimationInfo& _value)
{
	// 現在再生しているアクションにのみ通知する
	auto curAction = static_cast<CharacterActState_Base*>(pActionController->GetCurrentNode());
	if (!curAction)
	{
		HASHI_DEBUG_LOG(GetObserverName() +
			"再生されているアクションがないためアニメーション変更通知がおこなえません");

		return;
	}

	// 通知する
	curAction->OnAnimationEnd(*_value.pFromAnimNodeName, *_value.pToAnimNodeName);

	HASHI_DEBUG_LOG("from:" + *_value.pFromAnimNodeName + "\nto:" + *_value.pToAnimNodeName);
}