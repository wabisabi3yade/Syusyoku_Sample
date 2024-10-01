#pragma once
#include "AnimationNode_Base.h"

class AnimationParameters;

/// @brief 2つ以上のアニメーションをブレンドされているノード
class BlendAnimationNode : public AnimationNode_Base
{
	/// @brief ブレンドで対応している軸数
	static constexpr int axisCnt = 2;
public:
	// ブレンドに使用するデータ
	struct BlendData
	{
		const AnimationData* pAnimation{ nullptr }; // アニメーションデータ
		float ratioX{ 0.0f };	// X軸ブレンドの割合、1軸のみ時使用
		float ratioY{ 0.0f };	// Y軸ブレンドの割合
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

	/// @brief アニメーションパラメータのポインタ
	const AnimationParameters* pAnimParameters;

	/// @brief 現在の割合
	float curBlendRatio{ 0.0f };

	/// @brief ターゲット割合
	float targetBlendRatio{ 0.0f };

	/// @brief ターゲット割合とする変数のポインタ
	std::array<float*, axisCnt> targetBlendRatios;

	/// @brief ターゲット割合決定時の割合
	float changeBlendRatio{ 0.0f };

	/// @brief ターゲットの割合へ移動するまでの時間
	float ratioSmoothTime{ 1.0f };

	/// @brief 現在の割合移動時間
	float curRatioSmoothTime{ 0.0f };

	/// @brief ターゲットの割合へ移動イージング
	HashiTaku::EaseKind ratioMoveEase{ HashiTaku::EaseKind::OutCubic };
public:
	BlendAnimationNode(const AnimationParameters& _animParams, std::string _nodeName);
	~BlendAnimationNode() {}

	void ImGuiPlaying() override;

	void Begin() override;

	/// @brief 更新処理を行う
	/// @param _playingRatio 再生の割合
	/// @param _boneList ボーンリスト
	void Update(BoneList& _boneList) override;

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

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 現在のブレンド値を移動する
	void MoveCurBlend();

	/// @brief アニメーションの更新処理
	/// @param _boneList 更新するボーンリスト
	void AnimationUpdate(BoneList& _boneList);

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
	void SingleUpdateAnimation(BlendData& _animationData, BoneList& _boneList);

	/// @brief ブレンドアニメーション更新
	/// @param _blendPair ブレンドする2つのデータ
	/// @param _playingRatio 再生割合
	/// @param _boneList ボーン配列
	void BlendUpdateAnimation(BlendPair& _blendPair, BoneList& _boneList);

	/// @brief ブレンドデータをアニメーション名から探す
	/// @param _animName アニメーション名
	/// @return 対応するブレンドデータ
	BlendData* FindBlendData(const std::string& _animName);

	/// @brief 0.0〜1.0の範囲内にする
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

	/// @brief 現在のアニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	void GetCurAnimTransform(BoneTransform& _outTransform, u_int _boneId) const override;

	/// @brief アニメーションのトランスフォーム取得
	/// @param _outTransform 格納するボーントランスフォーム
	/// @param _boneId ボーンのID
	/// @param _requestRatio 取得する指定のアニメーション割合
	void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const override;

private:
	void ImGuiSetting() override;

	// ブレンドのターゲット変数を変更する
	void ImGuiChangeTargetParam();

	// ブレンドデータをセーブする
	nlohmann::json SaveBlendData(BlendData& _blendData);

	// ブレンドデータをロードする
	void LoadBlendData(const nlohmann::json& _blendData);
};

