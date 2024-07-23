#pragma once
#include "Asset_Base.h"

struct aiScene;
struct aiAnimation;

// アニメーションのデータクラス
class AnimationData : public Asset_Base
{ 
	// ↓Assimpの型から独自の型へ変換させる 20240722
	const aiScene* pAiScene;

	// ↓Assimpの型から独自の型へ変換させる 20240722
	const aiAnimation* pAnimationData;

	// アニメーションのボーンID配列
	std::vector<u_int> boneIndicies;

public:
	AnimationData() : pAiScene(nullptr), pAnimationData(nullptr) {}
	~AnimationData() {}

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
};

