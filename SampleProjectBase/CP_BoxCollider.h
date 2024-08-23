#pragma once
#include "CP_Collider.h"

class CP_BoxCollider : public CP_Collider, public CloneComponent<CP_BoxCollider>
{
	/// @brief 中心座標
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief 長さ
	DirectX::SimpleMath::Vector3 length;
	
	/// @brief 右方向ベクトル
	DirectX::SimpleMath::Vector3 rightVector;

	/// @brief 上方向ベクトル
	DirectX::SimpleMath::Vector3 upVector;

	/// @brief 前方向ベクトル
	DirectX::SimpleMath::Vector3 forwardVector;

	/// @brief 座標オフセット
	DirectX::SimpleMath::Vector3 posOffset;

	//// 傾き
	//DirectX::SimpleMath::Vector3 angleOffset;  

	// 大きさオフセット
	DirectX::SimpleMath::Vector3 sizeOffset; 

	// AABBフラグ
	bool isAABB = false;	

public:
	CP_BoxCollider();
	~CP_BoxCollider() {}

	void Start();

	void LateUpdate() override;

	void Draw() override;

	void ImGuiSetting() override;

	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { posOffset = _offset; } 
	//void SetAngle(const DirectX::SimpleMath::Vector3& _angle) { angleOffset = _angle; }    // 角度
	void SetSize(const DirectX::SimpleMath::Vector3& _size) { sizeOffset = _size; } // 大きさ

	// ワールド座標系へ変換
	DirectX::SimpleMath::Vector3 GetCenterPos()const;
	DirectX::SimpleMath::Vector3 GetLength()const;

	// 各方向ベクトル
	DirectX::SimpleMath::Vector3 GetRightVector()const;
	DirectX::SimpleMath::Vector3 GetUpVector()const;
	DirectX::SimpleMath::Vector3 GetForwardVector()const;

	static bool CollisionBox(CP_Collider& _box1, CP_Collider& _box2);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief モデルのサイズから当たり判定の大きさを決める
	void SizeFromModelSize();

	/// @brief 座標更新
	void PosUpdate();
	
	/// @brief 方向更新
	void VectorUpdate();

	/// @brief 長さ更新
	void LengthUpdate();
};

