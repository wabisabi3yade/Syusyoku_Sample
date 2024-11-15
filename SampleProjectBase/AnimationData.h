#pragma once
#include "AssetPath_Base.h"

#include "AnimationChannel.h"

struct BoneTransform;

/// @brief アニメーションのデータクラス
class AnimationData : public AssetPath_Base
{
private:
	/// @brief アニメーションチャンネルリスト
	std::vector<std::unique_ptr<AnimationChannel>> pAnimChannels;

	/// @brief ルートモーションのアニメーションチャンネル
	std::unique_ptr<AnimationChannel> pRootMotionChannels;

	/// @brief 対応ボーンリスト名
	std::string boneListName;

	/// @brief ルートモーションで移動する移動速度
	DirectX::SimpleMath::Vector3 rootMovePosPerSec;

	/// @brief ルートボーンのインデックス(-1は設定なし)
	u_int rootBoneId;

	/// @brief アニメーションの時間(s)
	float animationTime_s;

	/// @brief 1キーごとの時間(s)
	float timePerKey_s;

	/// @brief 全体のフレーム数
	u_int allFrameCnt;

	/// @brief 右手系か？
	bool isRightHand;
public:
	AnimationData();
	~AnimationData() {}

	/// @brief アニメーションノードを追加する
	/// @param _pAnimNode アニメーションノード
	void AddAnimationChannel(std::unique_ptr<AnimationChannel> _pAnimNode);

	/// @brief ボーンIDからチャンネルを取得
	/// @param _boneIdx ボーンID
	/// @return アニメーションチャンネル
	const AnimationChannel* FindChannel(u_int _boneIdx) const;

	/// @brief ルートモーション関係のパラメータを求める
	void CalcRootMotion(u_int _rootBoneId);

	// ボーンリスト名をセット
	void SetBoneListName(const std::string& _boneListName);

	// アニメーション時間をセット
	void SetAnimationTime(float _animTime);

	/// @brief 全体のアニメーションのフレーム数をセット
	/// @param _allFrameCnt 全体のフレーム数
	void SetAllFrameCnt(u_int _allFrameCnt);

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

	/// @brief アニメーションのキー数を取得
	/// @return アニメーションのキー数
	u_int GetAllAnimationFrame() const;

	/// @brief 割合からキーに変換
	/// @param _ratio 割合
	/// @return キー
	u_int GetRatioToFrame(float _ratio);

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
	/// @param _outTransform 結果ボーントランスフォーム
	void GetTransformByRatio(u_int _boneId, float _playingRatio, BoneTransform& _outTransform) const;

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
	
	/// @brief ルートモーションの移動速度を求める(秒
	/// @return ルートモーションの移動速度
	const DirectX::SimpleMath::Vector3& GetRootMotionPosSpeedPerSec() const;
	
	/// @brief ルートモーションの移動座標を割合から取得する
	/// @param _ratio 取得したい割合
	/// @return ルートモーションでの移動座標
	DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio) const;

	/// @brief ルートモーションの回転量を割合から取得する
	/// @param _ratio 取得したい割合
	/// @return ルートモーションでの移動座標
	DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio) const;

	// アニメーション全体の時間を取得
	float GetAnimationTime() const;

	/// @brief セーブ
	/// @return データ
	nlohmann::json Save() override;
};

