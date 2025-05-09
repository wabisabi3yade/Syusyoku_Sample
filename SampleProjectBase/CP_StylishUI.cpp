#include "pch.h"
#include "CP_StylishUI.h"
#include "GameObject.h"
#include "CP_UISlider.h"
#include "InSceneSystemManager.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	/// @brief 限界のスタイリッシュポイント
	constexpr float MAX_LIMIT_STYLISHPOINT(5000);

	CP_StylishUI::CP_StylishUI() :
		pStylishRankRenderer(nullptr),
		pStylishGage(nullptr),
		pGageGroupObj(nullptr),
		curStylishRank(StylishRank::SSS),
		curStylishPoint(0),
		decadeBeginTime(7.0f),
		elapsedToDecadeTime(0.0f),
		rankupEase(EaseKind::Linear),
		curRankupDirState(RankupDirectionState::Wait),
		rankupScalingTime(1.0f),
		rankupScalingRatio(2.0f),
		elapsedRankupDirectionTime(0.0f)
	{
	}

	void CP_StylishUI::AddStylishPoint(float _addPoint)
	{
		// 加算
		curStylishPoint += _addPoint;
		curStylishPoint = std::min(curStylishPoint, MAX_LIMIT_STYLISHPOINT);

		// 減少時間リセット
		elapsedToDecadeTime = 0.0f;

		// 次のランクの条件に達したか？
		StylishRank nextRank = GetNextRank(curStylishRank);
		if (curStylishPoint >= GetEnterRankPoint(nextRank))
		{
			// 達したら次のランクへ
			ChangeRank(GetCurrentRankFromPoint());
		}

		// スライダーに反映
		ApplyStylishGage();
	}

	void CP_StylishUI::DecadeStylishPoint(float _decadePoint)
	{
		// 減算
		curStylishPoint -= _decadePoint;
		curStylishPoint = std::max(curStylishPoint, 0.0f);

		// 今のランクのポイントより下回ったか？
		if (curStylishPoint < GetEnterRankPoint(curStylishRank))
		{
			// 前のランクへ
			ChangeRank(GetCurrentRankFromPoint());
		}

		// スライダーに反映
		ApplyStylishGage();
	}

	json CP_StylishUI::Save()
	{
		json data = Component::Save();

		data["decadeBeginTime"] = decadeBeginTime;
		data["gageObjName"] = stylishGageName;
		data["gageGroupObjName"] = gageGroupObjName;
		data["rankParameters"] = SaveRankParameters();
		data["rankupShake"] = rankupShakeParam.Save();
		data["rankupScalingRatio"] = rankupScalingRatio;
		data["rankupEase"] = rankupEase;
		data["rankupScalingTime"] = rankupScalingTime;
		return data;
	}

	json CP_StylishUI::SaveRankParameters()
	{
		json rankParamDataList;

		// 各ランクごとのパラメータをセーブ
		u_int rankCnt = static_cast<u_int>(StylishRank::MaxNum);
		for (u_int r_i = 0; r_i < rankCnt; r_i++)
		{
			json rankParamData;
			RankParameter& rankParam = rankParameters[r_i];

			// パラメータ
			if (rankParam.displayTexture)
			{
				rankParamData["texName"] = rankParam.displayTexture->GetAssetName();
			}
			rankParamData["enterPoint"] = rankParam.requireEnterPoints;
			rankParamData["decadeSpeed"] = rankParam.decadePointSpeed;

			// 追加
			rankParamDataList.push_back(rankParamData);
		}

		return rankParamDataList;
	}

	void CP_StylishUI::Load(const json& _data)
	{
		Component::Load(_data);

		LoadJsonFloat("decadeBeginTime", decadeBeginTime, _data);
		LoadJsonString("gageObjName", stylishGageName, _data);
		LoadJsonString("gageGroupObjName", gageGroupObjName, _data);
		LoadJsonFloat("rankupScalingRatio", rankupScalingRatio, _data);
		LoadJsonEnum<EaseKind>("rankupEase", rankupEase, _data);
		LoadJsonFloat("rankupScalingTime", rankupScalingTime, _data);
		// ランクパラメータ
		if (IsJsonContains(_data, "rankParameters"))
		{
			LoadRankParameters(_data["rankParameters"]);
		}
		if (IsJsonContains(_data, "rankupShake"))
		{
			rankupShakeParam.Load(_data["rankupShake"]);
		}

	}

	void CP_StylishUI::LoadRankParameters(const json& _rankParamDataList)
	{
		// 各ランクごとのパラメータをセーブ
		u_int rankCnt = static_cast<u_int>(StylishRank::MaxNum);
		for (u_int r_i = 0; r_i < rankCnt; r_i++)
		{
			const json& rankParamData = _rankParamDataList[r_i];
			RankParameter& rankParam = rankParameters[r_i];

			// パラメータ
			std::string texName;
			if (LoadJsonString("texName", texName, rankParamData))
			{
				rankParam.displayTexture = AssetGetter::GetAsset<Texture>(texName);
			}
			LoadJsonFloat("enterPoint", rankParam.requireEnterPoints, rankParamData);
			LoadJsonFloat("decadeSpeed", rankParam.decadePointSpeed, rankParamData);
		}
	}

	void CP_StylishUI::Start()
	{
		pRankupShakeVector = std::make_unique<PerlinShake>();

		// ランク上がったときのパラメータ初期か
		InitRankupParam();

		// オブジェクトを取得
		FindRequireObject();

		// ランクなしから始まる
		ChangeRank(StylishRank::None);
	}

	void CP_StylishUI::Update()
	{
		// 活動できるか確認
		if (!GetCanActive()) return;

		// ポイントを減らす
		DecadePointUpdate();

		// 演出の更新処理
		RankupDirectionUpdate();
	}

	void CP_StylishUI::InitRankupParam()
	{
		// オブジェクトの初期座標とスケールを代入
		Transform& transform = GetTransform();
		rankUIBasePos = transform.GetPosition();
		rankUIBaseScale = transform.GetScale();
	}

	void CP_StylishUI::FindRequireObject()
	{
		// 取得
		pStylishRankRenderer = GetGameObject().GetComponent<CP_UIRenderer>();
		if (!pStylishRankRenderer)
			HASHI_DEBUG_LOG("UI描画がありません");

		// ゲージ取得
		SceneObjects& sceneObjs = InSceneSystemManager::GetInstance()->GetSceneObjects();
		GameObject* pObj = sceneObjs.GetSceneObject(stylishGageName);
		if (pObj)
		{
			pStylishGage = pObj->GetComponent<CP_UISlider>();
			if (!pStylishGage)
				HASHI_DEBUG_LOG("UISliderがありません");
		}
		ApplyStylishGage();

		// ゲージグループ
		pGageGroupObj = sceneObjs.GetSceneObject(gageGroupObjName);
		if (!pGageGroupObj)
			HASHI_DEBUG_LOG("ゲージグループがありません");

	}

	void CP_StylishUI::DecadePointUpdate()
	{
		// 減る時間を過ぎたなら
		if (elapsedToDecadeTime > decadeBeginTime)
		{
			// 今フレームで減らすポイントを計算する
			float decadePoint = rankParameters[static_cast<u_int>(curStylishRank)].decadePointSpeed *
				DeltaTime();

			// 減らす
			DecadeStylishPoint(decadePoint);
		}
		else // 下回っているなら
		{
			// 加算する
			elapsedToDecadeTime += DeltaTime();
		}
	}

	void CP_StylishUI::RankupDirectionUpdate()
	{
		switch (curRankupDirState)
		{
		case RankupDirectionState::Scaling:
			RankupScalingUpdate();
			break;

		case RankupDirectionState::Shaking:
			RankupShakeUpdate();
			break;

		default:
			break;
		}

	}

	void CP_StylishUI::RankupScalingUpdate()
	{
		if (!pStylishRankRenderer) return;

		// 時間を経過
		elapsedRankupDirectionTime += DeltaTime();

		// 0徐算防止
		float scaleTime = std::max(rankupScalingTime, Mathf::epsilon);
		// 時間の割合
		float timeRatio = elapsedRankupDirectionTime / scaleTime;
		timeRatio = Easing::EaseValue(timeRatio, rankupEase);

		// スケーリング
		DXSimp::Vector3 beginScale = rankUIBaseScale * rankupScalingRatio;
		beginScale.z = rankUIBaseScale.z;
		DXSimp::Vector3 curScale = DXSimp::Vector3::Lerp(beginScale,
			rankUIBaseScale,
			timeRatio);
		pStylishRankRenderer->GetTransform().SetScale(curScale);

		// α値移動
		float curAlpha = Mathf::Lerp(0.0f, 1.0f, timeRatio);
		pStylishRankRenderer->SetAlpha(curAlpha);

		// 演出時間が過ぎたら次のステップへ
		if (elapsedRankupDirectionTime > rankupScalingTime)
		{
			curRankupDirState = RankupDirectionState::Shaking;

			// シェイク開始
			pRankupShakeVector->BeginShake(rankupShakeParam);
		}
	}

	void CP_StylishUI::RankupShakeUpdate()
	{
		if (!pStylishRankRenderer) return;

		Transform& transform = pStylishRankRenderer->GetTransform();

		// オフセット計算
		pRankupShakeVector->Update(DeltaTime());

		// 座標反映
		transform.SetPosition(rankUIBasePos + pRankupShakeVector->GetShakeOffset());

		// 終わったら演出終了
		if (!pRankupShakeVector->GetIsShaking())
		{
			curRankupDirState = RankupDirectionState::Wait;
		}
	}

	bool CP_StylishUI::GetCanActive() const
	{
		if (!pStylishRankRenderer) return false;
		if (!pStylishGage) return false;

		return true;
	}

	float CP_StylishUI::GetEnterRankPoint(StylishRank _checkRank) const
	{
		return rankParameters[static_cast<u_int>(_checkRank)].requireEnterPoints;
	}

	void CP_StylishUI::ChangeRank(StylishRank _targetRank)
	{
		// 現在のランクと同じなら処理しない
		if (curStylishRank == _targetRank) return;

		// 変更前のランク取得
		StylishRank prevRank = curStylishRank;

		// ランク変更
		curStylishRank = _targetRank;

		if (pStylishRankRenderer)
		{
			// ランクの画像を変える
			u_int curRankId = static_cast<u_int>(curStylishRank);
			pStylishRankRenderer->SetTexture(rankParameters[curRankId].displayTexture);

			// 前回のランクより上がっていたなら
			u_int prevRankId = static_cast<u_int>(prevRank);
			if (curRankId > prevRankId)
			{
				// 演出を始める
				BeginRankUpDirection();
			}
		}

		// ランクなしならゲージを消す
		if (_targetRank == StylishRank::None)
		{
			if (pGageGroupObj)
				pGageGroupObj->SetActive(false);
		}
		else if (prevRank == StylishRank::None) // 前回のランクがなしからなら
		{
			if (pGageGroupObj)
				pGageGroupObj->SetActive(true);	// ゲージグループを表示
		}
	}

	void CP_StylishUI::ApplyStylishGage()
	{
		if (!pStylishGage) return;

		// 次のランクに入るポイントを取得
		float nextEnterPoint = MAX_LIMIT_STYLISHPOINT;

		// 最大ランクでないなら
		if (!GetIsCurMaxRank())
		{
			// 次のランクのポイントを取得する
			StylishRank nextRank = GetNextRank(curStylishRank);
			nextEnterPoint = GetEnterRankPoint(nextRank);
		}

		// 現在のポイントが次までのどの程度の割合なのか求める
		float curEnterPoint = GetEnterRankPoint(curStylishRank);
		float diffCurPoint = curStylishPoint - curEnterPoint;
		float diffNextEnterPoint = nextEnterPoint - curEnterPoint;
		diffNextEnterPoint = std::max(diffNextEnterPoint, Mathf::epsilon);

		// スライダーを割合でセットする
		pStylishGage->SetCurrentRatio(diffCurPoint / diffNextEnterPoint);
	}

	void CP_StylishUI::BeginRankUpDirection()
	{
		// ランクの文字の演出をする
		if (!pStylishRankRenderer) return;

		Transform& rankUiTransform = GetTransform();

		// ランクにUIを演出の初期状態にする
		elapsedRankupDirectionTime = 0.0f;
		curRankupDirState = RankupDirectionState::Scaling;

		// 拡大した状態から
		DXSimp::Vector3 beginRankUIScale = rankUIBaseScale * rankupScalingRatio;
		beginRankUIScale.z = rankUIBaseScale.z;

		// 透明から
		pStylishRankRenderer->SetAlpha(0.0f);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetNextRank(StylishRank _targetRank) const
	{
		// 一番上のランクなら起こさない
		if (_targetRank == StylishRank::SSS) return StylishRank::SSS;

		u_int nextRankId = static_cast<u_int>(curStylishRank) + 1;
		return static_cast<StylishRank>(nextRankId);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetPrevRank(StylishRank _targetRank) const
	{
		// 一番上のランクなら起こさない
		if (_targetRank == StylishRank::None) return StylishRank::None;

		u_int prevRankId = static_cast<u_int>(curStylishRank) - 1;
		return static_cast<StylishRank>(prevRankId);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetCurrentRankFromPoint()
	{
		int rankCnt = static_cast<u_int>(StylishRank::MaxNum);

		// 上のランクから見ていき、現在のポイントが必要なポイントより初めて上になるランクが現在のランク
		for (int r_i = rankCnt - 1; r_i >= 0; r_i--)
		{
			if (curStylishPoint < rankParameters[r_i].requireEnterPoints)
				continue;

			return static_cast<StylishRank>(r_i);
		}

		HASHI_DEBUG_LOG("正常にランクを取得できませんでした");
		return StylishRank::None;
	}

	bool CP_StylishUI::GetIsCurMaxRank() const
	{
		return curStylishRank == StylishRank::SSS;
	}

	void CP_StylishUI::ImGuiDebug()
	{
#ifdef EDIT
		Component::ImGuiDebug();

		// 現在の状態
		ImGui::Text(std::string(magic_enum::enum_name(curStylishRank)).c_str());	// ランク
		ImGui::Text("curPoint:%f", curStylishPoint);	// 現在のポイント
		ImGui::Text("nextPoint:%f", GetEnterRankPoint(GetNextRank(curStylishRank)));	// 次のポイント
		ImGui::Text("elapsed:%f", elapsedToDecadeTime);	// 現在の経過
		ImGui::DragFloat("decadeBeginTime", &decadeBeginTime, 0.1f, 0.0f, 100.0f);

		// ランク上がったときの演出
		ImGuiRankupDirection();

		// ランクパラメータ
		ImGuiRankParameter();

		// オブジェクトをセット
		ImGuiSetObject();

#endif // EDIT
	}

	void CP_StylishUI::ImGuiRankupDirection()
	{
		if (!ImGuiMethod::TreeNode("Rankup Directio")) return;

		Easing::ImGuiSelect(rankupEase, "Rankup Ease");
		ImGui::DragFloat("Scaling Time", &rankupScalingTime, 0.01f, 0.0f, 100.0f);
		ImGui::DragFloat("Scaling Ratio", &rankupScalingRatio, 0.01f, 1.0f, 100.0f);

		rankupShakeParam.ImGuiCall();

		ImGui::TreePop();
	}

	void CP_StylishUI::ImGuiRankParameter()
	{
#ifdef EDIT
		u_int rankCnt = static_cast<u_int>(StylishRank::MaxNum);
		for (u_int r_i = 1; r_i < rankCnt; r_i++)
		{
			// ランク名を取得する
			StylishRank rank = static_cast<StylishRank>(r_i);
			std::string rankName = std::string(magic_enum::enum_name(rank));

			if (!ImGuiMethod::TreeNode(rankName)) continue;
			RankParameter& rankParam = rankParameters[r_i];

			// ランクのテクスチャ
			std::string texName;
			if (rankParam.displayTexture)
				texName = rankParam.displayTexture->GetAssetName();
			if (AssetGetter::ImGuiGetCombobox<Texture>("Texture", texName))
			{
				rankParam.displayTexture = AssetGetter::GetAsset<Texture>(texName);
			}

			ImGui::DragFloat("EnterPoint", &rankParam.requireEnterPoints, 0.1f, 0.0f, 10000.0f);	// ランクの条件
			ImGui::DragFloat("DecadeSpeed", &rankParam.decadePointSpeed, 0.1f, 0.0f, 10000.0f);	// 減る速度

			ImGui::TreePop();
		}
#endif // EDIT
	}

	void CP_StylishUI::ImGuiSetObject()
	{
#ifdef EDIT
		// 入力
		static char input[IM_INPUT_BUF] = "\0";
		ImGui::InputText("##Obj", input, IM_INPUT_BUF);

		// セット
		if (ImGui::Button("Set Gage"))
		{
			stylishGageName = input;
		}
		ImGui::SameLine();
		ImGui::Text(stylishGageName.c_str());

		if (ImGui::Button("Set GageGroup"))
		{
			gageGroupObjName = input;
		}
		ImGui::SameLine();
		ImGui::Text(gageGroupObjName.c_str());
#endif // EDIT
	}
}
