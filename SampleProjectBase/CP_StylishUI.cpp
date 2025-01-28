#include "pch.h"
#include "CP_StylishUI.h"
#include "CP_UISlider.h"

namespace HashiTaku
{
	/// @brief ���E�̃X�^�C���b�V���|�C���g
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
		// ���Z
		curStylishPoint += _addPoint;
		curStylishPoint = std::min(curStylishPoint, MAX_LIMIT_STYLISHPOINT);

		// �������ԃ��Z�b�g
		elapsedToDecadeTime = 0.0f;

		// ���̃����N�̏����ɒB�������H
		StylishRank nextRank = GetNextRank(curStylishRank);
		if (curStylishPoint >= GetEnterRankPoint(nextRank))
		{
			// �B�����玟�̃����N��
			ChangeRank(nextRank);
		}

		// �X���C�_�[�ɔ��f
		ApplyStylishGage();
	}

	void CP_StylishUI::DecadeStylishPoint(float _decadePoint)
	{
		// ���Z
		curStylishPoint -= _decadePoint;
		curStylishPoint = std::max(curStylishPoint, 0.0f);

		// ���̃����N�̃|�C���g��艺��������H
		if (curStylishPoint >= GetEnterRankPoint(curStylishRank))
		{
			// �O�̃����N��
			ChangeRank(GetPrevRank(curStylishRank));
		}

		// �X���C�_�[�ɔ��f
		ApplyStylishGage();
	}

	void CP_StylishUI::Update()
	{
		// �����ł��邩�m�F
		if (!GetCanActive()) return;

		// �|�C���g�����炷
		DecadePointUpdate();
	}

	void CP_StylishUI::DecadePointUpdate()
	{
		// ���鎞�Ԃ��߂����Ȃ�
		if (elapsedToDecadeTime > decadeBeginTime)
		{
			// ���t���[���Ō��炷�|�C���g���v�Z����
			float decadePoint =	rankParameters[static_cast<u_int>(curStylishRank)].decadePointSpeed * 
				DeltaTime();

			// ���炷
			DecadeStylishPoint(decadePoint);
		}
		else // ������Ă���Ȃ�
		{
			// ���Z����
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

		// ���̃����N�ɓ���|�C���g���擾
		float nextEnterPoint = MAX_LIMIT_STYLISHPOINT;

		// �ő僉���N�łȂ��Ȃ�
		if (!GetIsCurMaxRank())
		{
			// ���̃����N�̃|�C���g���擾����
			StylishRank nextRank = GetNextRank(curStylishRank);
			nextEnterPoint = GetEnterRankPoint(nextRank);
		}
		
		// ���݂̃|�C���g�����܂ł̂ǂ̒��x�̊����Ȃ̂����߂�
		float curEnterPoint = GetEnterRankPoint(curStylishRank);
		float diffCurPoint = curStylishPoint - curEnterPoint;
		float diffNextEnterPoint = nextEnterPoint - curEnterPoint;
		diffNextEnterPoint = std::max(diffNextEnterPoint, Mathf::epsilon);

		// �X���C�_�[�������ŃZ�b�g����
		pStylishGage->SetCurrentRatio(diffCurPoint / diffNextEnterPoint);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetNextRank(StylishRank _targetRank) const
	{
		// ��ԏ�̃����N�Ȃ�N�����Ȃ�
		if (_targetRank == StylishRank::SSS) return StylishRank::SSS;

		u_int nextRankId = static_cast<u_int>(curStylishRank) + 1;
		return static_cast<StylishRank>(nextRankId);
	}

	CP_StylishUI::StylishRank CP_StylishUI::GetPrevRank(StylishRank _targetRank) const
	{
		// ��ԏ�̃����N�Ȃ�N�����Ȃ�
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
