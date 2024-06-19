#pragma once

// 座標・回転・スケールをまとめたパラメータ
class Transform
{
	DirectX::SimpleMath::Vector3 forward;

public:
	DirectX::SimpleMath::Vector3 position;	// 座標
	DirectX::SimpleMath::Vector3 rotation;	// 回転角度
	DirectX::SimpleMath::Vector3 scale;	// スケール
	DirectX::SimpleMath::Quaternion quaternion;	// クォータニオン


	Transform() : scale(DirectX::SimpleMath::Vector3::One) {};
	~Transform() {};

	void UpdateVector();	// 方向ベクトルを更新する

	void ResetParameter();	// パラメータを初期化

	DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// 正面ベクトルを求める

	void LookAt(DirectX::SimpleMath::Vector3 _worldPos);
};

