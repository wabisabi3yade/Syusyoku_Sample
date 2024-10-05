#pragma once
#include "SkeletalMesh.h"

#include "AnimationNode_Base.h"

/// @brief ノード再生の基底クラス
class AnimNodePlayer_Base
{
	/// @brief 現在の再生割合
	float curPlayRatio;

	/// @brief 1フレーム前の再生割合
	float lastPlayRatio;

	/// @brief ノードの再生速度
	float progressNodeSpeed;
protected:
	/// @brief 前回のルートモーション座標値
	DirectX::SimpleMath::Vector3 p_RootMotionPos;

	/// @brief 前回のルートモーション回転量
	DirectX::SimpleMath::Quaternion p_RootMotionRot;

	/// @brief 動かすボーン
	BoneList* pBoneList;

	/// @brief 再生するアニメーションノード
	const AnimationNode_Base* pPlayAnimNode;

	/// @brief ルートモーション時に移動するオブジェクトのトランスフォーム
	Transform* pObjectTransform;
public:
	AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	virtual ~AnimNodePlayer_Base(){}

	/// @brief 更新処理呼び出し
	/// @param _controllerPlaySpeed コントローラー自体の再生速度
	virtual void UpdateCall(float _controllerPlaySpeed);

	/// @brief 現在の再生割合をセット
	/// @param 再生割合
	void SetCurPlayRatio(float _playRatio);

	/// @brief 現在の再生割合を取得する
	/// @return 現在の再生割合
	float GetCurPlayRatio() const;

private:
	/// @brief 再生割合を進める
	/// @param _controllerPlaySpeed コントローラー自体の再生速度
	void ProgressPlayRatio(float _controllerPlaySpeed);

	/// @brief ループ再生できるか確認
	/// @return ループできるか？
	bool IsCanLoop() const;

	/// @brief 再生がループした時の処理
	void OnPlayLoop();

	/// @brief トランスフォームにルートモーションを反映する
	void ApplyRootMotionToTransform();
protected:
	/// @brief 各ノードプレイヤーの更新処理
	virtual void Update() = 0;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @return 現在の割合のルートモーション座標
	virtual DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) = 0;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @return 現在の割合のルートモーション回転量
	virtual DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio) = 0;
};

