#pragma once
#include "CloneComponent.h"

class Bone;
class SkeletalMesh;

/// @brief 武器につけるコンポーネント
class CP_Weapon : public Component , public CloneComponent<CP_Weapon>
{
	/// @brief 武器をもつボーン名
	std::string grabBoneName;

	/// @brief 武器をもつボーン
	const Bone* pGrabBone;

	/// @brief 持つボーンの座標
	DirectX::SimpleMath::Vector3 offsetPosition;

	/// @brief 持つボーンの回転
	DirectX::SimpleMath::Vector3 offsetAngles;

public:
	CP_Weapon();
	~CP_Weapon() {}

	void LateUpdate() override;

	void Draw() override;

	void ImGuiSetting() override;

	/// @brief スケルタルメッシュから必要な情報を取得する
	/// @param _sk スケルタルメッシュ
	void SetSkeletalMesh(const SkeletalMesh& _sk);

	// 武器をもつボーンを取得する
	void SetGrabBone(const Bone& _grabBone);

	// ボーン名セット
	void SetGrabBoneName(const std::string& _grabName);

	// ボーン名取得
	std::string GetGrabBoneName() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void UpdateTransform();
};

