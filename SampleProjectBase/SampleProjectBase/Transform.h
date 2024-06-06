#pragma once

// 座標・回転・スケールをまとめたパラメータ
class Transform
{
public: 
	DirectX::SimpleMath::Vector3 position;	// 座標
	DirectX::SimpleMath::Vector3 rotation;	// 回転角度
	DirectX::SimpleMath::Vector3 scale;	// スケール
	DirectX::SimpleMath::Quaternion Quaternion;	// クォータニオン

	Transform() : scale(DirectX::SimpleMath::Vector3::One) {};
	~Transform() {};
};

