#pragma once
#include "ISaveLoad.h"
#include "CollisionInfo.h"

namespace HashiTaku
{
	class GameObject;
	// ゲームのサブシステムとなるコンポーネントの基底クラス
	class Component : public ISaveLoad, public IImGuiUser
	{
		friend class GameObject;

		/// @brief 名前
		std::string name;

		/// @brief 優先度
		int priority;

		/// @brief Awake処理を行ったかどうか？
		bool isAlreadyAwake;

		/// @brief Start処理を行ったかどうか？
		bool isAlreadyStart;
	protected:
		/// @brief 活動状態
		bool isEnable;

		/// @brief このコンポーネントの所持オブジェクト
		GameObject* gameObject;
	public:
		Component();
		virtual ~Component() {};

		virtual void Init() {};

		/// @brief 外部から呼び出すAwake処理呼び出し
		void AwakeCall();

		/// @brief 外部から呼び出すStart処理呼び出し
		void StartCall();

		/// @brief 外部から呼び出すUpdate処理呼び出し
		void UpdateCall();

		/// @brief 外部から呼び出すLateUpdate処理呼び出し
		void LateUpdateCall();

		/// @brief 外部から呼び出すDraw処理呼び出し
		void DrawCall();

		/// @brief 削除されたときに処理する
		virtual void OnDestroy() {}
		/// @brief トランスフォームの移動座標が変更した時の処理
		virtual void OnChangePosition() {}
		/// @brief トランスフォームのスケール値が変更した時の処理
		virtual void OnChangeScale() {}
		/// @brief トランスフォームの回転角度が変更した時の処理
		virtual void OnChangeRotation() {}

		/// @brief 衝突開始時の処理呼び出し
		virtual void OnCollisionEnter(const CollisionInfo& _otherColInfo) {}
		/// @brief 衝突中の処理呼び出し
		virtual void OnCollisionStay(const CollisionInfo& _otherColInfo) {}
		/// @brief 衝突終了時の処理呼び出し
		virtual void OnCollisionExit(const CollisionInfo& _otherColInfo) {}

		/// @brief 名前をセットする
		/// @param _name 名前
		void SetName(const std::string& _name);

		/// @brief 優先度をセット
		/// @param _priority 優先度
		void SetPriority(int _priority);

		// 活動状態をセット
		void SetEnable(bool _enable);

		/// @brief オブジェクトの経過時間を取得
		/// @return 経過時間
		float DeltaTime() const;

		/// @brief 経過時間を進める速度取得
		/// @return 進める速度
		float GetDeltaSpeed() const;

		// 名前を取得
		std::string GetName() const;

		/// @brief 優先度を取得
		/// @return 優先度
		int GetPriority() const;

		// オブジェクトのトランスフォーム
		Transform& GetTransform() const;

		// ゲームオブジェクト取得	
		GameObject& GetGameObject() const { return *gameObject; }

		// 活動フラグ取得
		bool GetIsEnable() const;

		// Awake処理行ったか取得
		bool GetIsAlreadyAwake() const;

		// Start処理行ったか取得
		bool GetIsAlreadyStart() const;

		/// @brief ImGuiでパラメータを変える処理
		void ImGuiSettingCall();

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;

	private:
		/// @brief 活動状態をtrueに変更したときの処理
		void OnEnableTrueCall();

		/// @brief 活動状態をfalseに変更したときの処理
		void OnEnableFalseCall();
	protected:
		/// @brief 作成時に呼ばれる処理
		virtual void Awake() {}

		/// @brief 1フレーム目で行う処理
		virtual void Start() {};

		/// @brief 毎フレーム更新処理
		virtual void Update() {};

		/// @brief Updateの後に行う更新処理
		virtual void LateUpdate() {};

		/// @brief 描画処理
		virtual void Draw() {};

		/// @brief Drawの後に行う描画処理
		virtual void LateDraw() {};

		// 活動状態を切り替える時の処理
		virtual void OnEnableTrue() {}
		virtual void OnEnableFalse() {}

		/// @brief 他のコンポーネントが追加されたときの処理
		/// @param _addComp 追加したコンポーネント
		virtual void OnAddComponent(Component& _addComp) {}

		/// @brief ゲームオブジェクトとコンポーネントの活動状態を取得
		/// @return どちらも活動しているか？
		bool GetIsActive();

		void ImGuiDebug() override {}
	};
}