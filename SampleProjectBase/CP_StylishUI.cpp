#include "pch.h"
#include "CP_StylishUI.h"
#include "CP_UISlider.h"

namespace HashiTaku
{
	/// @brief 限界のスタイリッシュポイント
	constexpr float MAX_LIMIT_STYLISHPOINT(100000);

	CP_StylishUI::CP_StylishUI() :
		pUIRenderer(nullptr),
		pStylishGage(nullptr),
		curStylishRank(StylishRank::D),
		curStylishPoint(0),
		decadeBeginTime(7.0f),
		elapsedToDecadeTime(0.0f),
		isPointDecading(false)
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
			ChangeRank(nextRank);
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
		if (curStylishPoint >= GetEnterRankPoint(curStylishRank))
		{
			// 前のランクへ
			ChangeRank(GetPrevRank(curStylishRank));
		}

		// スライダーに反映
		ApplyStylishGage();
	}

	void CP_StylishUI::Update()
	{
		// 活動できるか確認
		if (!GetCanActive()) return;

		// ポイントを減らす
		DecadePointUpdate();
	}

	void CP_StylishUI::DecadePointUpdate()
	{
		// 減る時間を過ぎたなら
		if (elapsedToDecadeTime > decadeBeginTime)
		{
			// 今フレームで減らすポイントを計算する
			float decadePoint =	rankParameters[static_cast<u_int>(curStylishRank)].decadePointSpeed * 
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

	bool CP_StylishUI::GetCanActive() const
	{
		if (!pUIRenderer) return false;
		if (!pStylishGage) return false;

		return true;
	}

	float CP_StylishUI::GetEnterRankPoint(StylishRank _checkRank) const
	{
		return rankParameters[static_cast<u_int>(_checkRank)].requireEnterPoints;
	}

	void CP_StylishUI::ChangeRank(StylishRank _targetRank)
	{
		curStylishRank = _targetRank;
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

	bool CP_StylishUI::GetIsCurMaxRank() const
	{
		return curStylishRank == StylishRank::SSS;
	}

	void CP_StylishUI::ImGuiDebug()
	{
	}
}
