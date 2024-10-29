#pragma once
#include "Component.h"
#include "IAnimationObserver.h"
#include "AnimControllPlayer.h"

// モデル
class TreeNode;

constexpr u_int MAX_BONEMTX(400);	// シェーダーの渡すボーン行列の最大数

/// @brief アニメーションコンポーネント
class CP_Animation : public Component
{
	// シェーダーに渡すボーン行列構造体
	struct BoneCombMtricies
	{
		DirectX::SimpleMath::Matrix matrix[MAX_BONEMTX];
	};

	static BoneCombMtricies boneComb;

	/// @brief スケルタルメッシュ
	SkeletalMesh* pSkeletalMesh;

	/// @brief モデルのオフセット行列
	DirectX::SimpleMath::Matrix offsetMtx;

	/// @brief アニメーションコントローラー
	AnimationController* pAnimController;

	/// @brief アニメーションコントローラ再生機能
	std::unique_ptr<AnimControllPlayer> pAnimConPlayer;
public:
	CP_Animation();
	CP_Animation(const CP_Animation& _other);
	virtual ~CP_Animation() {}

	CP_Animation& operator=(const CP_Animation& _other);

	void Init() override {}

	void Awake() override;

	void Update() override;

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

	/// @brief 指定したトリガー変数をtrueにする
	/// @param _paramName パラメーター名
	void SetTrigger(const std::string& _paramName);

	/// @brief 指定したbool変数に値を取得
	/// @param _paramName パラメーター名
	/// @param _isBool 取得する値
	bool GetBool(const std::string& _paramName);

	/// @brief 指定したint変数に値を取得
	/// @param _paramName パラメーター名
	/// @param _intVall 取得する値
	int GetInt(const std::string& _paramName);

	/// @brief 指定したfloat変数に値を取得
	/// @param _paramName パラメーター名
	/// @param _floatVal 取得する値
	float GetFloat(const std::string& _paramName);

	/// @brief Rendererからスケルタルメッシュをセット 
	void SetupSkeletalMesh();

	// アニメーションコントローラーをセット
	void SetAnimationController(AnimationController& _controller);

	/// @brief プレイヤーの再生速度をセットする
	/// @param _setSpeed スピードをセット
	void SetCurPlayerSpeed(float _setSpeed);

	/// @brief アニメーション変更オブザーバーを追加
	/// @param _observer オブザーバー
	void AddChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	/// @brief アニメーション変更オブザーバーを削除
	/// @param _observer オブザーバー
	void RemoveChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	// スケルタルメッシュを取得
	SkeletalMesh& GetSkeletalMesh();

	AnimationController* GetAnimationController();

	/// @brief 現在再生しているアニメーションのルートモーションの座標移動速度を渡す
	/// @return 座標移動速度(s)
	const DirectX::SimpleMath::Vector3& GetMotionPosSpeedPerSec() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief アニメーションコントローラーの準備
	void SetupAnimCon();

	/// @brief 再生できる状態か？
	/// @return 再生できるか
	bool IsCanPlay();

	/// @brief コピーされたパラメータが存在しているか
	/// @return 存在している
	bool IsExistCopyAnimParameter();

	/// @brief ボーンコンビネーション行列を更新
	void UpdateBoneCombMtx();

	/// @brief 子ノードのコンビネーション行列を更新する（再帰関数）
	/// @param _treeNode 更新するノード
	/// @param _parentMtx ワールド変換するための親までの行列
	void UpdateNodeHierarchy(TreeNode& _treeNode, const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief ボーンのコンビネーション行列を更新
	void UpdateAnimationMtx();

	/// @brief シェーダーのバッファを更新する
	void UpdateBoneBuffer();

	void Copy(const CP_Animation& _other);

	void ImGuiSetting() override;
};

