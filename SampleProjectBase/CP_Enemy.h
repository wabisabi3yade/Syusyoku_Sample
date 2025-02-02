#pragma once
#include "CP_Character.h"
#include "ITargetAccepter.h"
#include "IParryAccepter.h"
#include "PerlinShake.h"

namespace HashiTaku
{
	class CP_MeshRenderer;

	class CP_Enemy : public CP_Character, public ITargetAccepter
	{
		/// @brief 攻撃レベルごとのダメージ受けたときに揺らす処理
		std::array<PerlinShakeParameter,
			static_cast<u_int>(AttackInformation::AttackLevel::MaxNum)> acceptDamageShakeParams;

		/// @brief 予約しているシェイクパラメーター
		PerlinShakeParameter* pReserveShakeParam;

		/// @brief ダメージ受けたときに揺らす処理
		PerlinShake acceptDamageShake;

		/// @brief メッシュ描画
		CP_MeshRenderer* pMeshRenderer;

		/// @brief 自身をターゲットにしている者
		std::list<IObjectTargeter*> pTargeters;

		/// @brief 敵の名称
		std::string enemyName;

	public:
		CP_Enemy();
		virtual ~CP_Enemy() {}

		/// @brief 敵名を取得
		/// @return 敵名
		const std::string& GetEnemyName() const;

		/// @brief そのオブジェクトのワールド座標を取得
		/// @return ワールド座標を取得
		const DXSimp::Vector3& GetWorldPos() const override;

		/// @brief ターゲットする側を取得する
		/// @param _targeter ターゲットする側
		void SetTargeter(IObjectTargeter& _targeter);

		/// @brief ターゲットする側を削除する
		/// @param _targeter 削除ターゲットする側
		void RemoveTargeter(IObjectTargeter& _targeter) override;

		/// @brief ヒットストップ開始した時の処理
		void OnHitStopBegin() override;

		/// @brief ヒットストップ終了した時の処理
		void OnHitStopEnd() override;

		/// @brief ターゲット側に死んだことを通知
		void OnRemoveNotifyToTargeter();

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;
	protected:
		void Start() override;
		void Update() override;
		void OnDestroy() override;

		/// @brief ヒットストップ中に揺れるか？
		void HitStopShaking();

		/// @brief 敵名をセット
		/// @param _enemyName 敵名
		void SetEnemyName(const std::string& _enemyName);

		/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		/// @param _attackerPos 攻撃した側の座標
		/// @return ダメージを受けたか？
		bool OnDamageBehavior(AttackInformation& _attackInfo) override;

		/// @brief ダメージ受けたときの処理
		/// @param _attackInfo 攻撃情報
		/// @param _contactPos 衝突地点
		void OnTakeDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos) override;

		/// @brief 死んだときの処理
		void OnDeathBehavior() override {};

		void ImGuiDebug() override;
	private:
		// ヒット時の揺れシェイク編集
		void ImGuiHitShake();
	};
}