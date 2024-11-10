#pragma once
#include "CP_Enemy.h"
#include "BossActionController.h"


/// @brief ボスのコンポーネント
class CP_Boss : public CP_Enemy
{
	std::unique_ptr<BossActionController> pActionController;

	/// @brief アニメーション
	CP_Animation* pAnimation;
	
	float hitStopBeforeAnimSpeed;
public:
	CP_Boss();
	~CP_Boss() {}

	void Init() override;

	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	void Awake() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	/// @brief 更新できるか？
	/// @return 更新できるか？
	bool GetCanUpdate();

	/// @brief ヒットストップ開始処理
	void OnHitStopBegin() override;

	/// @brief ヒットストップ終了処理
	void OnHitStopEnd() override;

	void ImGuiDebug() override;
};

