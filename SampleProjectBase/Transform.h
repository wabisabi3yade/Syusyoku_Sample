#pragma once

// 座標・回転・スケールをまとめたパラメータ
class Transform
{
	/// @brief 親トランスフォーム
	Transform* pParent;

	/// @brief 子トランスフォーム
	std::list<Transform> pChilds;

	// 各方向の単位ベクトル
	DirectX::SimpleMath::Vector3 up;	// 上
	DirectX::SimpleMath::Vector3 right;	// 右
	DirectX::SimpleMath::Vector3 forward;	// 前

	DirectX::SimpleMath::Vector3 eularAngles;	// 回転角度
	DirectX::SimpleMath::Quaternion rotation;	// クォータニオン
public:
	DirectX::SimpleMath::Vector3 position;	// 座標
	DirectX::SimpleMath::Vector3 scale;	// スケール
	

	Transform();
	~Transform() {};

	// 方向ベクトルを更新する
	void UpdateVector();	

	// パラメータを初期化
	void ResetParameter();	
	
	// その座標にオブジェクトを向ける
	void LookAt(DirectX::SimpleMath::Vector3 _worldPos, DirectX::SimpleMath::Vector3 _upVector
	= DirectX::SimpleMath::Vector3::Up);	

	/// @brief 親トランスフォームを設定する
	/// @param _parent 親トランスフォームの参照
	void SetParent(Transform& _parent);

	// オイラー角をセット
	void SetEularAngleX(float _angle_x);	// x軸
	void SetEularAngleY(float _angle_y);	// y軸
	void SetEularAngleZ(float _angle_z);	// z軸
	void SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);

	// クォータニオンをセット
	void SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

	// オイラー角を取得
	DirectX::SimpleMath::Vector3 GetEularAngles() const;

	// クォータニオンを取得
	DirectX::SimpleMath::Quaternion GetRotation() const;

	DirectX::SimpleMath::Vector3 Right()const { return right; }	// 右ベクトル
	DirectX::SimpleMath::Vector3 Up()const { return up; }	// 上ベクトル
	DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// 正面ベクトル
};

