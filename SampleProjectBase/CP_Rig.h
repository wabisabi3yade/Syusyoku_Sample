#pragma once
#include "Component.h"

namespace HashiTaku
{
	class Bone;
	class BoneList;

	/// @brief 武器につけるコンポーネント
	class CP_Rig : public Component
	{
		std::string grabBoneName;

		/// @brief 武器をもつボーン
		const Bone* pGrabBone;

		/// @brief ロード時の角度
		DirectX::SimpleMath::Quaternion loadMeshRot;

		/// @brief ロード時のスケール
		float loadMeshScale;

		/// @brief ボーン座標を表示する
		bool isDisplayPos;
	public:
		CP_Rig();
		~CP_Rig() {}

		void Init() override;

		void Start() override;

		void LateUpdate() override;

		void Draw() override;

		void ImGuiDebug() override;

		// 武器をもつボーンを取得する
		void SetGrabBone(const Bone* _grabBone);

		// ボーン名セット
		void SetGrabBoneName(const std::string& _grabName);

		/// @brief スケルタルメッシュから情報を取得する
		/// @param _skeletalMesh 反映するスケルタルメッシュ
		void SetSkeletalMeshData(BoneList& _skeletalMesh);

		// ボーン名取得
		std::string GetGrabBoneName() const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief ボーンから座標を更新する
		void UpdateTransform();

		void ImGuiSetBone();

		/// @brief 更新できる状態か確認
		/// @return 更新できるか？
		bool IsCanUpdate();

		void SetBoneFromParent();
	};
}