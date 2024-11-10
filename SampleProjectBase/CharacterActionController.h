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

	/// @brief キャラクターコンポーネント
	CP_Character* pCharacter;

#ifdef EDIT
	bool isDebugDisplay;
#endif // EDIT
protected:
	/// @brief アニメーションコンポーネント	
	CP_Animation* pAnimation;
public:
	CharacterActionController(CP_Character& _character, const std::string& _stateMachineName);
	~CharacterActionController() {}

	/// @brief 初期化処理
	/// @param _animationController アニメーションコンポーネント
	virtual void Init(CP_Animation& _animation);

	/// @brief アニメーション変更オブザーバーを取得
	/// @return アニメーション変更オブザーバー
	CharacterChangeAnimObserver& GetChangeAnimObserver();

	/// @brief アニメーションを取得
	/// @return アニメーションコンポーネント
	CP_Animation& GetAnimation();

	/// @brief デバッグ描画を行う
	void DebugDisplay();

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override {};

protected:
	/// @brief キャラクターコンポーネントを取得
	/// @return キャラクターコンポーネント
	CP_Character& GetCharacter();

	virtual void ImGuiSetting();
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