#pragma once
#include "AnimationController.h"
#include "AnimNodePlayer_Base.h"
#include "AnimTransitionChecker.h"
#include "ChangeAnimObserver.h"

namespace HashiTaku
{
	/// @brief AnimationControllerを再生するクラス
	class AnimControllPlayer : public IImGuiUser
	{
		/// @brief アニメーションの更新ステート
		enum class UpdateState
		{
			PlayNode,	// ノード再生
			Transition,	// 遷移中
		};

		/// @brief 現在のルートモーション座標位置
		DXSimp::Vector3 rootMotionPos;

		/// @brief 再生するアニメーションコントローラー
		const AnimationController* pAnimController;

		/// @brief コントローラ内のパラメータを複製したオブジェクトごとのパラメータリスト
		std::unique_ptr<AnimationParameters> pCopyAnimParameters;

		/// @brief 現在のノード再生機能
		std::unique_ptr<AnimNodePlayer_Base> pCurNodePlayer;

		/// @brief ひとつ前のノード再生機能
		std::unique_ptr<AnimNodePlayer_Base> pPrevNodePlayer;

		/// @brief 遷移条件を満たしているかどうかチェックする
		std::unique_ptr<AnimTransitionChecker> pTransChecker;

		/// @brief クロスフェード
		std::unique_ptr<CrossFadeAnimation> pCrossFadeInterp;

		/// @brief アニメーション変更サブジェクト
		std::unique_ptr<ChangeAnimSubject> pChangeAnimSubject;

		/// @brief 動かす対象のボーン
		BoneList* pAssetBoneList;

		/// @brief ゲームオブジェクトのトランスフォーム
		Transform* pObjectTransform;

		/// @brief 再生速度
		float playSpeed;

		/// @brief 経過時間
		float deltaTime;

		/// @brief アニメーション更新の状態
		UpdateState updateState;

		/// @brief 遷移中の種類
		AnimInterpolateKind curTransitonKind;
	public:
		/// @brief コンストラクタ
		/// @param _animController 対応アニメーションコントローラー
		/// @param _boneList ボーン配列
		/// @param _transform オブジェクトのトランスフォーム
		AnimControllPlayer(const AnimationController& _animController, 
			BoneList& _boneList, 
			Transform& _transform);
		~AnimControllPlayer() {}

		/// @brief 更新処理
		///  @param _deltaTime　経過時間
		void Update(float _deltaTime);

		/// @brief サブジェクトにオブザーバーを登録
		/// @param _observer オブザーバー
		void AddChangeAnimObserver(ChangeAnimObserver& _observer);

		/// @brief サブジェクトにオブザーバーを削除
		/// @param _observer オブザーバー
		void RemoveChangeAnimObserver(ChangeAnimObserver& _observer);

		/// @brief 現在の再生速度をセットする
		/// @param _observer 再生速度
		void SetCurrentPlaySpeed(float _speed);

		/// @brief コピーされた側のアニメーションパラメータ
		/// @return アニメーションパラメータの参照
		AnimationParameters& GetCopyAnimParameters();

		/// @brief 現在再生しているノード再生を取得
		/// @return ノード再生
		AnimNodePlayer_Base& GetCurNodePlayer() const;

		/// @brief ルートボーンのローカル座標を取得
		/// @param _outPos 結果
		void GetCurrentRootPos(DXSimp::Vector3& _outPos) const;

		/// @brief 現在の再生速度を取得
		/// @return 再生速度
		float GetCurrentPlaySpeed() const;

	private:
		/// @brief 再生前初期化
		void PlayInit();

		/// @brief 初期状態をセット
		void SetDefault();

		/// @brief 各状態の更新処理
		void StateUpdate();

		/// @brief ノードを再生する
		void NodePlayUpdate();

		/// @brief 親ノードからの行列を代入していく
		/// @param _animationTransforms 格納するトランスフォームリスト
		/// @param _nextNode 次のノード
		void SetTreeNodeMatrix(std::vector<BoneTransform>& _setTransforms, const TreeNode& _nextNode);

		/// @brief アニメーション遷移更新処理
		void TransitionUpdate();

		/// @brief クロスフェードの更新処理
		void CrossFadeUpdate();

		/// @brief 慣性補間の更新処理
		void InertializationUpdate();

		/// @brief アニメーション遷移できるか確認
		void TransitionCheck();

		/// @brief アニメーション遷移するタイミング処理
		/// @param _changeArrow 遷移する矢印
		void OnChangeAnimation(const AnimTransitionArrow& _changeArrow);

		/// @brief ノード情報を切り替える
		/// @param _pChangeNodeInfo 変更するノード
		void ChangeNodePlayer(const AnimationNode_Base& _changeNode);

		/// @brief クロスフェード遷移の開始処理
		void OnCrossFadeBegin(const AnimTransitionArrow& _changeArrow);

		/// @brief アニメーション変更のサブジェクトを更新する
		void ChangeAnimSubjectUpdate();

		void ImGuiDebug();
	};
}