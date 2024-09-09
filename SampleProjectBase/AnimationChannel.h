#pragma once

/// @brief Vector3型パラメータのアニメーションキー
struct AnimKey_V3
{
	DirectX::SimpleMath::Vector3 parameter;
	float startKeyNum;	// 開始のキー数
};

/// @brief Quaternion型パラメータのアニメーションキー
struct AnimKey_Q
{
	DirectX::SimpleMath::Quaternion parameter;
	float startKeyNum;	// 開始のキー数
};

/// @brief 1ボーンに対するアニメーション情報
class AnimationChannel
{
	/// @brief ノード名
	std::string channelName;

	/// @brief 座標
	std::vector<AnimKey_V3> positionKeys;

	/// @brief スケール
	std::vector<AnimKey_V3> scaleKeys;

	/// @brief クォータニオン
	std::vector<AnimKey_Q> quaternionKeys;

	/// @brief 対応しているボーンのID
	int boneIdx;
public:
	AnimationChannel() : boneIdx(-1) {}
	~AnimationChannel() {}

	/// @brief 座標キーを追加
	/// @param _startTime 開始キー数
	/// @param _position 座標
	void AddPosKey(float _startKeyNum, const DirectX::SimpleMath::Vector3& _position);

	/// @brief スケールキーを追加
	/// @param _startTime 開始キー数
	/// @param _scale スケール値
	void AddScaleKey(float _startKeyNum, const DirectX::SimpleMath::Vector3& _scale);

	/// @brief クォータニオンキーを追加
	/// @param _startTime 開始キー数
	/// @param _quaternion クォータニオン
	void AddQuatKey(float _startKeyNum, const DirectX::SimpleMath::Quaternion& _quaternion);

	// 割合から前のキー数を探す
	u_int FindPrevPosKey(float _playingRatio) const;
	u_int FindPrevScaleKey(float _playingRatio) const;
	u_int FindPrevQuatKey(float _playingRatio) const;

	// 対応ボーンのIDをセット
	void SetBoneIdx(u_int _boneIdx);

	// 名前をセット
	void SetName(const std::string& _name);

	/// @brief 座標キー数を割合から求める
	/// @param _ratio 再生割合
	/// @return 座標キー数
	float GetPosKeyByRatio(float _playingRatio) const;

	/// @brief スケールキー数を割合から求める
	/// @param _ratio 再生割合
	/// @return スケールキー数
	float GetScaleKeyByRatio(float _playingRatio) const;

	/// @brief 回転量キー数を割合から求める
	/// @param _ratio 再生割合
	/// @return 回転量キー数
	float GetQuatKeyByRatio(float _playingRatio) const;

	/// @brief キー数から座標を取得
	/// @param _keyNum キー数
	/// @return 座標キー
	const AnimKey_V3& GetPosKey(u_int _keyNum) const;

	/// @brief キー数からスケールを取得
	/// @param _keyNum キー数
	/// @return スケールキー
	const AnimKey_V3& GetScaleKey(u_int _keyNum) const;

	/// @brief キー数からクォータニオンを取得
	/// @param _keyNum キー数
	/// @return ボーンのクォータニオン
	const AnimKey_Q& GetQuatKey(u_int _keyNum) const;

	// 各キーの数を取得する
	u_int GetPosKeyCnt() const;
	u_int GetScaleKeyCnt() const;
	u_int GetQuatKeyCnt() const;

	// 対応ボーンのIDを取得
	u_int GetBodeIdx() const;

	// 名前を取得
	std::string GetName();

	// 各キーの進めた数を取得する
	u_int GetNextPosKey(u_int _currentKeyNum, int _progressNum = 1) const;
	u_int GetNextScaleKey(u_int _currentKeyNum, int _progressNum = 1) const;
	u_int GetNextQuatKey(u_int _currentKeyNum, int _progressNum = 1) const;
};
