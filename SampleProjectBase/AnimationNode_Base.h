#pragma once
#include "AnimationData.h"

#include "AnimTransitionArrow.h"

class BoneList;

/// @brief アニメーションノードに基底クラス
class AnimationNode_Base : public HashiTaku::IImGuiUser
{
public:
	// ノードの種類
	enum class NodeType
	{
		Single,	// 一つだけのアニメーション
		Blend,	// ブレンドされているアニメーション
	};

private:
	/// @brief ノード名
	std::string nodeName;

	/// @brief 遷移に関するデータを持った矢印リスト
	std::list<std::unique_ptr<AnimTransitionArrow>> pFromArrows;

	///// @brief 遷移に関するデータを持った矢印リスト
	//std::vector<AnimTransitionArrow*> pToArrows;

	/// @brief ノードの種類
	NodeType nodeType;

	/// @brief アニメーションの時間
	float animationTime;

	/// @brief ループ再生するか？
	bool isLoop;

	/// @brief アニメーション終了しているか？
	bool isFinish;
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type) : nodeName(_nodeName), nodeType(_type), animationTime(0.0f), isLoop(true), isFinish(false) {}
	virtual ~AnimationNode_Base() {}

	// 再生中のときに表示
	virtual void ImGuiPlaying();

	/// @brief アニメーション開始時に行う処理
	virtual void Begin();

	/// @brief 更新処理を呼び出す
	virtual void UpdateCall(float _playingRatio, BoneList& _boneList);

	/// @brief 遷移するか確認する
	/// @return 遷移先データ nullptrなら遷移しない
	AnimTransitionArrow* CheckTransition();

	/// @brief アニメーションを追加する
	/// @param _animData アニメーション名
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// ノード名をセット
	void SetNodeName(const std::string& _nodeName);

	// ループするかセット
	void SetIsLoop(bool _isLoop);

	/// @brief 終了フラグをセットする
	void SetFinish();

	// アニメーション遷移矢印を追加
	void AddTransitionArrow(std::unique_ptr<AnimTransitionArrow> _setArrow);

	// ノード名取得
	std::string GetNodeName() const;

	// ノードの種類を取得
	NodeType GetNodeType() const;

	// 再生時間を取得
	float GetAnimationTime() const;

	/// @brief ループ再生フラグを取得
	/// @return ループ再生フラグ
	bool GetIsLoop() const;

	// アニメーション終了フラグを取得する
	bool GetIsFinish() const;

	/// @brief アニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	/// @param _requestRatio 取得する指定のアニメーション割合
	virtual void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const = 0;
protected:
	/// @brief 更新処理を行う
	/// @param 再生割合
	/// @param _boneList ボーンリスト
	virtual void Update(float _playingRatio, BoneList& _boneList) = 0;

	// 再生時間をセットする
	void SetAnimationTime(float _time);

	void ImGuiSetting() override;
};

