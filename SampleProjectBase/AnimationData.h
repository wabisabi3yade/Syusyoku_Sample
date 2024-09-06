#pragma once
#include "AssetPath_Base.h"

#include "AnimationChannel.h"

struct BoneTransform;

/// @brief アニメーションのデータクラス
class AnimationData : public AssetPath_Base
{
private:
	/// @brief アニメーションノードリスト
	std::vector<std::unique_ptr<AnimationChannel>> pAnimChannels;

	/// @brief 対応ボーンリスト名
	std::string boneListName;

	/// @brief アニメーションの時間(s)
	float animationTime_s;

	/// @brief 1キーごとの時間(s)
	float timePerKey_s;

	/// @brief 右手系か？
	bool isRightHand;
public:
	AnimationData() : animationTime_s(0.0f), timePerKey_s(0.0f), isRightHand(false) {}
	~AnimationData() {}

	/// @brief アニメーションノードを追加する
	/// @param _pAnimNode アニメーションノード
	void AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode);

	/// @brief ボーンIDからチャンネルを取得
	/// @param _boneIdx ボーンID
	/// @return アニメーションチャンネル
	const AnimationChannel* FindChannel(u_int _boneIdx) const;

	// ボーンリスト名をセット
	void SetBoneListName(const std::string& _boneListName);

	// アニメーション時間をセット
	void SetAnimationTime(float _animTime);

	// 1キーごとのの時間をセット
	void SetTimePerKey(float _timePerKey);

	// 右手系かセット
	void SetIsRightHand(bool _isRightHand);

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
	/// @param_playingRatio 再生割合
	/// @return アニメーションのスケール
	DirectX::SimpleMath::Vector3 GetScaleByRatio(u_int _boneId, float _playingRatio) const;

	/// @brief クォータニオンを求める
	/// @param _boneId ボーンID
	/// @param _playingRatio 再生割合
	/// @return アニメーションのクォータニオン
	DirectX::SimpleMath::Quaternion GetQuaternionByRatio(u_int _boneId, float _playingRatio) const;

	/// @brief 座標を求める
	/// @param _boneId ボーンID
	/// @param_playingRatio 再生割合
	/// @return アニメーションの座標
	DirectX::SimpleMath::Vector3 GetPositionByRatio(u_int _boneId, float _playingRatio) const;

	/// @brief トランスフォーム取得
	/// @param _boneId ボーンID
	/// @param _playingRatio 再生割合
	/// @return ボーンのトランスフォーム
	BoneTransform GetTransformByRatio(u_int _boneId, float _playingRatio) const;

	/// @brief スケールを求める
	/// @param _boneId ボーンID
	/// @param _requestKey 取得するキー
	/// @return アニメーションのスケール
	DirectX::SimpleMath::Vector3 GetScaleByKey(u_int _boneId, u_int _playingKey) const;

	/// @brief クォータニオンを求める
	/// @param _boneId ボーンID
	/// @param _requestKey 取得するキー
	/// @return アニメーションのクォータニオン
	DirectX::SimpleMath::Quaternion GetQuaternioneByKey(u_int _boneId, u_int _playingKey) const;

	/// @brief 座標を求める
	/// @param _boneId ボーンID
	/// @param _requestKey 取得するキー
	/// @return アニメーションの座標
	DirectX::SimpleMath::Vector3 GetPositioneByKey(u_int _boneId, u_int _playingKey) const;

	/// @brief トランスフォーム取得
	/// @param _boneId ボーンID
	/// @param _requestKey 取得するキー
	/// @return ボーンのトランスフォーム
	BoneTransform GetTransformByKey(u_int _boneId, u_int _playingKey) const;

	// アニメーション全体の時間を取得
	float GetAnimationTime() const;

	/// @brief セーブ
	/// @return データ
	nlohmann::json Save() override;
};

