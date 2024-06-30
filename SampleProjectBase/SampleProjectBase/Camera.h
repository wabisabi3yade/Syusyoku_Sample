#pragma once
#include "GameObject.h"

// ゲーム内カメラクラス
class Camera : public GameObject
{
	// カメラの注視点
	DirectX::SimpleMath::Vector3 focusPos{ 0,0,0 };
	// カメラの上ベクトル
	DirectX::SimpleMath::Vector3 camUp{ 0,1.f,0 };

	float fov;	// 視野角
	// 描画距離
	float nearZ;	// 最短
	float farZ;	 // 最長
	bool isOrthographic;	// 平行投影をするか
	void UpdatePerspective(u_int _viewPortSlot = 0);	// 透視投影をセットする
	void UpdateOrthographic(u_int _viewPortSlot = 0);	// 透視投影をセットする
public:
	Camera();
	~Camera();

	void UpdateViewMatrix();	// ビュー変換行列を更新
	
	// 各プロジェクションに設定
	void SetOrthographic();	// 平行投影
	void SetPerspective();	// 透視投影

	void SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos);
};

