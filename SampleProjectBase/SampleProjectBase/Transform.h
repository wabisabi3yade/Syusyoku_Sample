#pragma once

// 座標・回転・スケールをまとめたパラメータ
class Transform
{

public: 
	Float3 position;	// 座標
	Float3 rotation;	// 回転角度
	Float3 scale;	// スケール
	Float4 Quaternion;	// クォータニオン

	Transform();
	~Transform();	
};

