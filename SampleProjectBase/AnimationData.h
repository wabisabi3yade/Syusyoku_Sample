#pragma once
#include "Asset_Base.h"

#include "AnimationChannel.h"

/// @brief アニメーションのデータクラス
class AnimationData : public Asset_Base
{ 
private:
	// アニメーションノードリスト
	std::vector<std::unique_ptr<AnimationChannel>> pAnimChannels;

	/// @brief アニメーションの時間
	float animationTime;

public:
	AnimationData() : animationTime(0.0f) {}
	~AnimationData() {}

	/// @brief アニメーションノードを追加する
	/// @param _pAnimNode アニメーションノード
	void AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode);

	/// @brief 〇番目のノードに対応するボーン名を取得
	/// @param _nodeId ノードのId
	/// @return ボーンの名前
	std::string GetBoneName(u_int _nodeId);

	// チャンネルの数を取得
	u_int GetChannelCount() const;

	/// @brief スケールを求める
	/// @param _nodeId ノードID
	/// @param _playingTime 再生時間
	/// @return アニメーションのスケール
	DirectX::SimpleMath::Vector3 GetScale(u_int _nodeId, float _playingTime) const;

	/// @brief クォータニオンを求める
	/// @param _nodeId ノードID
	/// @param _playingTime 再生時間
	/// @return アニメーションのクォータニオン
	DirectX::SimpleMath::Quaternion GetQuaternion(u_int _nodeId, float _playingTime) const;

	/// @brief 座標を求める
	/// @param _nodeId ノードID
	/// @param _playingTime 再生時間
	/// @return アニメーションの座標
	DirectX::SimpleMath::Vector3 GetPosition(u_int _nodeId, float _playingTime) const;

	// アニメーション全体の時間を取得
	float GetAnimationTime() const;
};

