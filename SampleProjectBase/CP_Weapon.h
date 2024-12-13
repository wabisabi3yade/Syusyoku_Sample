#pragma once
#include "Component.h"
#include "AttackInformation.h"
#include "IDamageable.h"
#include "Tag.h"

class CP_RigidBody;

/// @brief 武器コンポーネント
class CP_Weapon : public Component
{
	/// @brief 攻撃対象タグの上限
	static constexpr u_int ATTACK_TAG_MAX{ 3 };

	/// @brief 攻撃情報
	const HashiTaku::AttackInformation* pAtkInfomation;

	/// @brief 攻撃にあたるタグ
	std::array<HashiTaku::Tag::Type, ATTACK_TAG_MAX> attackableTags;

	/// @brief 一回の攻撃判定で重複しないように記録する用リスト
	std::vector<const CP_RigidBody*> attackedRbs;

	/// @brief この武器を持つオブジェクトの座標
	const DirectX::SimpleMath::Vector3* pHaveObjectPos;

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

	void Draw() override;

	// コンポーネント共通
	void OnCollisionStay(const HashiTaku::CollisionInfo& _otherColInfo);

	/// @brief 攻撃情報をセットする
	/// @param _attackInformation 攻撃情報
	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInformation);

	/// @brief 武器の攻撃判定コリジョン
	/// @param _isAttackCollision 
	void SetIsAttackCollision(bool _isAttackCollision);

	/// @brief 所有オブジェクトの座標のポインタ
	/// @param _pWorldPos ワールド座標
	void SetHaveObjPosPointer(const DirectX::SimpleMath::Vector3* _pWorldPos);

	/// @brief 攻撃済みコリジョンをリセット
	void ClearAttackedRb();

	/// @brief セーブする
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 攻撃できるか取得
	/// @return 攻撃できるか？
	bool CanAttack() const;

	/// @brief 攻撃処理
	/// @param _damager 攻撃与える対象
	/// @param _haveObjPos 所有オブジェクトの座標
	void OnAttack(HashiTaku::IDamageable& _damager, 
		const DirectX::SimpleMath::Vector3& _haveObjPos);

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

	/// @brief ヒットエフェクトを出す
	/// @param 当たり判定の衝突地点
	void CreateHitVfx(const DirectX::SimpleMath::Vector3& _contactPos);

	// デバッグ用攻撃フラグが立っているか見るための描画
	void DebugAttackFlag();

	/// @brief 攻撃できるタグを追加
	/// @param _addTag タグ
	void AddAttackableTag(HashiTaku::Tag::Type _addTag);

	/// @brief 最後尾の攻撃できるタグを削除
	/// @param _addTag タグ
	void RemoveBackAttackableTag();

	void ImGuiDebug() override;
};



