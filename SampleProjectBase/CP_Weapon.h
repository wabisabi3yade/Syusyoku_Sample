#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "Tag.h"
#include "IAttacker.h"
#include "IParryAccepter.h"

namespace HashiTaku
{
	class CP_RigidBody;
	class CP_SoundManager;

	/// @brief 武器コンポーネント
	class CP_Weapon : public Component
	{
		/// @brief 攻撃対象タグの上限
		static constexpr u_int ATTACK_TAG_MAX{ 3 };

		/// @brief 攻撃情報
		AttackInformation* pAtkInfomation;

		/// @brief 攻撃にあたるタグ
		std::array<Tag::Type, ATTACK_TAG_MAX> attackableTags;

		/// @brief 一回の攻撃判定で重複しないように記録する用リスト
		std::vector<const CP_RigidBody*> attackedRbs;

		/// @brief エフェクトマネージャー
		DX11EffekseerManager* pVfxManager;

		/// @brief サウンドマネージャー
		CP_SoundManager* pSoundManager;

		/// @brief カメラのトランスフォーム
		const Transform* pCameraTransform;

		/// @brief 攻撃の方向を測定するオフセット座標
		DXSimp::Vector3 measAtkVecOffsetPos;

		/// @brief 1フレーム前の武器の座標
		DXSimp::Vector3 prevWeaponPos;
		
		/// @brief 攻撃方向のリスト
		std::list<DXSimp::Vector3> attackVectorList;

		/// @brief 武器所有者のポインタ
		IAttacker* pAttacker;

		/// @brief 攻撃タグの数
		u_int attackTagCnt;

		/// @brief 攻撃方向の数
		u_int attackVectorCnt;

		/// @brief ヒットエフェクトを手前に発生するオフセット距離
		float hitVfxOffsetPos;

		/// @brief 攻撃判定コリジョン
		bool isAttackCollision;

#ifdef EDIT
		/// @brief 攻撃フラグ描画
		bool isDebugAttackDisplay{ true };
		/// @brief デバッグ攻撃ベクトル描画
		bool isDebugMeasDisplay{ false };
		/// @brief 攻撃場所
		DXSimp::Vector3 hitPos;
		/// @brief 攻撃のベクトル
		DXSimp::Vector3 hitAtkVector;
#endif // EDIT

	public:
		CP_Weapon();
		virtual ~CP_Weapon() {}

		// コンポーネント共通
		void OnCollisionStay(const CollisionInfo& _otherColInfo);

		/// @brief 攻撃情報をセットする
		/// @param _attackInformation 攻撃情報
		void SetAttackInfo(AttackInformation& _attackInformation);

		/// @brief 武器の攻撃判定コリジョン
		/// @param _isAttackCollision 
		void SetIsAttackCollision(bool _isAttackCollision);

		/// @brief 所有者をセットする
		/// @param _weaponOwner 所有者
		void SetWeaponOwner(IAttacker& _weaponOwner);

		/// @brief 攻撃済みコリジョンをリセット
		void ClearAttackedRb();

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;
	private:
		void Start() override;
		void LateUpdate() override;
		void Draw() override;

		/// @brief 攻撃方向を更新する
		void UpdateAttackVector();

		/// @brief 攻撃できるか取得
		/// @param _targetObject 対象のオブジェクト
		/// @return 攻撃できるか？
		bool CanAttack(const CollisionInfo& _otherColInfo) const;

		/// @brief 攻撃処理
		/// @param _damager 攻撃与える対象
		/// @param _contactPos 衝突地点
		void OnAttack(IDamageable& _damager, const DXSimp::Vector3& _contactPos);

		/// @brief 攻撃成功したら起こす処理
		void OnAttackSuccess(const DamageInfo& _damageInfo);

		/// @brief ヒットエフェクトを作成し、再生する
		/// @param _damageInfo ダメージ情報
		void CreateHitVfx(const DamageInfo& _damageInfo);

		/// @brief 既に攻撃したかリストに追加
		/// @param _rb 攻撃済みのRb
		void AddAttackedRb(const CP_RigidBody& _rb);

		/// @brief 攻撃できるか確認
		/// @param _targetObject 対象のオブジェクト
		/// @return 攻撃できるか？
		bool CheckAttackableTag(GameObject& _targetObject) const;

		/// @brief 攻撃できるか確認
		/// @param  _targetRb 対象のオブジェクト
		/// @return 攻撃できるか？
		bool CheckAttackedRb(const CP_RigidBody& _targetRb) const;

		// デバッグ用攻撃フラグが立っているか見るための描画
		void DebugAttackFlag();

		/// @brief 攻撃できるタグを追加
		/// @param _addTag タグ
		void AddAttackableTag(Tag::Type _addTag);

		/// @brief 最後尾の攻撃できるタグを削除
		/// @param _addTag タグ
		void RemoveBackAttackableTag();

		/// @brief 攻撃ベクトルを測定するワールド座標を取得
		/// @return 攻撃ベクトルを測定するワールド座標
		DXSimp::Vector3 GetMeasAtkVecWorldPos() const;

		/// @brief 斬った方向から角度を求める
		/// @param _slashWorldVec ワールド軸の斬った方向
		/// @return 斬った角度
		DXSimp::Vector3 CalcSlashWorldAngles(const DXSimp::Vector3& _slashWorldVec) const;

		void ImGuiDebug() override;
	};
}