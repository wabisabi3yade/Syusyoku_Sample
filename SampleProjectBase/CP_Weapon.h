#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "Tag.h"
#include "IAttacker.h"
#include "IParryAccepter.h"

namespace HashiTaku
{
	class CP_RigidBody;

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

		/// @brief 武器所有者のポインタ
		IAttacker* pAttacker;

		/// @brief 攻撃タグの数
		u_int attackTagCnt;

		/// @brief 攻撃判定コリジョン
		bool isAttackCollision;

#ifdef EDIT
		/// @brief 攻撃フラグ描画
		bool isDebugAttackDisplay{ true };
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
		void Draw() override;

		/// @brief 攻撃できるか取得
		/// @return 攻撃できるか？
		bool CanAttack() const;

		/// @brief 攻撃処理
		/// @param _damager 攻撃与える対象
		/// @param _contactPos 衝突地点
		void OnAttack(IDamageable& _damager, const DXSimp::Vector3& _contactPos);

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

		void ImGuiDebug() override;
	};
}