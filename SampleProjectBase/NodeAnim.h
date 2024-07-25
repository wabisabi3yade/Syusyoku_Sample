#pragma once

struct aiNodeAnim;

class NodeAnim
{
	const aiNodeAnim* pAiNodeAnim;

	u_int boneIdx;	// アニメーションで動かすボーンのID

public:
	NodeAnim();
	~NodeAnim();

	DirectX::SimpleMath::Vector3 GetScaling(float _playTime) const;
	DirectX::SimpleMath::Quaternion GetQuaternion(float _playTime) const;
	DirectX::SimpleMath::Vector3 GetPosition(float _playTime) const;

	// 対応ボーンのIDをセット
	void SetBoneIdx(u_int _boneIdx);

	// 対応ボーンのIDを取得
	u_int GetBodeIdx() const;

private:
	// 再生時間から各パラメータのキーを取得する
	// 引数：_playingTime 再生時間
	u_int FindPreviousScaleKey(float _playingTime);
	u_int FindPreviousRotKey(float _playingTime);
	u_int FindPreviousPosKey(float _playingTime);
};

