#pragma once

// 座標・回転・スケールをまとめたパラメータ
class Transform
{
	/// @brief 親トランスフォーム
	Transform* pParent;

	/// @brief 子トランスフォーム
	std::list<Transform*> pChilds;

	// 各方向の単位ベクトル
	DirectX::SimpleMath::Vector3 up;	// 上
	DirectX::SimpleMath::Vector3 right;	// 右
	DirectX::SimpleMath::Vector3 forward;	// 前

	// ワールド
	DirectX::SimpleMath::Vector3 position;	// 座標
	DirectX::SimpleMath::Vector3 scale;	// スケール
	DirectX::SimpleMath::Vector3 eularAngles;	// 回転角度
	DirectX::SimpleMath::Quaternion rotation;	// クォータニオン

	// ローカル
	DirectX::SimpleMath::Vector3 localPosition;	// 座標
	DirectX::SimpleMath::Vector3 localScale;	// スケール
	DirectX::SimpleMath::Vector3 localEularAngles;	// 回転角度
	DirectX::SimpleMath::Quaternion localRotation;	// クォータニオン

protected:
	/// @brief 親トランスフォームがあるか？
	bool isHaveParent;

public:
	Transform(bool _isInit = true);
	virtual ~Transform();

	// 方向ベクトルを更新する
	void UpdateVector();

	// その座標にオブジェクトを向ける
	void LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector = DirectX::SimpleMath::Vector3::Up);

	/// @brief 親トランスフォームを非設定にする
	void RemoveParent();

	/// @brief 親トランスフォームを設定する
	/// @param _parent 親トランスフォームの参照
	void SetParent(Transform& _parent);

	/// @brief 子トランスフォームを設定する
	/// @param _parent 子トランスフォームの参照
	virtual void SetChild(Transform& _child);

	// ワールド
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& _pos);
	virtual void SetScale(const DirectX::SimpleMath::Vector3& _scale);
	virtual void SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
	virtual void SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

	// ローカル
	virtual void SetLocalPos(const DirectX::SimpleMath::Vector3& _position);
	virtual void SetLocalScale(const DirectX::SimpleMath::Vector3& _scale);
	virtual void SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
	virtual void SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

	// ワールド
	DirectX::SimpleMath::Vector3 GetPosition() const;
	DirectX::SimpleMath::Vector3 GetScale() const;
	DirectX::SimpleMath::Vector3 GetEularAngles() const;
	DirectX::SimpleMath::Quaternion GetRotation() const;

	// ローカル
	DirectX::SimpleMath::Vector3 GetLocalPosition() const;
	DirectX::SimpleMath::Vector3 GetLocalScale() const;
	DirectX::SimpleMath::Vector3 GetLocalEularAngles() const;
	DirectX::SimpleMath::Quaternion GetLocalRotation() const;

	DirectX::SimpleMath::Vector3 Right()const { return right; }	// 右ベクトル
	DirectX::SimpleMath::Vector3 Up()const { return up; }	// 上ベクトル
	DirectX::SimpleMath::Vector3 Forward()const { return forward; }	// 正面ベクトル

	//　子トランスフォームの数を取得
	u_int GetChilidCnt() const;

private:
	//  子トランスフォームパラメータを更新(再帰関数)
	// 座標
	void UpdateHierarchyPositions();
	// スケール
	void UpdateHierarchyScales();
	// 回転
	void UpdateHierarchyRotations();
};