#pragma once
#include "AnimationData.h"

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
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type);
	virtual ~AnimationNode_Base() {}
	AnimationNode_Base(const AnimationNode_Base& _other);
	AnimationNode_Base& operator=(const AnimationNode_Base& _other);

	// 再生中のときに表示
	virtual void ImGuiPlaying();

	/// @brief アニメーション開始時に行う処理
	virtual void Begin();

	/// @brief 更新処理
	/// @param _boneList ボーンリスト
	virtual void UpdateCall(BoneList& _boneList);

	/// @brief 現在の再生割合を進める
	/// @param _animationSpeed 再生速度
	void ProgressPlayRatio(float _animationSpeed);

	/// @brief アニメーションを追加する
	/// @param _animData アニメーション名
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// 現在の再生割合をセット
	void SetCurPlayRatio(float _playingRatio);

	// ノード名をセット
	void SetNodeName(const std::string& _nodeName);

	// ループするかセット
	void SetIsLoop(bool _isLoop);

	/// @brief 終了フラグをセットする
	void SetFinish();

	// ノード名取得
	std::string GetNodeName() const;

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

	/// @brief 現在のアニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	virtual void GetCurAnimTransform(BoneTransform& _outTransform, u_int _boneId) const = 0;

	/// @brief アニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	/// @param _requestRatio 取得する指定のアニメーション割合
	virtual void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const = 0;

	/// @brief 再生速度倍率を求める
	/// @return 再生速度倍率
	float GetPlaySpeedTimes() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:

	/// @brief ループできるかチェック
	/// @return ループできるか？
	bool IsCanLoop();

	void Copy(const AnimationNode_Base& _other);

	// ノードのパラメーター設定
	void ImGuiSetParameter();
protected:
	/// @brief 更新処理を行う
	/// @param _boneList ボーンリスト
	virtual void Update(BoneList& _boneList) = 0;

	// 再生時間をセットする
	void SetAnimationTime(float _time);

	void ImGuiSetting() override;
};

