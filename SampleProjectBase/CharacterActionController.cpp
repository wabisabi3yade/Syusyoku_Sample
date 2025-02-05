#include "pch.h"
#include "CharacterActionController.h"
#include "CharacterActState_Base.h"
#include "CP_Animation.h"

namespace HashiTaku
{
	CharacterActionController::CharacterActionController(CP_Character& _character, const std::string& _stateMachineName)
		: StateMachine_Base(_stateMachineName), pCharacter(&_character), pAnimation(nullptr)
	{
		pChangeAnimObserver = std::make_unique<CharacterChangeAnimObserver>(*this, _stateMachineName);
	}

	void CharacterActionController::Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody)
	{
		if (_pAnimation)
		{
			// アニメーションコントローラーを各ステートに渡す
			pAnimation = _pAnimation;
			// アニメーション変更オブザーバーを追加
			pAnimation->AddChangeAnimObserver(*pChangeAnimObserver);
		}

		pRigidBody = _pRigidBody;

		// ステートマシン共通開始処理
		StateMachine_Base::Init();
	}

	void CharacterActionController::AddForce(const DXSimp::Vector3& _power)
	{
#ifdef EDIT
		if (!pRigidBody)
		{
			HASHI_DEBUG_LOG("RigidBodyがありません");
			return;
		}
#endif // EDIT

		pRigidBody->AddForce(_power);
	}

	void CharacterActionController::AddImpulse(const DXSimp::Vector3& _power)
	{
#ifdef EDIT
		if (!pRigidBody)
		{
			HASHI_DEBUG_LOG("RigidBodyがありません");
			return;
		}
#endif // EDIT

		pRigidBody->AddImpulse(_power);
	}


	CharacterChangeAnimObserver& CharacterActionController::GetChangeAnimObserver()
	{
		return *pChangeAnimObserver;
	}

	Transform& CharacterActionController::GetMyTransform()
	{
		return pCharacter->GetTransform();
	}

	void CharacterActionController::DebugDisplay()
	{
#ifdef EDIT
		if (!isDebugDisplay) return;

		// デバッグ描画を行う
		if (pCurrentNode)
			static_cast<CharacterActState_Base&>(*pCurrentNode).DebugDisplay();
#endif // EDIT
	}


	void CharacterActionController::SetAnimationBool(const std::string& _paramName, bool _isBool)
	{
		if (!CheckHaveAnimation()) return;
		pAnimation->SetBool(_paramName, _isBool);
	}

	void CharacterActionController::SetAnimationInt(const std::string& _paramName, int _intVal)
	{
		if (!CheckHaveAnimation()) return;
		pAnimation->SetInt(_paramName, _intVal);
	}

	void CharacterActionController::SetAnimationFloat(const std::string& _paramName, float _floatVal)
	{
		if (!CheckHaveAnimation()) return;
		pAnimation->SetFloat(_paramName, _floatVal);
	}

	void CharacterActionController::SetAnimationTrigger(const std::string& _paramName)
	{
		if (!CheckHaveAnimation()) return;
		pAnimation->SetTrigger(_paramName);
	}

	void CharacterActionController::GetVelocity(DXSimp::Vector3& _outVelocity)
	{
#ifdef EDIT
		if (!pRigidBody)
		{
			HASHI_DEBUG_LOG("RigidBodyがありません");
			return;
		}
#endif // EDIT

		_outVelocity = pRigidBody->GetVelocity();
	}

	DXSimp::Vector3 CharacterActionController::GetVelocity() const
	{
		if (!pRigidBody) return DXSimp::Vector3::Zero;

		return pRigidBody->GetVelocity();
	}

	void CharacterActionController::SetVelocity(const DXSimp::Vector3& _velocity)
	{
#ifdef EDIT
		if (!pRigidBody)
		{
			HASHI_DEBUG_LOG("RigidBodyがありません");
			return;
		}
#endif // EDIT

		pRigidBody->SetVelocity(_velocity);
	}

	bool CharacterActionController::GetAnimationBool(const std::string& _paramName)
	{
		if (!CheckHaveAnimation()) return false;
		return pAnimation->GetBool(_paramName);
	}

	int CharacterActionController::GetAnimationInt(const std::string& _paramName)
	{
		if (!CheckHaveAnimation()) return 0;
		return pAnimation->GetInt(_paramName);
	}

	float CharacterActionController::GetAnimationFloat(const std::string& _paramName)
	{
		if (!CheckHaveAnimation()) return 0.0f;
		return pAnimation->GetFloat(_paramName);
	}

	float CharacterActionController::DeltaTime() const
	{
		return pCharacter->DeltaTime();
	}

	json CharacterActionController::Save()
	{
		json data;

		for (auto& node : stateNodeList)
		{
			json actData;
			// 文字列に変換
			actData["typeString"] = GetStateStr(node.first);
			actData["data"] = static_cast<CharacterActState_Base&>(*node.second).Save();
			data["actData"].push_back(actData);
		}

		return data;
	}

	void CharacterActionController::Load(const json& _data)
	{
		json actDataList;
		// ステートごとのパラメータをロードする
		if (LoadJsonDataArray("actData", actDataList, _data))
		{
			for (auto& actData : actDataList)
			{
				std::string stateString;
				if (!LoadJsonString("typeString", stateString, actData))
					continue;

				// 文字列→状態ID
				int stateId = GetStateId(stateString);

				json actParam;
				if (!LoadJsonData("data", actParam, actData))
					continue;

				// ロード
				if (stateNodeList.contains(stateId))
					static_cast<CharacterActState_Base&>(*stateNodeList[stateId]).Load(actParam);
			}
		}
	}

	bool CharacterActionController::CheckHaveAnimation()
	{
		if (!pAnimation)
		{
			HASHI_DEBUG_LOG("アニメーションが設定されていません");
			return false;
		}

		return true;
	}

	CP_Character& CharacterActionController::GetCharacter()
	{
		return *pCharacter;
	}

	void CharacterActionController::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::Checkbox("DebugDisplay", &isDebugDisplay);

		int prevState;
		DebugGetPrevState(prevState);
		ImGui::Text(GetStateStr(prevState).c_str());
		ImGui::Text(GetStateStr(currentStateKey).c_str());

		// 各アクションの調整
		for (auto& pAct : stateNodeList)
		{
			std::string stateStr = GetStateStr(pAct.first);
			if (ImGuiMethod::TreeNode(stateStr))
			{
				static_cast<CharacterActState_Base&>(*pAct.second).ImGuiCall();
				ImGui::TreePop();
			}
		}
#endif // EDIT
	}

	CP_Animation* CharacterActionController::GetAnimation()
	{
		return pAnimation;
	}

	CharacterChangeAnimObserver::CharacterChangeAnimObserver(CharacterActionController& _playerActCon,
		const std::string& _observerName)
		: ChangeAnimObserver(_observerName), pActionController(&_playerActCon)
	{
	}

	void CharacterChangeAnimObserver::ObserverUpdate(const ChangeAnimationInfo& _value)
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
	}
}