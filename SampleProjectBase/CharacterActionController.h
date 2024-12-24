#pragma once
#include "StateMachine.h"
#include "ChangeAnimObserver.h"
#include "CP_Character.h"
#include "CP_Animation.h"
#include "CP_RigidBody.h"
//#include "AttackInformation.h"

namespace HashiTaku
{
	class CharacterChangeAnimObserver;

	/// @brief キャラクター（プレイヤー、敵など)のアクションコントローラー
	class CharacterActionController :
		public StateMachine_Base<int>, public IImGuiUser,
		public ISaveLoad
	{
		/// @brief アニメーション変更したときのオブザーバー
		std::unique_ptr<CharacterChangeAnimObserver> pChangeAnimObserver;

	protected:
		/// @brief キャラクターコンポーネント
		CP_Character* pCharacter;

		/// @brief アニメーションコンポーネント	
		CP_Animation* pAnimation;

		/// @brief プレイヤーのリジッドボディ
		CP_RigidBody* pRigidBody;
#ifdef EDIT
		// アクション周りでデバッグ描画するか？
		bool isDebugDisplay{ true };
#endif // EDIT

	public:
		CharacterActionController(CP_Character& _character, const std::string& _stateMachineName);
		~CharacterActionController() {}

		/// @brief 初期化処理
		/// @param _pAnimation アニメーションコンポーネント
		/// @param _pRigidBody リジッドボディ
		virtual void Init(CP_Animation* _pAnimation, CP_RigidBody* _pRigidBody);

		/// @brief ダメージ受けたときのアクション処理
		/// @param _atkInfo 攻撃情報
		virtual void OnDamage(const AttackInformation& _atkInfo,
			const DirectX::SimpleMath::Vector3& _attackerPos,
			bool* _pAcceptDamage = nullptr) {};

		/// @brief アニメーション変更オブザーバーを取得
		/// @return アニメーション変更オブザーバー
		CharacterChangeAnimObserver& GetChangeAnimObserver();

		/// @brief キャラクターコンポーネントを取得
		/// @return キャラクターコンポーネント
		CP_Character& GetCharacter();

		/// @brief 自身のオブジェクトのトランスフォーム
		/// @return オブジェクトのトランスフォーム
		Transform& GetMyTransform();

		/// @brief RigidBodyを取得
		/// @return RigidBody
		CP_RigidBody* GetRB();

		/// @brief アニメーションを取得
		/// @return アニメーションコンポーネント
		CP_Animation* GetAnimation();

		/// @brief デバッグ描画を行う
		virtual void DebugDisplay();

		/// @brief 指定したbool変数に値をセット
		/// @param _paramName パラメーター名
		/// @param _isBool セットする値
		void SetAnimationBool(const std::string& _paramName, bool _isBool);

		/// @brief 指定したint変数に値をセット
		/// @param _paramName パラメーター名
		/// @param _intVall セットする値
		void SetAnimationInt(const std::string& _paramName, int _intVal);

		/// @brief 指定したfloat変数に値をセット
		/// @param _paramName パラメーター名
		/// @param _floatVal セットする値
		void SetAnimationFloat(const std::string& _paramName, float _floatVal);

		/// @brief 指定したトリガー変数をtrueにする
		/// @param _paramName パラメーター名
		void SetAnimationTrigger(const std::string& _paramName);

		/// @brief 指定したbool変数に値を取得
		/// @param _paramName パラメーター名
		/// @param _isBool 取得する値
		bool GetAnimationBool(const std::string& _paramName);

		/// @brief 指定したint変数に値を取得
		/// @param _paramName パラメーター名
		/// @param _intVall 取得する値
		int GetAnimationInt(const std::string& _paramName);

		/// @brief 指定したfloat変数に値を取得
		/// @param _paramName パラメーター名
		/// @param _floatVal 取得する値
		float GetAnimationFloat(const std::string& _paramName);

		json Save() override;
		void Load(const json& _data) override;
	protected:
		/// @brief アニメーションを持っているか確認
		/// @return アニメーション持っているか？
		bool CheckHaveAnimation();

		/// @brief 各Stateの文字列を取得する
		/// @param _stateId 状態のID
		/// @return 文字列
		virtual std::string GetStateStr(int _stateId) = 0;

		/// @brief 各StateのIDを取得する
		/// @param _stateName　状態名
		/// @return 状態のID
		virtual int GetStateId(const std::string& _stateName) = 0;

		virtual void ImGuiDebug();
	};

	/// @brief キャラクターがアニメーション遷移したときのオブザーバー
	class CharacterChangeAnimObserver : public ChangeAnimObserver
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
		void ObserverUpdate(const ChangeAnimationInfo& _value);
	};
}