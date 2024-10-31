#pragma once
#include "Component.h"
#include "AttackInformation.h"
#include "IDamageable.h"
#include "Tag.h"

/// @brief 武器コンポーネント
class CP_Weapon : public Component
{
	/// @brief 攻撃情報
	HashiTaku::AttackInformation atkInfomation;

	/// @brief 攻撃にあたるタグ
	std::list<HashiTaku::Tag::Type> attackableTags;

	/// @brief 攻撃判定コリジョン
	bool isAttackCollision;

#ifdef EDIT
	/// @brief 攻撃フラグ描画
	bool isDebugAttackDisplay{ true };
#endif // EDIT

public:
	CP_Weapon();
	virtual ~CP_Weapon() {}

	void Draw() override;

	// コンポーネント共通
	void OnCollisionEnter(const HashiTaku::CollisionInfo& _otherColInfo);

	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation);

	/// @brief 武器の攻撃判定コリジョン
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);

	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 攻撃処理
	/// @param _damager 攻撃与える対象
	void OnAttack(HashiTaku::IDamageable& _damager);

	/// @brief 攻撃できるか確認
	/// @param _targetObject 対象のオブジェクト
	/// @return 攻撃できるか？
	bool CheckAttackable(GameObject& _targetObject);

	// デバッグ用攻撃フラグが立っているか見るための描画
	void DebugAttackFlag();

	/// @brief 攻撃できるタグを追加
	/// @param _addTag タグ
	void AddAttackableTag(HashiTaku::Tag::Type _addTag);

	void ImGuiSetting() override;
};



