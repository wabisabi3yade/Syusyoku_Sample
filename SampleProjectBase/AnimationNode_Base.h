#pragma once
#include "AnimationData.h"
#include "AnimationCurve.h"

class BoneList;

/// @brief アニメーションノードに基底クラス
class AnimationNode_Base : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
public:
	// ノードの種類
	enum class NodeType
	{
		Single,	// 一つだけのアニメーション
		Blend,	// ブレンドされているアニメーション
	};

#ifdef EDIT
	/// @brief 列挙型を文字列に変換
	static std::vector<std::string> edit_nodeTypeStrings;
#endif // EDIT
private:
	/// @brief ノード名
	std::string nodeName;

	/// @brief アニメーションカーブ
	std::unique_ptr<AnimationCurve> pAnimationCurve;

	/// @brief ノードの種類
	NodeType nodeType;

	/// @brief 現在の再生時間
	float curPlayingRatio;

	/// @brief 現在の再生時間
	float lastPlayingRatio;

	/// @brief ノードの再生速度倍率(1.0は等速)
	float playNodeSpeedTimes;

	/// @brief アニメーションの時間
	float animationTime;

	/// @brief ループ再生するか？
	bool isLoop;

	/// @brief アニメーション終了しているか？
	bool isFinish;

	/// @brief 移動座標のルートモーションを適用するか(XZ座標)
	bool isRootMotionPosXZ;

	/// @brief 移動座標のルートモーションを適用するか(Y座標)
	bool isRootMotionPosY;

	/// @brief 回転量のルートモーションを適用するか
	bool isRootMotionRot;
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type);
	virtual ~AnimationNode_Base() {}
	/*AnimationNode_Base(const AnimationNode_Base& _other);
	AnimationNode_Base& operator=(const AnimationNode_Base& _other);*/

	// 再生中のときに表示
	virtual void ImGuiPlaying();

	/// @brief アニメーションを追加する
	/// @param _animData アニメーション名
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// ノード名をセット
	void SetNodeName(const std::string& _nodeName);

	// ループするかセット
	void SetIsLoop(bool _isLoop);

	/// @brief 終了フラグをセットする
	void SetFinish();

	// ノード名取得
	const std::string& GetNodeName() const;

	// ノードの種類を取得
	NodeType GetNodeType() const;

	// 現在の再生割合を取得
	float GetCurPlayRatio() const;

	/// @brief 1フレーム前の再生割合を取得
	float GetLastPlayRatio() const;

	// 再生時間を取得
	float GetAnimationTime() const;

	/// @brief ループ再生フラグを取得
	/// @return ループ再生フラグ
	bool GetIsLoop() const;

	// アニメーション終了フラグを取得する
	bool GetIsFinish() const;

	/// @brief XZ軸をルートモーションで移動するか？
	/// @return 移動するか？
	bool GetIsRootMotionXZ() const;

	/// @brief Y軸をルートモーションで移動するか？
	/// @return 移動するか？
	bool GetIsRootMotionY() const;

	/// @brief ルートモーションで回転するか？
	/// @return 回転するか？
	bool GetIsRootMotionRot() const;

	/// @brief アニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	/// @param _requestRatio 取得する指定のアニメーション割合
	virtual void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const {};

	/// @brief 再生速度倍率を求める
	/// @return 再生速度倍率
	float GetPlaySpeedTimes() const;

	/// @brief アニメーションカーブの値を取得する
	/// @param _ratio 割合(0.0～1.0)
	/// @return アニメーションカーブの値(基本的に0.0～1.0)
	float GetCurveValue(float _ratio) const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void Copy(const AnimationNode_Base& _other);

	// ノードのパラメーター設定
	void ImGuiSetParameter();
protected:
	/// @brief 更新処理を行う
	/// @param _boneList ボーンリスト
	virtual void Update(BoneList& _boneList) {};

	// 再生時間をセットする
	void SetAnimationTime(float _time);

	void ImGuiSetting() override;

};

