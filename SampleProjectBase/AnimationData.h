#pragma once
#include "Asset_Base.h"

#include "AnimationChannel.h"

/// @brief アニメーションのデータクラス
class AnimationData : public Asset_Base
{ 
private:
	// アニメーションノードリスト
	std::vector<std::unique_ptr<AnimationChannel>> pAnimChannels;

	/// @brief アニメーションの時間(s)
	float animationTime_s;

	/// @brief 1キーごとの時間(s)
	float timePerKey_s;

public:
	AnimationData() : animationTime_s(0.0f), timePerKey_s(0.0f) {}
	~AnimationData() {}

	/// @brief アニメーションノードを追加する
	/// @param _pAnimNode アニメーションノード
	void AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode);

	/// @brief ボーンIDからチャンネルを取得
	/// @param _boneIdx ボーンID
	/// @return アニメーションチャンネル
	const AnimationChannel* FindChannel(u_int _boneIdx) const;

	// アニメーション時間をセット
	void SetAnimationTime(float _animTime);

	// 1キーごとのの時間をセット
	void SetTimePerKey(float _timePerKey);

	/// @brief 〇番目のノードに対応するボーン名を取得
	/// @param _nodeId ノードのId
	/// @return ボーンの名前
	std::string GetBoneName(u_int _nodeId) const;

	/// @brief 〇番目のノードに対応するボーンIDを取得
	/// @param _nodeId ノードのId
	/// @return ボーンID
	u_int GetBoneIdx(u_int _nodeId) const;

	// チャンネルの数を取得
	u_int GetChannelCount() const;

	/// @brief スケールを求める
	/// @param _boneId ボーンID
	/// @param _playingTime 再生時間
	/// @return アニメーションのスケール
	DirectX::SimpleMath::Vector3 GetScale(u_int _boneId, float _playingTime) const;

	/// @brief クォータニオンを求める
	/// @param _boneId ボーンID
	/// @param _playingTime 再生時間
	/// @return アニメーションのクォータニオン
	DirectX::SimpleMath::Quaternion GetQuaternion(u_int _boneId, float _playingTime) const;

	/// @brief 座標を求める
	/// @param _boneId ボーンID
	/// @param _playingTime 再生時間
	/// @return アニメーションの座標
	DirectX::SimpleMath::Vector3 GetPosition(u_int _boneId, float _playingTime) const;

	// アニメーション全体の時間を取得
	float GetAnimationTime() const;
};

