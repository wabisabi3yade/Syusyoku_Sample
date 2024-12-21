#pragma once
#include "AnimationData.h"
#include "AnimationCurve.h"

namespace HashiTaku
{
	class BoneList;

	/// @brief アニメーションノードに基底クラス
	class AnimationNode_Base : public IImGuiUser, public ISaveLoad
	{
	public:
		// ノードの種類
		enum class NodeType
		{
			Single,	// 一つだけのアニメーション
			Blend,	// ブレンドされているアニメーション
			Layerd,	// 部位ごと
		};

#ifdef EDIT
		/// @brief 列挙型を文字列に変換
		static std::vector<std::string> edit_nodeTypeStrings;
#endif // EDIT
	private:
		/// @brief ノード名
		std::string nodeName;

		/// @brief アニメーションカーブ
		std::unique_ptr<AnimationCurve> pCurveSpeed;

		/// @brief ノードの種類
		NodeType nodeType;

		/// @brief ノードの再生速度倍率(1.0は等速)
		float playNodeSpeedTimes;

		/// @brief アニメーションの時間
		float animationTime;

		/// @brief ループ再生するか？
		bool isLoop;

		/// @brief アニメーション終了しているか？
		bool isFinish;

		/// @brief 移動座標のルートモーションを適用するか(XZ座標)
		bool isRootMotionPosXZ;

		/// @brief 移動座標のルートモーションを適用するか(Y座標)
		bool isRootMotionPosY;

	public:
		AnimationNode_Base(const std::string& _nodeName,
			NodeType _type);
		virtual ~AnimationNode_Base() {}

		// ノード名をセット
		void SetNodeName(const std::string& _nodeName);

		// ループするかセット
		void SetIsLoop(bool _isLoop);

		/// @brief 終了フラグをセットする
		void SetFinish();

		// ノード名取得
		const std::string& GetNodeName() const;

		// ノードの種類を取得
		NodeType GetNodeType() const;
		// 再生時間を取得
		float GetAnimationTime() const;

		/// @brief 全体キー数を求める
		/// @return 全体のキー数
		virtual u_int GetAllKeyFrame() const;

		/// @brief ループ再生フラグを取得
		/// @return ループ再生フラグ
		bool GetIsLoop() const;

		// アニメーション終了フラグを取得する
		bool GetIsFinish() const;

		/// @brief XZ軸をルートモーションで移動するか？
		/// @return 移動するか？
		bool GetIsRootMotionXZ() const;

		/// @brief Y軸をルートモーションで移動するか？
		/// @return 移動するか？
		bool GetIsRootMotionY() const;

		/// @brief 再生速度倍率を求める
		/// @return 再生速度倍率
		float GetPlaySpeedTimes() const;

		/// @brief アニメーションカーブの値を取得する
		/// @param _ratio 割合(0.0～1.0)
		/// @return アニメーションカーブの値(基本的に0.0～1.0)
		float GetCurveValue(float _ratio) const;

		json Save() override;
		void Load(const json& _data) override;
	private:
		// ノードのパラメーター設定
		void ImGuiSetParameter();
	protected:

		// 再生時間をセットする
		void SetAnimationTime(float _time);

		void ImGuiDebug() override;
	};
}