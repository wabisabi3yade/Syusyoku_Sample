#pragma once
#include "Asset_Base.h"

struct aiScene;
struct aiAnimation;
struct aiNodeAnim;
struct aiNode;

/// @brief アニメーションのデータクラス
class AnimationData : public Asset_Base
{ 
private:

	// ↓Assimpの型から独自の型へ変換させる 20240722
	const aiScene* pAiScene;

	// ↓Assimpの型から独自の型へ変換させる 20240722
	const aiAnimation* pAnimationData;

	// アニメーションのボーンID配列
	std::vector<u_int> boneIndicies;

	/// @brief アニメーションの時間
	float animationTime;

public:
	AnimationData() : pAiScene(nullptr), pAnimationData(nullptr), animationTime(0.0f) {}
	~AnimationData() {}

	// ノードに2キー以上あるか確認する(各パラメータ)
	// 引数：_nodeId ノードID
	// 戻り値：2つ以上あるか？
	bool HasScaleTwoKeys(u_int _nodeId) const;
	bool HasQuatTwoKeys(u_int _nodeId) const;
	bool HasPosTwoKeys(u_int _nodeId) const;

	// アニメーションセット
	void SetAiScene(const aiScene* _pAiScene);

	/// @brief ノードに対応したボーンのIDをセットする
	/// @param _nodeId ノードID 
	void SetBoneIdx(u_int _nodeId, u_int _boneIdx);

	/// @brief 〇番目のノードに対応するボーン名を取得
	/// @param _nodeId ノードのId
	/// @return ボーンの名前
	std::string GetBoneName(u_int _nodeId);

	// チャンネルの数を取得
	u_int GetChannelCount();

	/// @brief スケールを求める
	/// @param _nodeId ノードID
	/// @param _playingTime 再生時間
	/// @return アニメーションのスケール
	DirectX::SimpleMath::Vector3 GetScale(u_int _nodeId, float _playingTime) const;
	DirectX::SimpleMath::Vector3 GetScale(u_int _nodeId, u_int _flame) const;

	/// @brief クォータニオンを求める
	/// @param _nodeId ノードID
	/// @param _playingTime 再生時間
	/// @return アニメーションのクォータニオン
	DirectX::SimpleMath::Quaternion GetQuaternion(u_int _nodeId, float _playingTime) const;
	DirectX::SimpleMath::Quaternion GetQuaternion(u_int _nodeId, u_int _flame) const;

	/// @brief 座標を求める
	/// @param _nodeId ノードID
	/// @param _playingTime 再生時間
	/// @return アニメーションの座標
	DirectX::SimpleMath::Vector3 GetPosition(u_int _nodeId, float _playingTime) const;
	DirectX::SimpleMath::Vector3 GetPosition(u_int _nodeId, u_int _flame) const;

	// アニメーション全体の時間を取得
	float GetAnimationTime() const;

	// 階層のルートであるノードを返す
	const aiNode* GetRootNode();

	// ノード名からアニメーションノードを取得 
	const aiNodeAnim* GetAiNodeAnim(const std::string& _nodeName);

private:
	/// @brief アニメーションの再生時間を求める
	void CalculateAnimTime();

	// 再生時間から各パラメータのキーを取得する
	// 引数：_playingTime 再生時間　_pAiNodeAnim ノード
	u_int FindPreviousRotKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const;
	u_int FindPreviousScaleKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const;
	u_int FindPreviousPosKey(float _playingTime, const aiNodeAnim* _pAiNodeAnim) const;

	// 各パラメータをノードIDとキー値から取得する
	// 引数：_nodeId ノードID　 u_int _key キー
	DirectX::SimpleMath::Vector3 GetScaleByKey(u_int _nodeId, u_int _key) const;
	DirectX::SimpleMath::Quaternion GetQuatByKey(u_int _nodeId, u_int _key) const;
	DirectX::SimpleMath::Vector3 GetPosByKey(u_int _nodeId, u_int _key) const;
};

