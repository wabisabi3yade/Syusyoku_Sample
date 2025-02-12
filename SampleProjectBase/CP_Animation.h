#pragma once
#include "Component.h"
#include "IBoneSupplier.h"
#include "IAnimationObserver.h"
#include "AnimControllPlayer.h"

namespace HashiTaku
{
	// モデル
	class TreeNode;
	class CP_MeshRenderer;

	/// @brief アニメーションコンポーネント
	class CP_Animation : public Component, public IBoneBufferSupplier
	{
		/// @brief ボーンのコンビネーション行列のバッファー
		BoneCombMtricies boneCombBuffer;

		/// @brief ルートボーンに対するオフセット行列
		DXSimp::Matrix rootOffsetMtx;

		/// @brief スケルタルメッシュ
		SkeletalMesh* pSkeletalMesh;

		/// @brief 実際に動かすボーン
		std::unique_ptr<BoneList> pMoveBoneList;

		/// @brief アニメーションコントローラー
		AnimationController* pAnimController;

		/// @brief アニメーションコントローラ再生機能
		std::unique_ptr<AnimControllPlayer> pAnimConPlayer;

		/// @brief ボーンの数
		u_int boneCnt;
	public:
		CP_Animation();
		virtual ~CP_Animation() {}

		/// @brief 初期化
		void Init() override;

		//void OnDestroy() override;

		/// @brief アニメーション変更オブザーバーを追加
		/// @param _observer オブザーバー
		void AddChangeAnimObserver(ChangeAnimObserver& _observer);

		/// @brief アニメーション変更オブザーバーを削除
		/// @param _observer オブザーバー
		void RemoveChangeAnimObserver(ChangeAnimObserver& _observer);

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

		// アニメーションコントローラーをセット
		void SetAnimationController(AnimationController& _controller);

		/// @brief コントローラープレイヤーの再生速度をセットする
		/// @param _setSpeed スピードをセット
		void SetControllerPlaySpeed(float _setSpeed);

		/// @brief 現在のノード再生速度をセットする
		/// @param _setSpeed スピードをセット
		void SetCurNodePlayerSpeed(float _setSpeed);

		/// @brief 現在再生しているアニメーションの再生割合をセット
		/// @param _playRatio 再生割合
		void SetPlayRatio(float _playRatio);

		/// @brief 現在再生しているアニメーションのフレームをセット
		/// @param _playFrame 再生フレーム
		void SetPlayFrame(u_int _playFrame);

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

		/// @brief アニメーションパラメータのポインターを取得する
		/// @tparam T パラメータのフラグ
		/// @param _paramName パラメータ名
		/// @return アニメーションパラメータのポインター
		template<AnimParam::AnimParamConcept T>
		const T* GetParameterPointer(const std::string& _paramName) const;

		/// @brief 現在再生しているアニメーションのルートモーションの座標移動速度を渡す
		/// @return 座標移動速度(s)
		const DXSimp::Vector3& GetMotionPosSpeedPerSec() const;

		/// @brief 現在のアニメーションのルートモーション座標を取得
		/// @param 取得する割合
		DXSimp::Vector3 GetCurAnimRMPos(float _ratio);

		/// @brief プレイヤー再生速度を取得する
		/// @return 現在の再生速度
		float GetControllerPlaySpeed() const;

		/// @brief プレイヤー再生割合を取得する
		/// @return 現在の再生割合
		float GetCurrentPlayRatio() const;

		/// @brief プレイヤー再生フレームを取得する
		/// @return 現在の再生フレーム
		u_int GetCurrentPlayFrame() const;

		/// @brief 実際に動かすボーンリストを取得する
		/// @return ボーンリスト
		BoneList* GetMoveBoneList();

		/// @brief ボーン数を取得
		/// @return ボーン数
		u_int GetBoneCnt() const;

		/// @brief ボーン行列のバッファを取得する
		/// @return ボーン行列のバッファ(転置済み)
		BoneCombMtricies* GetBoneBuffer() override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void Awake() override;
		void Update() override;
		void Draw() override;

		/// @brief アニメーションコントローラーの準備
		void SetupAnimCon();

		/// @brief Rendererからボーンをコピーし、動かすボーンを作成 
		void CopyBoneList();

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
		void UpdateNodeHierarchy(const TreeNode& _treeNode, const DXSimp::Matrix& _parentMtx);

		/// @brief ボーンのコンビネーション行列を更新
		void UpdateAnimationMtx();

		/// @brief シェーダーのバッファを更新する
		void UpdateBoneBuffer();

		void ImGuiDebug() override;
	};

	template<AnimParam::AnimParamConcept T>
	inline const T* CP_Animation::GetParameterPointer(const std::string& _paramName) const
	{
		if (!pAnimConPlayer) return nullptr;

		// パラメータから取得する
		auto pValue = pAnimConPlayer->GetCopyAnimParameters().GetValueAddress(_paramName);
		return std::get_if<T>(pValue);
	}
}