#pragma once
#include "AnimationNode_Base.h"

class AnimationParameters;

/// @brief 2つ以上のアニメーションをブレンドされているノード
class BlendAnimationNode : public AnimationNode_Base
{
public:
	/// @brief ブレンドスペース内で使用するアニメーションデータ(ブレンドポイント)
	struct AnimPointData
	{
		/// @brief アニメーションデータ
		const AnimationData* pAnimation{ nullptr };
		/// @brief ブレンド値
		DirectX::SimpleMath::Vector2 blendValue;
	};

	/// @brief 姿勢を求めるときにブレンドするときの1ブレンドパラメータ
	struct BlendingData
	{
		/// @brief アニメーションデータ
		const AnimationData* pAnimation{ nullptr };

		/// @brief どれだけブレンドするか重み(合計で1.0)
		float blendWeight{ 0.0f };
	};

private:
	/// @brief 軸ごとのパラメータ
	struct AxisParameter
	{
		/// @brief ブレンド値の目標にするパラメータ名
		std::string targetParamName;

		/// @brief 最大範囲の値
		float maxRangeValue{ 1.0f };

		/// @brief 最小範囲の値
		float minRangeValue{ 0.0f };

		/// @brief ブレンド値の移動時間
		float blendMoveTime{ 1.0f };

		/// @brief ターゲットの割合へ移動イージング
		HashiTaku::EaseKind blendMoveEase{ HashiTaku::EaseKind::OutCubic };
	};

	/// @brief ポイントとの距離を求める構造
	struct PointToDistance
	{
		const BlendAnimationNode::AnimPointData* point{ nullptr };
		float distance = 0.0f;
	};

	/// @brief ブレンドで対応している最大軸数
	static constexpr unsigned short MAX_AXIS_CNT = 2;

	/// @brief ブレンドで対応している最低軸数
	static constexpr unsigned short MIN_AXIS_CNT = 1;

	/// @brief  ブレンドするアニメーションデータ
	std::list<AnimPointData> animationPoints;

	/// @brief 軸ごとのパラメータ変数
	std::vector<AxisParameter> axisParameters;

	/// @brief アニメーションパラメータのポインタ
	const AnimationParameters* pAnimParameters;

	/// @brief アニメーション間でブレンドする(しない場合、ウェイトが大きいアニメーションだけ)
	bool isUseBlend;
public:
	BlendAnimationNode(const AnimationParameters& _animParams, std::string _nodeName);
	~BlendAnimationNode() {}

	/// @brief アニメーションを追加する
	/// @param _animData アニメーションデータ
	void SetAnimationData(const std::string& _animName) override;

	/// @brief ブレンド値とするアニメーションパラメータ名を取得
	/// @param _axisIdx 軸番号(0はX、1はY)
	/// @return アニメーションパラメータ名
	const std::string& GetTargetParamName(u_int _axisIdx) const;

	/// @brief ブレンド値を移動する時間を取得
	/// @param _axisIdx 軸番号(0はX、1はY)
	/// @return 移動時間
	float GetBlendMoveTime(u_int _axisIdx) const;

	/// @brief ブレンド値を移動するイージングを取得
	/// @param _axisIdx 軸番号(0はX、1はY)
	/// @return 移動イージング
	HashiTaku::EaseKind GetBlendMoveEasing(u_int _axisIdx) const;

	void CalcRootMotionSpeed(const std::vector<BlendingData>& _blendData, DirectX::SimpleMath::Vector3& _outPos) const;

	/// @brief 軸の数を取得（最大2）
	/// @return 軸の数
	u_int GetAxisCnt() const;

	/// @brief ルートモーションの座標を取得
	/// @param _ratio 割合
	/// @param _blendDatas ブレンドデータ 
	/// @param _outPos 結果
	void GetRootMotionPos(float _ratio, const std::vector<BlendingData>& _blendDatas, DirectX::SimpleMath::Vector3& _outPos) const;

	/// @brief ブレンドするかどうか取得
	/// @return ブレンドするか？
	bool GetIsUseBlend() const;

	/// @brief X軸のみのときのブレンドペア探索
	/// @param _blendValue ブレンド値
	/// @param _outData ブレンドデータリスト
	void FindBlendingDataSingleAxis(float _blendValue, std::vector<BlendingData>& _outData) const;

	/// @brief Y軸のみのときのブレンドペア探索
	/// @param _blendValues ブレンド値
	/// @param _outData ブレンドデータリスト
	void FindBlendPairTwoAxis(DirectX::SimpleMath::Vector2 _blendValues, std::vector<BlendingData>& _outData) const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief ブレンド値とポイントのブレンド値との距離を昇順から並べる
	/// @param _a A
	/// @param _b B
	/// @return 並び変える
	static bool CompareBlendDistance(const PointToDistance& _a, const PointToDistance& _b);

	/// @brief ブレンド値えお昇順から並べる
	/// @param _a A
	/// @param _b B
	/// @return 並び変える
	static bool CompareBlendvalue(const AnimPointData& _a, const AnimPointData& _b);

	/// @brief ブレンド値でソートする
	void SortBlendPoint();

	/// @brief 軸を増やす
	void AddAxisParam();

	/// @brief 軸を減らす
	void RemoveAxisParam();

	/// @brief アニメーションポイントの数取得
	/// @return アニメーションポイント数
	u_int GetAnimPointCnt() const;

	void ImGuiDebug() override;

	/// @brief ブレンド内のアニメーション
	void ImGuiAnimationInBlend();

	// 軸ごとの軸パラメータを調整
	void ImGuiAxisParam();

	// ブレンドのターゲット変数を変更する
	void ImGuiChangeTargetParam();

	// ブレンドデータをセーブする
	nlohmann::json SaveAxisParameter(const AxisParameter& _axisParam);

	// 軸パラメータをセーブする
	nlohmann::json SaveBlendPoint(const AnimPointData& _blendPoint);

	// 軸パラメータをロードする
	void LoadAxisParameter(const nlohmann::json& _blendData);

	// ブレンドデータをロードする
	void LoadBlendPoint(const nlohmann::json& _blendData);
};

