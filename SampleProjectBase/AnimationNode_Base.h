#pragma once

#include "AnimationData.h"

class BoneList;

// アニメーションノードに基底クラス
class AnimationNode_Base
{
public:
	// ノードの種類
	enum class NodeType
	{
		Single,	// 一つだけのアニメーション
		Blend	// ブレンドされているアニメーション
	};

private:
	/// @brief ノード名
	std::string nodeName;

	/// @brief ノードの種類
	NodeType nodeType;

	/// @brief アニメーションの時間
	float animationTime;

	/// @brief ループ再生するか？
	bool isLoop;
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type) : nodeName(_nodeName), nodeType(_type), animationTime(0.0f), isLoop(true) {}
	virtual ~AnimationNode_Base() {}

	// 再生中のときに表示
	virtual void ImGuiPlaying();

	/// @brief 更新処理を行う
	/// @param 再生割合
	/// @param _boneList ボーンリスト
	virtual void Update(float _playingRatio, BoneList& _boneList) = 0;

	/// @brief アニメーションを追加する
	/// @param _animData アニメーション名
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// ノード名をセット
	void SetNodeName(const std::string& _nodeName);

	// ノード名取得
	std::string GetNodeName() const;

	// ノードの種類を取得
	NodeType GetNodeType() const;

	// 再生時間を取得
	float GetAnimationTime() const;

	/// @brief ループ再生フラグを取得
	/// @return ループ再生フラグ
	bool GetIsLoop() const;

	/// @brief アニメーションのトランスフォーム取得
	/// @param _transforms 格納するボーントランスフォーム配列
	/// @param _boneNum ボーンの数
	/// @param _requestKeyNum 取得したいキー数
	virtual void GetAnimTransform(std::vector<BoneTransform>& _transforms, u_int _boneNum, u_int _requestKeyNum) const = 0;
protected:

	// 再生時間をセットする
	void SetAnimationTime(float _time);
};

