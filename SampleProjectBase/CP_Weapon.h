#pragma once
#include "Component.h"

class Bone;

/// @brief 武器につけるコンポーネント
class CP_Weapon : public Component
{
	/// @brief 武器をもつボーン名
	std::string grabBoneName;

	/// @brief 武器をもつボーン
	const Bone* pGrabBone;

public:
	CP_Weapon();
	~CP_Weapon() {}

	void LateUpdate() override;

	void Draw() override;

	// 武器をもつボーンを取得する
	void SetGrabBone(const Bone& _grabBone);

	// ボーン名セット
	void SetGrabBoneName(const std::string& _grabName);

	// ボーン名取得
	std::string GetGrabBoneName() const;

private:
	void UpdateTransform();

	
};

