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
		DXSimp::Matrix worldMatrix;

		/// @brief ローカル空間トランスフォーム行列
		DXSimp::Matrix localMatrix;

		/// @brief トランスフォームを持つオブジェクト
		GameObject* pGameObject;

		/// @brief 親トランスフォーム
		Transform* pParent;

		/// @brief 子トランスフォーム
		std::list<Transform*> childTransforms;

		// 各方向の単位ベクトル
		DXSimp::Vector3 up;	// 上
		DXSimp::Vector3 right;	// 右
		DXSimp::Vector3 forward;	// 前

		// ワールド
		DXSimp::Vector3 position;	// 座標
		DXSimp::Vector3 scale;	// スケール
		DXSimp::Vector3 eularAngles;	// 回転角度
		DXSimp::Quaternion rotation;	// クォータニオン


		// ローカル
		DXSimp::Vector3 localPosition;	// 座標
		DXSimp::Vector3 localScale;	// スケール
		DXSimp::Vector3 localEularAngles;	// 回転角度
		DXSimp::Quaternion localRotation;	// クォータニオン

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
		void LookAt(const DXSimp::Vector3& _worldPos, const DXSimp::Vector3& _upVector = DXSimp::Vector3::Up);

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
		virtual void SetPosition(const DXSimp::Vector3& _pos);
		virtual void SetScale(const DXSimp::Vector3& _scale);
		virtual void SetEularAngles(const DXSimp::Vector3& _eularAngles);
		virtual void SetRotation(const DXSimp::Quaternion& _quaternion);

		// ローカル
		virtual void SetLocalPosition(const DXSimp::Vector3& _position);
		virtual void SetLocalScale(const DXSimp::Vector3& _scale);
		virtual void SetLocalEularAngles(const DXSimp::Vector3& _eularAngles);
		virtual void SetLocalRotation(const DXSimp::Quaternion& _quaternion);

		// ワールド
		const DXSimp::Vector3& GetPosition() const;
		const DXSimp::Vector3& GetScale() const;
		const DXSimp::Vector3& GetEularAngles() const;
		const DXSimp::Quaternion& GetRotation() const;
		const DXSimp::Matrix& GetWorldMatrix() const;

		// ローカル
		const DXSimp::Vector3& GetLocalPosition() const;
		const DXSimp::Vector3& GetLocalScale() const;
		const DXSimp::Vector3& GetLocalEularAngles() const;
		const DXSimp::Quaternion& GetLocalRotation() const;
		const DXSimp::Matrix& GetLocalMatrix() const;

		const DXSimp::Vector3& Right()const { return right; }	// 右ベクトル
		const DXSimp::Vector3& Up()const { return up; }	// 上ベクトル
		const DXSimp::Vector3& Forward()const { return forward; }	// 正面ベクトル

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
		json Save() override;

		/// @brief ロードする
		/// @param  _transformData ロードするデータ 
		void Load(const json& _transformData) override;

		/// @brief 親子関係をロードする
		/// @param  _transformData ロードするデータ 
		void LoadChildTransform(const json& _transformData);

	private:
		/// @brief 自身のワールド行列を更新する
		/// @param _parentWorldMtx 親のワールド行列
		void UpdateWorldMatrix(const DXSimp::Matrix& _parentWorldMtx);

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