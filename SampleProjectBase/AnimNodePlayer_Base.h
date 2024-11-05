#pragma once
#include "SkeletalMesh.h"

#include "AnimationNode_Base.h"
#include "AnimationNotify_Base.h"

class AnimationParameters;

/// @brief ノード再生の基底クラス
class AnimNodePlayer_Base : public HashiTaku::IImGuiUser
{
	/// @brief 現在の再生割合
	float curPlayRatio;

	/// @brief 1フレーム前のアニメーション割合
	float lastAnimationRatio;

	/// @brief アニメーション割合(アニメーションカーブを反映させる)
	float curAnimationRatio;

	/// @brief ノードの再生速度
	float playerSpeedTimes;

	/// @brief 計算した全てを考慮した再生速度
	float allPlaySpeed;

	/// @brief 再生フラグ
	bool isPlay;

	/// @brief ループしたタイミングか？
	bool isJustLoop;

	/// @brief アニメーションコントローラ内の通知イベントからコピーしてきた通知イベントリスト
	std::list<std::unique_ptr<AnimationNotify_Base>> copyNotifys;
protected:
	/// @brief ルートモーションによる座標移動速度
	DirectX::SimpleMath::Vector3 rootMotionPosSpeedPerSec;

	/// @brief 前回のルートモーション座標値
	DirectX::SimpleMath::Vector3 p_RootMotionPos;

	/// @brief 前回のルートモーション回転量
	DirectX::SimpleMath::Quaternion p_RootMotionRot;

	/// @brief 動かすボーン
	BoneList* pBoneList;

	/// @brief 再生するアニメーションノード
	const AnimationNode_Base* pPlayAnimNode;

	/// @brief ルートモーション時に移動するオブジェクトのトランスフォーム
	Transform* pObjectTransform;
public:
	/// @brief コンストラクタ
	/// @param _playNode 再生ノード
	/// @param _boneList ボーン配列
	/// @param _transform トランスフォーム
	AnimNodePlayer_Base(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	virtual ~AnimNodePlayer_Base(){}

	/// @brief アニメーションコントローラーから通知イベントをコピーする
	/// @param _notifyList コピー元の通知イベントリスト
	/// @param _animationParameters アニメーションパラメータ
	void CopyNotifys(const std::list<std::unique_ptr<AnimationNotify_Base>>& _notifyList, AnimationParameters& _animationParameters);

	/// @brief 更新処理呼び出し
	/// @param _controllerPlaySpeed コントローラー自体の再生速度
	void UpdateCall(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed);

	/// @brief トランスフォームにルートモーションを反映する
	/// @return このフレームの移動量
	DirectX::SimpleMath::Vector3 CalcRootMotionToTransform();

	/// @brief トランスフォームにこのフレームのルートモーションを移動する
	/// @param _rootMovement ルートモーションの移動量
	void ApplyRootMotion(const DirectX::SimpleMath::Vector3& _rootMovement);

	void OnInterpolateUpdate(std::vector<BoneTransform>& _outTransforms, float _controllerPlaySpeed);

	/// @brief 再生割合を進める
	/// @param _controllerPlaySpeed コントローラー自体の再生速度
	void ProgressPlayRatio(float _controllerPlaySpeed);

	/// @brief 変更前終了処理
	void OnTerminal();

	/// @brief 現在の再生割合をセット
	/// @param 再生割合
	void SetCurPlayRatio(float _playRatio);

	/// @brief 再生速度倍率をセット
	/// @param 再生速度倍率
	void SetPlaySpeedTimes(float _playSpeed);

	/// @brief 現在の再生割合を取得する
	/// @return 現在の再生割合
	float GetCurPlayRatio() const;

	/// @brief 1フレーム前の再生割合を取得
	/// @return 1フレーム前の再生割合
	float GetLastAnimationRatio() const;

	/// @brief アニメーション割合を取得
	/// @return アニメーション割合
	float GetAnimationRatio() const;

	/// @brief ノード再生速度を取得
	/// @return ノード再生速度
	float GetNodePlaySpeed() const;

	/// @brief ルートモーションの前回からの差分移動値
	/// @param _outPos 結果
	void GetDeltaRootPos(DirectX::SimpleMath::Vector3& _outPos) const;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @param _isLoadScaling ロード時のスケールを反映するか
	/// @return 現在の割合のルートモーション座標
	void GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos, bool _isLoadScaling) const;

	/// @brief 再生しているノード名を取得
	/// @return ノード名
	const std::string& GetNodeName() const;

	const DirectX::SimpleMath::Vector3& GetRootMotionSpeed() const;
private:
	/// @brief ループ再生できるか確認
	/// @return ループできるか？
	bool IsCanLoop();

	/// @brief 再生がループした時の処理
	void OnPlayLoop();

	/// @brief 通知イベントを更新
	void NotifyUpdate();
protected:
	/// @brief 各ノードプレイヤーの更新処理
	virtual void Update(std::vector<BoneTransform>& _outTransforms) = 0;

	/// @brief ルートモーションの座標移動速度を計算する
	/// @param _controllerSpeed コントローラー速度
	/// @return 座標移動速度
	virtual void CalcRootMotionPosSpeed(float _controllerSpeed) = 0;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @param _isLoadScaling ロード時のスケールを反映するか
	/// @return 現在の割合のルートモーション座標
	virtual DirectX::SimpleMath::Vector3 GetRootMotionPos(float _ratio, bool _isLoadScaling = true) const = 0;

	/// @brief ルートモーションを取得する（内部で必要な計算を行う）
	/// @param 現在の再生割合
	/// @param _isLoadScaling ロード時のスケールを反映するか
	/// @return 現在の割合のルートモーション回転量
	virtual DirectX::SimpleMath::Quaternion GetRootMotionRot(float _ratio, bool _isLoadScaling = true) const = 0;

	/// @brief ルートモーションをロード時のスケール・回転量を反映
	/// @param _rootMotionPos 反映させたいルートモーション
	void ApplyLoadTransform(DirectX::SimpleMath::Vector3& _rootMotionPos) const;

	void ImGuiSetting() override;
};

