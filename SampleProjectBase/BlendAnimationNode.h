#pragma once
#include "AnimationNode_Base.h"

/// @brief 2つ以上のアニメーションをブレンドされているノード
class BlendAnimationNode : public AnimationNode_Base
{
public:
	// ブレンドに使用するデータ
	struct BlendData
	{
		const AnimationData* pAnimation{ nullptr }; // アニメーションデータ
		float ratio{ 0.0f };	// ブレンドの割合(0.0～1.0)
	};

private:
	// ブレンドするときのペア
	struct BlendPair
	{
		BlendData* prevData{ nullptr };	// 前
		BlendData* nextData{ nullptr };	// 後
	};

	/// @brief  ブレンドするアニメーションデータ
	std::list<BlendData> blendDatas;

	/// @brief 現在の割合
	float curBlendRatio{ 0.0f };

	/// @brief ターゲット割合
	float targetBlendRatio{ 0.0f };

	/// @brief ターゲット割合決定時の割合
	float changeBlendRatio{ 0.0f };

	/// @brief ターゲットの割合へ移動するまでの時間
	float ratioSmoothTime{ 1.0f };

	/// @brief 現在の割合移動時間
	float curRatioSmoothTime{ 0.0f };

	/// @brief ターゲットの割合へ移動イージング
	HashiTaku::EaseKind ratioMoveEase{ HashiTaku::EaseKind::OutCubic };
public:
	BlendAnimationNode(std::string _nodeName);

	~BlendAnimationNode() {}

	void ImGuiPlaying() override;

	/// @brief 更新処理を行う
	/// @param _playingRatio 再生の割合
	/// @param _boneList ボーンリスト
	void Update(float _playingRatio, BoneList& _boneList) override;

	/// @brief アニメーションを追加する
	/// @param _animData アニメーションデータ
	void SetAnimationData(const std::string& _animName) override;

	/// @brief 目標の割合をセットする
	/// @param _ratio 割合
	void SetTargetBlendRatio(float _ratio);

	/// @brief アニメーションの割合を変える。
	/// @param _ratio 割合
	/// @param _animName 変更するアニメーション
	void SetAnimationRatio(float _ratio, const std::string& _animName);
private:
	/// @brief 現在のブレンド値を移動する
	void MoveCurBlend();

	/// @brief アニメーションの更新処理
	/// @param _playingRatio 再生割合
	/// @param _boneList 更新するボーンリスト
	void AnimationUpdate(float _playingRatio, BoneList& _boneList);

	/// @brief 更新処理を行えるのか
	/// @return 更新できる返す
	bool IsCanUpdate();

	/// @brief 現在の割合からブレンドすべきペアを探す
	/// @return ブレンドすべき2つのブレンドデータ
	BlendPair FindBlendPair();

	/// @brief 単一のアニメーション更新
	/// @param _boneList ボーン配列
	/// @param _playingRatio 再生割合
	/// @param _boneList ボーン配列
	void SingleUpdateAnimation(BlendData& _animationData, float _playingRatio, BoneList& _boneList);

	/// @brief ブレンドアニメーション更新
	/// @param _blendPair ブレンドする2つのデータ
	/// @param _playingRatio 再生割合
	/// @param _boneList ボーン配列
	void BlendUpdateAnimation(BlendPair& _blendPair, float _playingRatio, BoneList& _boneList);

	/// @brief ブレンドデータをアニメーション名から探す
	/// @param _animName アニメーション名
	/// @return 対応するブレンドデータ
	BlendData* FindBlendData(const std::string& _animName);

	/// @brief 0.0～1.0の範囲内にする
	/// @param _ratio セットされる割合の参照
	/// @param _setRatio セットする割合
	void SetRatio(float& _ratio, float _setRatio);

	/// @brief ブレンド値を昇順にソートする
	void SortBlendValue();

	/// @brief データごとのブレンド値の比較関数
	/// @param _bd1 データ1
	/// @param _bd2 データ2
	/// @return  入れ替えるなら
	static bool CompareBlendValue(const BlendData& _bd1, const BlendData& _bd2);

	/// @brief アニメーションのトランスフォーム取得
	/// @param _transforms 格納するボーントランスフォーム配列
	/// @param _boneNum ボーンの数
	/// @param _requestKeyNum 取得したいキー数
	void GetAnimTransform(std::vector<BoneTransform>& _transforms, u_int _boneNum, u_int _requestKeyNum) const override;
};

