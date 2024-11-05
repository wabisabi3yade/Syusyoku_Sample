#include "pch.h"
#include "AnimationCurve.h"

constexpr int MIN_PLOT_CNT(2);	// �Œ���K�v�ȃv���b�g�_

constexpr float VECTOR_LENGTH(2.0f);	// ���x�̑傫��
constexpr float VEC_DRAW_TIMES(0.05f);	// �x�N�g���_�Ƃ̋�������`�悷��Ƃ��̔{��
constexpr float REACT_DISTANCE = 0.0035f; // �}�E�X�N���b�N�œ_�Ƃ̔�������

constexpr float MIN_TIME(0.0f);	// �ŏ�����
constexpr float MAX_TIME(1.0f);	// �ő厞��
constexpr float MIN_VAL(0.0f);	// �ŏ��l
constexpr float MAX_VAL(1.0f);	// �ő�l

AnimationCurve::AnimationCurve()
	: easeKind(HashiTaku::EaseKind::Linear)
{
	plotPoints =
	{
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 0.0f}
	};
}

float AnimationCurve::GetValue(float _time) const
{
	float returnVal = 0.0f;

	_time = std::clamp(_time, MIN_TIME, MAX_TIME);

	if (isUseHermite)
	{
		// �O�ƌ��������
		auto prevItr = plotPoints.begin();
		auto nextItr = std::next(prevItr);
		for (; nextItr != plotPoints.end(); nextItr++)
		{
			// ������������
			if (_time <= (*nextItr).time)
			{
				prevItr = std::prev(nextItr);
				break;
			}
			else
				assert("���̎��ԉ挩����܂���ł���");
		}

		returnVal = CalcHermiteCurve(_time, *prevItr, *nextItr);

	}
	else
	{
		returnVal = HashiTaku::Easing::EaseValue(_time, easeKind);
	}

	return returnVal;
}

float AnimationCurve::CalcHermiteCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const
{
	// 2�_�Ԃ̊���
	float ratio = (_getTime - _p0.time) / (_p1.time - _p0.time);
	ratio = HashiTaku::Easing::EaseValue(ratio, easeKind);

	float t2 = ratio * ratio;
	float t3 = t2 * ratio;
	float mP0 = 2 * t3 - 3 * t2 + 1;
	float mV0 = t3 - 2 * t2 + ratio;
	float mP1 = -2 * t3 + 3 * t2;
	float mV1 = t3 - t2;

	return _p0.value * mP0 + _p0.vector * mV0 + _p1.value * mP1 + _p1.vector * mV1;
}

bool AnimationCurve::SortPointTime(const HermitePlotParam& _p0, const HermitePlotParam& _p1)
{
	return _p0.time < _p1.time;
}

void AnimationCurve::AddPlot(float _time)
{
	// ����
	_time = std::clamp(_time, MIN_TIME + Mathf::epsilon, MAX_TIME - Mathf::epsilon);

	HermitePlotParam addPlot;
	addPlot.time = _time;

	// �O�ƌ��������
	auto prevItr = plotPoints.begin();
	auto nextItr = std::next(prevItr);
	for (; nextItr != plotPoints.end(); nextItr++)
	{
		// ������������
		if (_time < (*nextItr).time)
		{
			prevItr = std::prev(nextItr);
			break;
		}
		else
			assert("���̎��ԉ挩����܂���ł���");
	}

	// �l�����߂�
	addPlot.value = CalcHermiteCurve(_time, *prevItr, *nextItr);

	// �x�N�g�������߂�
	addPlot.vector = ((*prevItr).vector + (*nextItr).vector) * 0.5f;

	// �ǉ�
	plotPoints.insert(nextItr, addPlot);
}

void AnimationCurve::RemovePlot(HermitePlotParam& _deletePlot)
{
#ifdef EDIT
	if (editingPlot == &_deletePlot)
		editingPlot = nullptr;
#endif // EDIT

	auto itr = std::find(plotPoints.begin(), plotPoints.end(), _deletePlot);
	if (itr == plotPoints.end())
	{
		return;
	}


	plotPoints.erase(itr);
}

bool AnimationCurve::CheckReactPlot(const HermitePlotParam& _plot, float _clickX, float _clickY)
{
	float disx = _clickX - _plot.time;
	float disy = _clickY - _plot.value;
	float dis = disx * disx + disy * disy;

	if (dis < REACT_DISTANCE)   // �����������Ȃ�
	{
		return true;
	}


	return false;
}

void AnimationCurve::SetEase(HashiTaku::EaseKind _easeKind)
{
	easeKind = _easeKind;

	// �n�_�ƏI�_�̃x�N�g�����ł��邾�����킹��
	/*HermitePlotParam* plot = &*plotPoints.begin();
	DirectX::SimpleMath::Vector2 dis;
	dis.x = Mathf::epsilon;
	float targetTime = plot->time + Mathf::epsilon;
	float val = HashiTaku::Easing::EaseValue(targetTime, easeKind);
	dis.y = val - plot->value;
	dis.Normalize();
	plot->vector = dis.y * VECTOR_LENGTH;

	plot = &*std::prev(plotPoints.end());
	targetTime = plot->time - Mathf::epsilon;
	val = HashiTaku::Easing::EaseValue(targetTime, easeKind);
	dis.x = Mathf::epsilon;
	dis.y = plot->value - val;
	dis.Normalize();
	plot->vector = dis.y * VECTOR_LENGTH;*/
}

bool AnimationCurve::IsStartOrEndPlot(const HermitePlotParam* _checkPlot)
{
	return _checkPlot == &*plotPoints.begin() || _checkPlot == &*std::prev(plotPoints.end());
}

void AnimationCurve::ImGuiSetting()
{
#ifdef EDIT
	//	ImGui::SetNextWindowSize(ImVec2(1.0f, 1.0f), ImGuiCond_Always);
		// ImPlot�����������ăJ�[�u��`��

	ImGui::Text("Curve");
	HashiTaku::Easing::ImGuiSelect(easeKind);
	ImGui::Checkbox("UseHermite", &isUseHermite);
	if (ImPlot::BeginPlot("##Animation Curve"))
	{
		ImPlot::SetupAxes("##Time", "##Value", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);
		ImPlot::SetupAxisLimits(ImAxis_X1, 0.0f, 1.0f);
		ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, 1.0f);

		if (isUseHermite)
		{
			// �x�N�g���_��ҏW
			ImEditVectorPoint();

			// �v���b�g�_��ҏW
			ImGuiPlotEditing();

			// �ǉ��E�폜
			ImAddPopPlot();			
		}

		ImDrawGraph();
		ImPlot::EndPlot();
	}

#endif // EDIT
}

void AnimationCurve::ImGuiPlotEditing()
{
#ifdef EDIT
	// �L�[�t���[���̓_��\��
	for (const auto& point : plotPoints)
	{
		ImPlot::PlotScatter("##Keyframes", &point.time, &point.value, 1);
	}

	// �}�E�X�ҏW
	ImPlotPoint mousePos = ImPlot::GetPlotMousePos();
	DirectX::SimpleMath::Vector2 floatMousePos =
	{
		static_cast<float>(mousePos.x),
		static_cast<float>(mousePos.y)
	};

	// �N���b�N��
	if (ImGui::IsMouseClicked(0))
	{
		bool isSelecting = false;
		for (auto& point : plotPoints)
		{
			if (CheckReactPlot(point, floatMousePos.x, floatMousePos.y))
			{
				editingPlot = &point;
				isSelecting = true;
				break;
			}
		}
	}
	// �h���b�O�ҏW
	if (ImGui::IsMouseDragging(0))
	{
		if (!editingPlot) return;

		// �ŏ��ƍŌ�͓������Ȃ�
		if (!IsStartOrEndPlot(editingPlot) &&
			CheckReactPlot(*editingPlot, floatMousePos.x, floatMousePos.y))
		{
			float changePrev = editingPlot->time;

			// ������������
			editingPlot->time = std::clamp(floatMousePos.x, 0.0f + Mathf::epsilon, 1.0f - Mathf::epsilon);
			editingPlot->value = std::clamp(floatMousePos.y, 0.0f + Mathf::epsilon, 1.0f - Mathf::epsilon);

			// �\�[�g����(����)
			if (changePrev != editingPlot->time)
				plotPoints.sort(SortPointTime);
		}
	}

#endif // EDIT

}

void AnimationCurve::ImEditVectorPoint()
{
#ifdef EDIT

	// �ҏW���Ă���_�𓮂���
	if (!editingPlot) return;

	// �`�掞�̃x�N�g���_
	ImPlotPoint vectorPoint;
	vectorPoint.y = editingPlot->value + editingPlot->vector * VEC_DRAW_TIMES;
	vectorPoint.x = editingPlot->time + std::pow(VECTOR_LENGTH * VECTOR_LENGTH - (editingPlot->vector * editingPlot->vector), 0.5f) * VEC_DRAW_TIMES;
	ImPlot::PlotScatter("##Vector", &vectorPoint.x, &vectorPoint.y, 1);

	ImPlotPoint mousePos = ImPlot::GetPlotMousePos();
	if (!isVectorEdit)	// �x�N�g���_�ҏW���Ă��Ȃ��Ȃ�
	{
		if (ImGui::IsMouseClicked(0))
		{
			// ��������
			double disVM_X = mousePos.x - vectorPoint.x;
			double disVM_Y = mousePos.y - vectorPoint.y;
			double disVM = disVM_X * disVM_X + disVM_Y * disVM_Y;

			if (disVM > REACT_DISTANCE)
				return;
			else
			{
				// �����������Ȃ�ҏW����
				isVectorEdit = true;
			}
		}
	}
	else
	{
		// �N���b�N��
		if (ImGui::IsMouseDragging(0))
		{
			DirectX::SimpleMath::Vector2 vecDis;
			vecDis.x = static_cast<float>(mousePos.x) - editingPlot->time;

			if (vecDis.x < 0.0f)
				vecDis.x = 0.0f;

			vecDis.y = static_cast<float>(mousePos.y) - editingPlot->value;
			vecDis.Normalize();

			editingPlot->vector = vecDis.y * VECTOR_LENGTH;
		}

		if (ImGui::IsMouseReleased(0))	// ��������
			isVectorEdit = false;
	}

#endif // EDIT
}

void AnimationCurve::ImDrawGraph()
{
	// ��Ԃ��ꂽ�J�[�u��`��
	constexpr int DRAW_POINT_CNT = 200;
	float timePoints[DRAW_POINT_CNT];
	float valuePoints[DRAW_POINT_CNT];

	if (isUseHermite)
	{
		// �O�ƌ�̃v���b�g�_���擾
		auto prevProtItr = plotPoints.begin();
		auto nextProtItr = std::next(prevProtItr);
		for (int p_i = 0; p_i < DRAW_POINT_CNT; p_i++)
		{
			timePoints[p_i] = (float)p_i / (DRAW_POINT_CNT - 1);

			// ���Ԃ���̃v���b�g�̎��Ԃ𒴂�����
			if (timePoints[p_i] > (*nextProtItr).time)
			{
				// �v���b�g���X�V
				prevProtItr = nextProtItr;
				nextProtItr = std::next(nextProtItr);
			}

			// �l���擾����
			valuePoints[p_i] = CalcHermiteCurve(timePoints[p_i], *prevProtItr, *nextProtItr);
		}
	}
	else
	{
		for (int p_i = 0; p_i < DRAW_POINT_CNT; p_i++)
		{
			timePoints[p_i] = (float)p_i / (DRAW_POINT_CNT - 1);
			valuePoints[p_i] = HashiTaku::Easing::EaseValue(timePoints[p_i], easeKind);
		}
	}
	

	ImPlot::PlotLine("##AnimationCurve", timePoints, valuePoints, DRAW_POINT_CNT);
}

void AnimationCurve::ImAddPopPlot()
{
#ifdef EDIT
	static ImPlotPoint clickPos;
	static HermitePlotParam* deletePlot = nullptr;

	// �E�N���b�N
	if (ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("##Curve Menu");
		clickPos = ImPlot::GetPlotMousePos();

		deletePlot = nullptr;
		for (auto& plot : plotPoints)
		{
			if (CheckReactPlot(plot, static_cast<float>(clickPos.x), static_cast<float>(clickPos.y)))
			{
				deletePlot = &plot;
				break;
			}
		}
	}
	if (ImGui::BeginPopup("##Curve Menu"))
	{
		if (editingPlot)
		{
			if (!IsStartOrEndPlot(editingPlot))
			{
				if (ImGui::DragFloat("Time", &editingPlot->time, 0.001f, 0.0f, 1.0f))
				{
					plotPoints.sort(SortPointTime);
				}

				ImGui::DragFloat("Value", &editingPlot->value, 0.001f, 0.0f, 1.0f);
			}
			ImGui::DragFloat("Vector", &editingPlot->vector, 0.001f, -1.0f, 1.0f);
		}

		if (ImGui::MenuItem("Add"))
		{
			// �}�E�X�̏ꏊ
			AddPlot(static_cast<float>(clickPos.x));
		}
		int plotCnt = static_cast<int>(plotPoints.size());
		if (ImGui::MenuItem("Delete", nullptr, false, plotCnt > MIN_PLOT_CNT && deletePlot))
		{
			if (deletePlot)
				RemovePlot(*deletePlot);
		}
		if (ImGui::MenuItem("Flat", nullptr, false, plotCnt > MIN_PLOT_CNT && deletePlot))
		{
			if (deletePlot)
				deletePlot->vector = 0.0f;
		}
		ImGui::EndPopup();
	}
#endif // EDIT
}

nlohmann::json AnimationCurve::Save()
{
	nlohmann::json data;

	data["easeKind"] = easeKind;
	data["useHermite"] = isUseHermite;

	auto& plotDatas = data["plotDatas"];
	for (const auto& plot : plotPoints)
	{
		nlohmann::json plotData;

		// �n�_�ƏI�_
		if (&plot == &*plotPoints.begin())
		{
			data["startVector"] = plot.vector;
			continue;
		}
		else if (&plot == &*std::prev(plotPoints.end()))
		{
			data["endVector"] = plot.vector;
			continue;
		}
		else // ����ȊO��
		{
			plotData["time"] = plot.time;
			plotData["value"] = plot.value;
			plotData["vector"] = plot.vector;
		}
		plotDatas.push_back(plotData);
	}

	return data;
}

void AnimationCurve::Load(const nlohmann::json& _data)
{
	using namespace HashiTaku;
	EaseKind loadEase = EaseKind::Linear;
	LoadJsonEnum<EaseKind>("easeKind", loadEase, _data);
	SetEase(loadEase);

	LoadJsonBoolean("useHermite", isUseHermite, _data);

	nlohmann::json plotDatas;
	if (LoadJsonDataArray("plotDatas", plotDatas, _data))
	{
		for (auto& plotData : plotDatas)
		{
			HermitePlotParam addPlot;
			LoadJsonFloat("time", addPlot.time, plotData);
			LoadJsonFloat("value", addPlot.value, plotData);
			LoadJsonFloat("vector", addPlot.vector, plotData);
			plotPoints.push_back(addPlot);
		}
	}

	plotPoints.sort(SortPointTime);

	float loadVector = 0.0f;
	if (LoadJsonFloat("startVector", loadVector, _data))
	{
		(*plotPoints.begin()).vector = loadVector;
	}
	if (LoadJsonFloat("endVector", loadVector, _data))
	{
		(*std::prev(plotPoints.end())).vector = loadVector;
	}
}
