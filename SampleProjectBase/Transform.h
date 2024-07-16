#pragma once

// 座標・回転・スケールをまとめたパラメータ
class Transform
{
	/// @brief 親トランスフォーム
	Transform* pParent;

	// 各方向の単位ベクトル
	DirectX::SimpleMath::Vector3 up;	// 上
	DirectX::SimpleMath::Vector3 right;	// 右
	DirectX::SimpleMath::Vector3 forward;	// 前
public:
	DirectX::SimpleMath::Vector3 position;	// 座標
	DirectX::SimpleMath::Vector3 rotation;	// 回転角度
	DirectX::SimpleMath::Vector3 scale;	// スケール
	DirectX::SimpleMath::Quaternion quaternion;	// クォータニオン

	Transform() : scale(DirectX::SimpleMath::Vector3::One), pParent(nullptr) {};
	~Transform() {};

	void UpdateVector();	// 方向ベクトルを更新する

	void ResetParameter();	// パラメータを初期化
	
	// その座標にオブジェクトを向ける
	void LookAt(DirectX::SimpleMath::Vector3 _worldPos, DirectX::SimpleMath::Vector3 _upVector
	= DirectX::SimpleMath::Vector3::Up);	

	/// @brief 親トランスフォームを設定する
	/// @param _parent 親トランスフォームの参照
	void SetParent(Transform& _parent);

	const DirectX::SimpleMath::Vector3 Right()const { return right; }	// 右ベクトル
	const DirectX::SimpleMath::Vector3 Up()const { return up; }	// 上ベクトル
	const DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// 正面ベクトル

	/// @brief 正面ベクトルをずらす(LookAt時に使用)
	void ShiftForward() { forward.x += 0.001f; }
};

