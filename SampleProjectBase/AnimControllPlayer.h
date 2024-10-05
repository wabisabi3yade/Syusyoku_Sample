#pragma once
#include "AnimationController.h"
#include "AnimNodePlayer_Base.h"

/// @brief AnimationControllerを再生するクラス
class AnimControllPlayer : public HashiTaku::IImGuiUser
{
	enum class PlayState
	{
		PlayNode,
		Transition,
	};

	/// @brief 再生するアニメーションコントローラー
	const AnimationController* pAnimController;

	/// @brief 現在再生しているノード情報
	const AnimationController::AnimNodeInfo* pNodeInfo;

	/// @brief コントローラ内のパラメータを複製したオブジェクトごとのパラメータリスト
	std::unique_ptr<AnimationParameters> pCopyAnimParameters;

	/// @brief 現在のノード再生機能
	std::unique_ptr<AnimNodePlayer_Base> pCurNodePlayer;

	/// @brief ひとつ前のノード再生機能
	std::unique_ptr<AnimNodePlayer_Base> pPrevNodePlayer;

	/// @brief 動かす対象のボーン
	BoneList* pBoneList;

	/// @brief ゲームオブジェクトのトランスフォーム
	Transform* pObjectTransform;

	/// @brief 再生速度
	float playSpeed;

	/// @brief プレイヤーの状態
	PlayState playState;
public:
	AnimControllPlayer(const AnimationController& _animController, BoneList& _boneList, Transform& _transform);
	~AnimControllPlayer() {}

	/// @brief 更新処理
	void Update();

	/// @brief ノード情報を切り替える
	/// @param _pChangeNodeInfo 変更するノード情報
	void ChangeNodeInfo(const AnimationController::AnimNodeInfo& _changeNodeInfo);

	/// @brief コピーされた側のアニメーションパラメータ
	/// @return アニメーションパラメータの参照
	AnimationParameters& GetCopyAnimParameters();

private:
	/// @brief 再生前初期化
	void PlayInit();

	/// @brief ノードを再生する
	void NodePlay();

	void ImGuiSetting();
};

