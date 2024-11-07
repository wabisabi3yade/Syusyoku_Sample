#pragma once
#include "StateMachine.h"
#include "ChangeAnimObserver.h"

class CharacterChangeAnimObserver;
class CP_Animation;
class CP_Character;

/// @brief キャラクター（プレイヤー、敵など)のアクションコントローラー
class CharacterActionController : public HashiTaku::StateMachine_Base<int>, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief アニメーション変更したときのオブザーバー
	std::unique_ptr<CharacterChangeAnimObserver> pChangeAnimObserver;

protected:
	/// @brief アニメーションコンポーネント	
	CP_Animation* pAnimation;

	/// @brief キャラクターコンポーネント
	CP_Character* pCharacter;
public:
	CharacterActionController(CP_Character& _character, const std::string& _stateMachineName);
	~CharacterActionController() {}

	/// @brief 初期化処理
	/// @param _animationController アニメーションコンポーネント
	virtual void Init(CP_Animation& _animation);

	/// @brief アニメーション変更オブザーバーを取得
	/// @return アニメーション変更オブザーバー
	CharacterChangeAnimObserver& GetChangeAnimObserver();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override {};

protected:
	virtual void ImGuiSetting() = 0;
};

/// @brief キャラクターがアニメーション遷移したときのオブザーバー
class CharacterChangeAnimObserver : public HashiTaku::ChangeAnimObserver
{
	/// @brief アクションコントローラー
	CharacterActionController* pActionController;

public:
	/// @brief コンストラクタ
	/// @param _playerActCon アクションコントローラー
	/// @param _observerName オブザーバー名
	CharacterChangeAnimObserver(CharacterActionController& _playerActCon,
		const std::string& _observerName);
	~CharacterChangeAnimObserver() {}

	/// @brief 通知がきたときの処理
	void ObserverUpdate(const HashiTaku::ChangeAnimationInfo& _value);
};