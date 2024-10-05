#pragma once
#include "Asset_Base.h"
#include"AnimationNode_Base.h"
#include "AnimConntrollerType.h"
#include "AnimationParameters.h"
#include "AnimTransitionArrow.h"

// クロスフェード・慣性補間
#include "CrossFadeAnimation.h"
#include "InertInterpAnimation.h"
// vector配列で使用するためinclude
#include "Bone.h"

// ボーン
class BoneList;

/// @brief アニメーション遷移を管理するクラス
class AnimationController : public Asset_Base, public HashiTaku::IImGuiUser
{
public:
	/// @brief アニメーションノード関連の情報
	struct AnimNodeInfo
	{
		std::unique_ptr<AnimationNode_Base> pAnimNode;	// アニメーションノード
		std::list<std::unique_ptr<AnimTransitionArrow>> pTransArrows; // 遷移元となっている矢印
	};

private:
	/// @brief アニメーションノード関連の情報リスト
	std::list<std::unique_ptr<AnimNodeInfo>> animNodeInfos;

	/// @brief パラメータリスト
	std::unique_ptr<AnimationParameters> pAnimParameters;

	/// @brief 前のアニメーション
	AnimationNode_Base* pPrevAnimNode;

	/// @brief 最初に始めるノード情報
	AnimNodeInfo* pDefaultNodeInfo;

	/// @brief 動かすボーン配列
	BoneList* pBoneList;

	/// @brief 今遷移で使用している矢印(nullptrなら遷移していない)
	const AnimTransitionArrow* pCurTransArrow;

	/// @brief クロスフェード
	std::unique_ptr<CrossFadeAnimation> pCrossFadeInterp;

	/// @brief 慣性補間
	std::unique_ptr<InertInterpAnimation> pInertInterp;

	/// @brief 再生速度
	float playSpeed;

	/// @brief 再生するか
	bool isPlay;

	/// @brief アニメーション遷移中か？
	bool isTransitioning;

protected:
	/// @brief 現在のノード情報
	AnimNodeInfo* pCurrentNodeInfo;

public:
	/// @brief コンストラクタ
	/// @param _setType コントローラーの種類
	AnimationController();
	~AnimationController() {}
	AnimationController(const AnimationController& _other);
	
	AnimationController& operator=(const AnimationController& _other);

	/// @brief 開始処理
	void Begin(BoneList& _boneList);

	/// @brief ボーンのアニメーションを更新する
	/// @param _boneList ボーンリスト
	/// @param _playingTime 再生時間
	void Update(BoneList& _boneList);

	/// @brief アニメーション遷移する(デバッグ用）
	/// @param _animName アニメーション名
	virtual void ChangeAnimation(const std::string& _animName);

	/// @brief アニメーション遷移する
	/// @param _transitionArrow 今回使用する遷移矢印
	virtual void ChangeAnimation(const AnimTransitionArrow& _transitionArrow);

	/// @brief シングルノードを作成
	/// @param _nodeName ノード名
	/// @param _animName セットするアニメーション名
	void CreateSingleNode(const std::string& _nodeName, const std::string& _animName);

	/// @brief アニメーション遷移の矢印を作成する
	/// @param _fromNodeName 遷移元アニメーション
	/// @param _toNodeName 遷移先アニメーション
	/// @return 作成した矢印
	AnimTransitionArrow* CreateTransitionArrow(const std::string& _fromNodeName, const std::string& _toNodeName);

	/// @brief アニメーションを除外
	/// @param _animName アニメーションの名前
	void RemoveNode(const std::string& _animName);

	/// @brief アニメーションがセットされているか返す
	/// @return アニメーションがセットされているか？
	bool IsSetAnimation();

	/// @brief ブレンド割合をセット
	/// @param _ratio 割合
	void SetBlendRatio(float _ratio);

	// 動かすボーン配列をセット
	void SetBoneList(BoneList& _boneList);

	/// @brief 開始時のデフォルトノードをセットする
	/// @param _nodeName ノード名
	void SetDefaultNode(const std::string& _nodeName);

	/// @brief 指定したbool変数に値をセット
	/// @param _paramName パラメーター名
	/// @param _isBool セットする値
	void SetBool(const std::string& _paramName, bool _isBool);

	/// @brief 指定したint変数に値をセット
	/// @param _paramName パラメーター名
	/// @param _intVall セットする値
	void SetInt(const std::string& _paramName, int _intVal);

	/// @brief 指定したfloat変数に値をセット
	/// @param _paramName パラメーター名
	/// @param _floatVal セットする値
	void SetFloat(const std::string& _paramName, float _floatVal);

	/// @brief 指定したトリガー変数に値をセット
	/// @param _paramName パラメーター名
	void SetTrigger(const std::string& _paramName);

	/// @brief 指定したbool変数に値を取得
	/// @param _paramName パラメーター名
	/// @return 取得する値
	bool GetBool(const std::string& _paramName);

	/// @brief 指定したint変数に値を取得
	/// @param _paramName パラメーター名
	/// @return 取得する値
	int GetInt(const std::string& _paramName);

	/// @brief 指定したfloat変数に値を取得
	/// @param _paramName パラメーター名
	/// @return 取得する値
	float GetFloat(const std::string& _paramName);

	/// @brief 指定したトリガー変数に値を取得
	/// @return 取得する値
	bool GetTrigger(const std::string& _paramName);

	// 現在のノードを取得する
	AnimationNode_Base* GetCurrentNode();

	/// @brief デフォルトノード情報を取得
	/// @return デフォルトノード情報
	const AnimationController::AnimNodeInfo* GetDefaultNode() const;
	
	// 名前からノード情報を取得 
	AnimNodeInfo* GetNodeInfo(const std::string& _name);

	// ノードからノード情報を取得する
	AnimNodeInfo* GetNodeInfo(const AnimationNode_Base& _node);

	// 現在の再生割合を取得
	float GetPlayingRatio() const;

	/// @brief ノードの数を取得
	/// @return ノード数
	void GetNodeArray(std::list<const AnimationNode_Base*>& _animNodeArray) const;

	/// @brief アニメーションのパラメータを取得
	/// @return アニメーションのパラメータ
	const AnimationParameters& GetAnimationParameters() const;

	// 再生しているか取得
	bool GetIsPlay() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 再生できる状態か？
	/// @return 再生できるか
	bool IsCanPlay();

	/// @brief アニメーションの更新処理
	void AnimatioUpdate();

	/// @brief ループ再生できるか？
	/// @return 再生できるフラグ
	bool IsCanLoop();

	/// @brief 通常時、アニメーション
	void NormalUpdate();

	/// @brief クロスフェード補間時のアニメーション
	void CrossFadeUpdate();

	/// @brief 慣性補間遷移時のアニメーション
	void InertInterpUpdate();

	/// @brief 慣性補間のキャッシュ更新
	void CacheUpdate();

	/// @brief 遷移するか確認する
	void TranstionCheck();

	/// @brief 共通した遷移開始処理
	void OnTransitionStart();

	/// @brief 名前のノードを既にあるか確認する
	/// @param _nodeName 確認する名前
	/// @return ノードを持っているか？
	bool IsHaveNode(const std::string& _nodeName);

	/// @brief ノード名を重複しないような名前に変換
	/// @param _nodename ノード名
	void NotDuplicateNodeName(std::string& _nodename);

	/// @brief クロスフェード補間を開始
	/// @param _targetAnimRatio 遷移終了時の遷移先のアニメーション割合
	/// @param _transitionTime 遷移時間
	/// @param _easeKind 遷移のイージング
	void CrossFadeStart(float _targetAnimRatio, float _transitionTime, HashiTaku::EaseKind _easeKind);

	/// @brief 慣性補間開始する
	/// @param_targetAnimRatio 遷移先のアニメーション割合
	/// @param _transitionTime 遷移時間
	void InterpTransitionStart(float _targetAnimRatio, float _transitionTime);

	/// @brief 遷移終了した時の処理
	void OnTransitionEnd();

	/// @brief ノードタイプから新しくノードを作成する
	/// @param _nodeType ノードの種類
	/// @param _nodeName ノード名
	/// @return 作成したノード情報
	AnimNodeInfo* CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName = "Default");

	void Copy(const AnimationController& _other);
	void CopyNodes(const AnimationController& _other);

	// ノード関係
	void ImGuiNode(const std::vector<std::string>& _nodeNames);

	// 遷移矢印
	void ImGuiTransArrow(AnimNodeInfo& _nodeInfo, const std::vector<std::string>& _nodeNames);

	// デフォルトノード設定
	void ImGuiSetDefaultNode(const std::vector<std::string>& _nodeNames);

	// 遷移情報表示
	void ImGuiTransition();

	// ノード作成
	void ImGuiCreateNode();

	// ノード情報をセーブする
	nlohmann::json SaveNodeInfo(AnimNodeInfo& _nodeInfo);

	// ノード情報をロードする
	void LoadNodeInfo(const nlohmann::json& _nodeInfoData);

	// 遷移矢印をロードする
	void LoadTransArrow(const nlohmann::json& _nodeInfoData);
protected:
	void ImGuiSetting() override;

	/// @brief アニメーション終了時処理
	virtual void OnAnimationFinish();

	/// @brief アニメーションを変更完了
	void OnChangeAnimComplete();
};
