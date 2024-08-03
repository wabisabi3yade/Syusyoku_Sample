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

	// 前のキー数を探す
	u_int FindPrevPosKey(float _playingTime, float _timePerKey) const;
	u_int FindPrevScaleKey(float _playingTime, float _timePerKey) const;
	u_int FindPrevQuatKey(float _playingTime, float _timePerKey) const;

	// 対応ボーンのIDをセット
	void SetBoneIdx(u_int _boneIdx);

	// 名前をセット
	void SetName(const std::string& _name);

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

	///// @brief 再生時間から座標を取得する
	///// @param _playingTime 再生時間
	///// @return ボーンの座標
	//DirectX::SimpleMath::Vector3 GetPosition(float _playingTime) const;

	///// @brief 再生時間からスケールを取得する
	///// @param _playingTime 再生時間
	///// @return ボーンのスケール
	//DirectX::SimpleMath::Vector3 GetScaling(float _playingTime) const;

	///// @brief 再生時間からクォータニオンを取得する
	///// @param _playingTime 再生時間
	///// @return ボーンのクォータニオン
	//DirectX::SimpleMath::Quaternion GetQuaternion(float _playingTime) const;

	// 各キーの数を取得する
	u_int GetPosKeyCnt() const;
	u_int GetScaleKeyCnt() const;
	u_int GetQuatKeyCnt() const;

	// 対応ボーンのIDを取得
	u_int GetBodeIdx() const;

	// 名前を取得
	std::string GetName();
private:

	//// 再生時間から補間を考慮したパラメータを取得する
	//// _prevKeyNum　前のキー数
	//// _playingTime 再生時間
	//DirectX::SimpleMath::Vector3 CalcInterpolatedPosition(u_int _prevKeyNum, float _playingTime) const;
	//DirectX::SimpleMath::Vector3 CalcInterpolatedScale(u_int _prevKeyNum, float _playingTime) const;
	//DirectX::SimpleMath::Quaternion CalcInterpolatedQuaternion(u_int _prevKeyNum, float _playingTime) const;
};

