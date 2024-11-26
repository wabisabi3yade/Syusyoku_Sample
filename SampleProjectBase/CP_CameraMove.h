#pragma once
#include "Component.h"
#include "CameraMoveController.h"

class CP_Camera;

/// @brief カメラ移動クラス
class CP_CameraMove : public Component, public IObjectTargeter
{
	/// @brief カメラの動きを統制するコントローラー
	std::unique_ptr<CameraMoveController> pMoveController;

	/// @brief 追従先のオブジェクト名
	std::string followObjName;

	/// @brief 追従対象
	const Transform* pFollowTransform;

	/// @brief ターゲットする先のオブジェクト
	ITargetAccepter* pTargetObject;

	/// @brief カメラ
	CP_Camera* pCamera;
public:
	CP_CameraMove();
	~CP_CameraMove();
	
	void Init() override;

	/// @brief 対象とするゲームオブジェクトをセット
	/// @param _targetTransform 対象のオブジェクト(nullptrで外す)
	void SetFollowTransform(const Transform* _targetTransform);

	/// @brief ターゲットオブジェクトを取得する
	/// @param _targetObject ターゲットオブジェクト
	void SetTargetObject(ITargetAccepter& _targetObject) override;

	/// @brief 解除するときの更新処理
	/// @param _removeObj 死んだターゲットオブジェクト
	void RemoveNotify(const ITargetAccepter& _removeObj) override;

	virtual void ImGuiDebug() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void Start() override;
	void LateUpdate() override;

	/// @brief 更新できるか確認する
	/// @return 更新できるか？
	bool IsCanUpdate();

	/// @brief 追従先のオブジェクト名を探す
	void FindFollowObject();

	// ターゲットオブジェクトをImGuiで
	void ImGuiSetTarget();
};

