#pragma once

namespace HashiTaku
{
	class CP_RigidBody;

	/// @brief 衝突するときにどの関数を選択するか
	class CollisionTypeJudge
	{
	public:
		/// @brief 衝突の種類
		enum class ColType
		{
			Already,	// 既に今フレームでは行った
			Enter,	// 衝突開始
			Stay,	// 衝突中
			Exit,	// 衝突終了
		};

	private:
		CP_RigidBody* pMyRb;

		/// @brief 前回フレームの衝突したゲームオブジェクトリスト
		std::unordered_set<CP_RigidBody*> prevColRigidBodyCp;

		/// @brief 今回フレームの衝突したゲームオブジェクトリスト
		std::unordered_set<CP_RigidBody*> curColRigidBodyCp;

	public:
		/// @brief コンストラクタ
		/// @param _rb 自身のRb
		CollisionTypeJudge(CP_RigidBody& _rb);
		~CollisionTypeJudge();

		/// @brief 衝突処理を行う種類を判断する
		/// @param _colRbCp 衝突相手のRbコンポーネント
		/// @return 衝突の種類
		ColType JudgeColKind(CP_RigidBody& _colRbCp);

		/// @brief 衝突終了したゲームオブジェクトを取得する
		/// @param _outGameObjects 結果配列
		void GetExitObjectList(std::vector<const CP_RigidBody*>& _outRbComponents);

		/// @brief 相手方から追加する
		/// @param _colRbCp 相手方Rb
		void AddCurrentColRb(CP_RigidBody& _colRbCp);

		/// @brief 次フレームの為の準備をする
		void SetupForNextFrame();
	};
}