#pragma once
// セーブ・ロード
#include "ISaveLoad.h"

namespace HashiTaku
{
	class GameObject;

	// 座標・回転・スケールをまとめたパラメータ
	class Transform : public ISaveLoad
	{
		/// @brief ワールド空間トランスフォーム行列
		DirectX::SimpleMath::Matrix worldMatrix;

		/// @brief ローカル空間トランスフォーム行列
		DirectX::SimpleMath::Matrix localMatrix;

		/// @brief トランスフォームを持つオブジェクト
		GameObject* pGameObject;

		/// @brief 親トランスフォーム
		Transform* pParent;

		/// @brief 子トランスフォーム
		std::list<Transform*> childTransforms;

		// 各方向の単位ベクトル
		DirectX::SimpleMath::Vector3 up;	// 上
		DirectX::SimpleMath::Vector3 right;	// 右
		DirectX::SimpleMath::Vector3 forward;	// 前

		// ワールド
		DirectX::SimpleMath::Vector3 position;	// 座標
		DirectX::SimpleMath::Vector3 scale;	// スケール
		DirectX::SimpleMath::Vector3 eularAngles;	// 回転角度
		DirectX::SimpleMath::Quaternion rotation;	// クォータニオン


		// ローカル
		DirectX::SimpleMath::Vector3 localPosition;	// 座標
		DirectX::SimpleMath::Vector3 localScale;	// スケール
		DirectX::SimpleMath::Vector3 localEularAngles;	// 回転角度
		DirectX::SimpleMath::Quaternion localRotation;	// クォータニオン

	protected:
		/// @brief 親トランスフォームがあるか？
		bool isHaveParent;

		bool isNull;

	public:
		Transform(GameObject* _pGameObject, bool _isInit = true);
		virtual ~Transform();

		// 方向ベクトルを更新する
		void UpdateVector();

		// その座標にオブジェクトを向ける
		void LookAt(const DirectX::SimpleMath::Vector3& _worldPos, const DirectX::SimpleMath::Vector3& _upVector = DirectX::SimpleMath::Vector3::Up);

		/// @brief 親トランスフォームを解除する
		virtual void RemoveParent();

		/// @brief 子トランスフォームを解除する
		/// @param _removeTransform 解除するトランスフォーム
		virtual void RemoveChild(Transform& _removeTransform);

		/// @brief 親トランスフォームを設定する
		/// @param _parent 親トランスフォームの参照
		virtual void SetParent(Transform& _parent);

		/// @brief 子トランスフォームを設定する
		/// @param _parent 子トランスフォームの参照
		virtual void SetChild(Transform& _child);

		// ワールド
		virtual void SetPosition(const DirectX::SimpleMath::Vector3& _pos);
		virtual void SetScale(const DirectX::SimpleMath::Vector3& _scale);
		virtual void SetEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
		virtual void SetRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

		// ローカル
		virtual void SetLocalPosition(const DirectX::SimpleMath::Vector3& _position);
		virtual void SetLocalScale(const DirectX::SimpleMath::Vector3& _scale);
		virtual void SetLocalEularAngles(const DirectX::SimpleMath::Vector3& _eularAngles);
		virtual void SetLocalRotation(const DirectX::SimpleMath::Quaternion& _quaternion);

		// ワールド
		const DirectX::SimpleMath::Vector3& GetPosition() const;
		const DirectX::SimpleMath::Vector3& GetScale() const;
		const DirectX::SimpleMath::Vector3& GetEularAngles() const;
		const DirectX::SimpleMath::Quaternion& GetRotation() const;
		const DirectX::SimpleMath::Matrix& GetWorldMatrix() const;

		// ローカル
		const DirectX::SimpleMath::Vector3& GetLocalPosition() const;
		const DirectX::SimpleMath::Vector3& GetLocalScale() const;
		const DirectX::SimpleMath::Vector3& GetLocalEularAngles() const;
		const DirectX::SimpleMath::Quaternion& GetLocalRotation() const;
		const DirectX::SimpleMath::Matrix& GetLocalMatrix() const;

		const DirectX::SimpleMath::Vector3& Right()const { return right; }	// 右ベクトル
		const DirectX::SimpleMath::Vector3& Up()const { return up; }	// 上ベクトル
		const DirectX::SimpleMath::Vector3& Forward()const { return forward; }	// 正面ベクトル

		/// @brief ゲームオブジェクトを取得する
		/// @return ゲームオブジェクト
		GameObject& GetGameObject();

		/// @brief ゲームオブジェクト名を取得する
		/// @return オブジェクト名
		const std::string& GetObjectName() const;

		// 親トランスフォームを取得
		Transform* GetParent();

		/// @brief 子トランスフォームを取得
		/// @param _childIdx 子供ID
		/// @return 子トランスフォーム
		Transform* GetChild(u_int _childIdx);

		//　子トランスフォームの数を取得
		u_int GetChildCnt() const;

		/// @brief セーブする
		/// @param _sceneData セーブシーンデータ
		nlohmann::json Save() override;

		/// @brief ロードする
		/// @param  _transformData ロードするデータ 
		void Load(const nlohmann::json& _transformData) override;

		/// @brief 親子関係をロードする
		/// @param  _transformData ロードするデータ 
		void LoadChildTransform(const nlohmann::json& _transformData);

	private:
		/// @brief 自身のワールド行列を更新する
		/// @param _parentWorldMtx 親のワールド行列
		void UpdateWorldMatrix(const DirectX::SimpleMath::Matrix& _parentWorldMtx);

		/// @brief 現在のローカルパラメータからローカル行列を更新
		void UpdateLocalMatrix();

		void UpdateLocalMatrixFromWorld();

		/// @brief 座標変更をオブジェクトに伝える
		void ChangePosNotify();

		/// @brief スケール変更をオブジェクトに伝える
		void ChangeScaleNotify();

		/// @brief 回転変更をオブジェクトに伝える
		void ChangeRotNotify();
	};
}