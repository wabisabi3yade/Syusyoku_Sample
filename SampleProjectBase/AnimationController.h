#pragma once
#include "Asset_Base.h"
#include"AnimationNode_Base.h"
#include "AnimationParameters.h"
#include "AnimTransitionArrow.h"
#include "AnimationNotifyFactory.h"
#include "AnimationNotify_Base.h"

// クロスフェード・慣性補間
#include "CrossFadeAnimation.h"
#include "InertInterpAnimation.h"
// vector配列で使用するためinclude
#include "Bone.h"

namespace HashiTaku
{
	// ボーン
	class BoneList;

	using AnimNotifyList = std::list<std::unique_ptr<AnimationNotify_Base>>;
	using TransArrowList = std::list<std::unique_ptr<AnimTransitionArrow>>;

	/// @brief アニメーションノード関連の情報
	struct AnimNodeInfo
	{
		std::unique_ptr<AnimationNode_Base> pAnimNode;	// アニメーションノード
		TransArrowList transitionArrows; // 遷移元となっている矢印
		std::string groupArrowsName;	// 属している遷移条件グループ名
		AnimNotifyList notifyList; // 通知イベント
	};

	/// @brief アニメーション遷移を管理するクラス
	class AnimationController : public Asset_Base, public IImGuiUser
	{
	private:
		/// @brief アニメーションノード関連の情報リスト
		std::list<std::unique_ptr<AnimNodeInfo>> animNodeInfos;

		/// @brief パラメータリスト
		std::unique_ptr<AnimationParameters> pAnimParameters;

		/// @brief 通知イベント作成クラス
		std::unique_ptr<AnimationNotifyFactory> pNotifyFactory;

		/// @brief 遷移条件をグループにしたもの(キャンセルのアニメーションやらをひとまとめにできる)
		std::unordered_map<std::string, TransArrowList> groupArrows;

		/// @brief 最初に始めるノード情報
		AnimNodeInfo* pDefaultNodeInfo;

		/// @brief	対応しているボーン配列
		BoneList* pAssetBoneList;

		/// @brief 再生速度
		float playSpeed;
	public:
		/// @brief コンストラクタ
		/// @param _setType コントローラーの種類
		AnimationController();
		~AnimationController() {}

		/// @brief アニメーション遷移の矢印を作成する
		/// @param _fromNodeName 遷移元アニメーション
		/// @param _toNodeName 遷移先アニメーション
		/// @return 作成した矢印
		AnimTransitionArrow* CreateTransitionArrow(const std::string& _fromNodeName,
			const std::string& _toNodeName);

		/// @brief アニメーション遷移の矢印を作成する
		/// @param _toNodeName 遷移先アニメーション
		/// @return 作成した矢印
		std::unique_ptr<AnimTransitionArrow> CreateTransitionArrow(const std::string& _toNodeName);

		/// @brief アニメーションを除外
		/// @param _animName アニメーションの名前
		void RemoveNode(const std::string& _animName);

		/// @brief アニメーションの進める速度を取得
		/// @return 進める速度
		float GetPlaySpeed() const;

		/// @brief デフォルトノード情報を取得
		/// @return デフォルトノード情報
		AnimNodeInfo* GetDefaultNode() const;

		// 名前からノード情報を取得 
		AnimNodeInfo* GetNodeInfo(const std::string& _name) const;

		// ノードからノード情報を取得する
		const AnimNodeInfo* GetNodeInfo(const AnimationNode_Base& _node) const;

		/// @brief ノードの数を取得
		/// @return ノード数
		void GetNodeArray(std::list<const AnimationNode_Base*>& _animNodeArray) const;

		/// @brief アニメーションのパラメータを取得
		/// @return アニメーションのパラメータ
		const AnimationParameters& GetAnimationParameters() const;

		/// @brief グループの遷移矢印リストを取得する
		/// @param _groupName グループ名
		/// @return グループの遷移矢印リスト(nullptrならグループ名ヒットせず)
		const TransArrowList* GetGroupArrows(const std::string& _groupName) const;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		/// @brief 名前のノードを既にあるか確認する
		/// @param _nodeName 確認する名前
		/// @return ノードを持っているか？
		bool IsHaveNode(const std::string& _nodeName);

		/// @brief ノード名を重複しないような名前に変換
		/// @param _nodename ノード名
		void NotDuplicateNodeName(std::string& _nodename);

		/// @brief ノードタイプから新しくノードを作成する
		/// @param _nodeType ノードの種類
		/// @param _nodeName ノード名
		/// @return 作成したノード情報
		AnimNodeInfo* CreateNodeInfoByType(AnimationNode_Base::NodeType _nodeType, const std::string& _nodeName = "Default");

		/// @brief 遷移条件のグループを作成する
		/// @param _groupName グループ名
		/// @return 作成したグループ矢印(nullptrなら作成失敗)
		TransArrowList* CreateGroupArrows(const std::string& _groupName);

		/// @brief グループに矢印を追加
		/// @param _groupName 追加するグループ名
		/// @param _addArrow 追加する矢印
		void AddArrowToGroup(const std::string& _groupName,
			std::unique_ptr<AnimTransitionArrow> _addArrow);

		/// @brief グループ遷移条件の名前一覧を取得
		/// @param _outList リスト
		void GetGroupNamelist(std::vector<const std::string*>& _outList);

		/// @brief 遷移矢印の優先順位でソート
		static bool SortArrowPriority(const std::unique_ptr<AnimTransitionArrow>& _a1, const std::unique_ptr<AnimTransitionArrow>& _a2);

		// ImGuiでノード関係
		void ImGuiNode(const std::vector<std::string>& _nodeNames);

		// ImGuiで遷移矢印
		void ImGuiTransArrow(AnimNodeInfo& _nodeInfo, const std::vector<std::string>& _nodeNames);

		// ImGuifでボーンリストをセット
		void ImGuiSetBoneList();

		// ImGuiでデフォルトノード設定
		void ImGuiSetDefaultNode(const std::vector<std::string>& _nodeNames);

		// ImGuiでノード作成
		void ImGuiCreateNode();

		// ImGuiで通知イベントを作成
		// 引数：追加先のノード情報
		void ImGuiAnimNotify(AnimNodeInfo& _nodeInfo);

		// ImGuiでグループの遷移条件を編集
		void ImGuiGroupArrows(const std::vector<std::string>& _nodeNames);

		// ノード情報をセーブする
		nlohmann::json SaveNodeInfo(AnimNodeInfo& _nodeInfo);

		// グループ遷移条件をセーブする
		nlohmann::json SaveGroupArrows();

		// ノード情報をロードする
		void LoadNodeInfo(const nlohmann::json& _nodeInfoData);

		// 通知イベントをロードする
		void LoadNotify(const nlohmann::json& _nodeInfoData);

		// 遷移矢印をロードする
		void LoadTransArrow(const nlohmann::json& _nodeInfoData);

		// グループ矢印をロードする
		void LoadGroupArrow(const nlohmann::json& _groupArrowData);
	protected:
		void ImGuiDebug() override;
	};
}