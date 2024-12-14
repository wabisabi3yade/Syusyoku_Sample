#pragma once
#include "CP_Button.h"
#include "CP_UIRenderer.h"

/// @brief ボタンをグループ化して、管理する
class CP_ButtonGroup : public Component
{
	/// @brief セレクトイメージの座標先
	std::vector<DirectX::SimpleMath::Vector3> selectTargetPos;

	/// @brief デフォルトのボタンのID
	u_int defaultSelectButtonId;
protected:
	/// @brief ボタンのグループ
	std::vector<CP_Button*> buttonGroup;

	/// @brief ボタンのコンポーネントを持つオブジェクト
	std::vector<std::string> buttonObjNames;

	/// @brief セレクトしてる時の画像
	CP_UIRenderer* pSelectBackImage;

	/// @brief セレクトしてる時の画像オブジェクト名
	std::string backImageObjName;

	/// @brief 現在選んでいるボタンのID
	int curSelectButtonId;

	/// @brief ボタンの数
	int maxButtonCnt;

	/// @brief 縦入力で移動するボタンの数
	int vertMoveSpeed;

	/// @brief 横入力で移動するボタンの数
	int horiMoveSpeed;

	/// @brief 移動することができるか
	bool canInput;
public:
	CP_ButtonGroup();
	virtual ~CP_ButtonGroup() {}

	/// @brief 移動できるかセットする
	/// @param _canMove 移動できるかフラグ
	void SetCanMove(bool _canMove);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
protected:
	void Start() override;

	void Update() override;

	/// @brief 入力で選択ボタンを移動する
	void MoveButton();

	/// @brief ボタンを押す
	void DecideButton();

	/// @brief セレクトの背景イメージを選択中のボタンへ移動
	void MoveSelectBackImage();

	/// @brief 最大数をセット
	/// @param _max 最大数
	void SetMaxButtonCnt(int _max);

	void ImGuiDebug() override;
};

