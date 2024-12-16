#include "pch.h"
#include "AnimationCurve.h"

namespace HashiTaku
{
	constexpr int MIN_PLOT_CNT(2);	// �Œ���K�v�ȃv���b�g�_

	constexpr float VECTOR_LENGTH(2.0f);	// ���x�̑傫��
	constexpr float VEC_DRAW_TIMES(0.05f);	// �x�N�g���_�Ƃ̋�������`�悷��Ƃ��̔{��
	constexpr float REACT_DISTANCE = 0.02f; // �}�E�X�N���b�N�œ_�Ƃ̔�������

	constexpr float MIN_TIME(0.0f);	// �ŏ�����
	constexpr float MAX_TIME(1.0f);	// �ŏ�����
	constexpr double MIN_VAL(0.0);	// �ŏ��l
	constexpr double MAX_VAL(10000.0);	// �ő�l

	constexpr double DISPLAY_OFFSET(0.2);	// �O���t�\���̎���̗]���̒���

	AnimationCurve::AnimationCurve()
		: easeKind(EaseKind::Linear), isUsePlot(true)
	{
		plotPoints =
		{
			{1.0f, 0.0f, 0.0f},
			{1.0f, 1.0f, 0.0f}
		};
	}

	void AnimationCurve::SetCurveName(const std::string& _name)
	{
#ifdef EDIT
		curveName = _name;
#endif // EDIT
	}

	float AnimationCurve::GetValue(float _time) const
	{
		float returnVal = 0.0f;

		_time = std::clamp(_time, MIN_TIME, MAX_TIME);

		if (isUsePlot)
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

			// �G���~�[�g
			if (prevItr->toNextInterp == InterpKind::Hermite)
				returnVal = CalcHermiteCurve(_time, *prevItr, *nextItr);
			// ���`
			else
				returnVal = CalcLinearCurve(_time, *prevItr, *nextItr);
		}
		else
		{
			returnVal = Easing::EaseValue(_time, easeKind);
		}

		return returnVal;
	}

	float AnimationCurve::CalcHermiteCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const
	{
		// 2�_�Ԃ̊���
		float ratio = (_getTime - _p0.time) / (_p1.time - _p0.time);
		ratio = Easing::EaseValue(ratio, easeKind);

		float t2 = ratio * ratio;
		float t3 = t2 * ratio;
		float mP0 = 2 * t3 - 3 * t2 + 1;
		float mV0 = t3 - 2 * t2 + ratio;
		float mP1 = -2 * t3 + 3 * t2;
		float mV1 = t3 - t2;

		return _p0.value * mP0 + _p0.vector * mV0 + _p1.value * mP1 + _p1.vector * mV1;
	}

	float AnimationCurve::CalcLinearCurve(float _getTime, const HermitePlotParam& _p0, const HermitePlotParam& _p1) const
	{
		// 2�_�Ԃ̊���
		float ratio = (_getTime - _p0.time) / (_p1.time - _p0.time);
		return Mathf::Lerp(_p0.value, _p1.value, ratio);
	}

	bool AnimationCurve::SortPointTime(const HermitePlotParam& _p0, const HermitePlotParam& _p1)
	{
		return _p0.time < _p1.time;
	}

	ImPlotRect AnimationCurve::GetPlotRect()
	{
		ImPlotRect rect;
		rect.X.Max = -1000.0;
		rect.X.Min = 1000.0;
		rect.Y.Max = -1000.0;
		rect.Y.Min = 1000.0;

		for (auto& plot : plotPoints)
		{
			rect.X.Min = std::min((double)plot.time, rect.X.Min);
			rect.X.Max = std::max((double)plot.time, rect.X.Max);

			rect.Y.Min = std::min((double)plot.value, rect.Y.Min);
			rect.Y.Max = std::max((double)plot.value, rect.Y.Max);
		}

		return rect;
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

	bool AnimationCurve::CheckReactPlot(const HermitePlotParam& _plot)
	{
		return CheckReactPlot(_plot.time, _plot.value);
	}

	bool AnimationCurve::CheckReactPlot(float _posX, float _posY)
	{
		ImPlotPoint mousePos = ImPlot::GetPlotMousePos();
		float disx = abs(static_cast<float>(mousePos.x) - _posX);
		float disy = abs(static_cast<float>(mousePos.y) - _posY);

		ImPlotPoint size = ImPlot::GetPlotLimits().Size();

		if (disx < REACT_DISTANCE * size.x && disy < REACT_DISTANCE * size.y)   // �����������Ȃ�
		{
			return true;
		}

		return false;
	}

	void AnimationCurve::SetEase(EaseKind _easeKind)
	{
		easeKind = _easeKind;
	}

	bool AnimationCurve::IsStartOrEndPlot(const HermitePlotParam* _checkPlot)
	{
		return _checkPlot == &*plotPoints.begin() || _checkPlot == &*std::prev(plotPoints.end());
	}

	void AnimationCurve::ImGuiDebug()
	{
#ifdef EDIT
		ImGui::PushID(curveName.c_str());

		ImGui::Text(curveName.c_str());

		// �C�[�W���O�I��
		Easing::ImGuiSelect(easeKind);
		ImGui::Checkbox("UseHermite", &isUsePlot);

		// �f�[�^�Ɋ�Â����\���͈͂�ݒ�
		if (isUsePlot)
		{
			auto rect = GetPlotRect();

			double offsetX = std::max(DISPLAY_OFFSET * rect.Size().x, DISPLAY_OFFSET);
			double offsetY = std::max(DISPLAY_OFFSET * rect.Size().y, DISPLAY_OFFSET);

			ImPlot::SetNextAxesLimits(
				rect.X.Min - offsetX,
				rect.X.Max + offsetX,
				rect.Y.Min - offsetY,
				rect.Y.Max + offsetY,
				ImGuiCond_Always);
		}
		else
		{
			ImPlot::SetNextAxesLimits(
				MIN_TIME - DISPLAY_OFFSET,
				MAX_TIME + DISPLAY_OFFSET,
				MIN_VAL - DISPLAY_OFFSET,
				1.0f + DISPLAY_OFFSET,
				ImGuiCond_Always);
		}


		if (ImPlot::BeginPlot("##Animation Curve"))
		{
			ImPlot::SetupAxes("##Time", "##Value", ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit);

			if (isUsePlot)
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

		ImGui::PopID();

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
				if (CheckReactPlot(point))
				{
					editingPlot = &point;
					dragingPlot = &point;
					isSelecting = true;
					break;
				}
			}

			return;
		}

		// �h���b�O�ҏW
		if (ImGui::IsMouseDragging(0))
		{
			if (!dragingPlot) return;

			// �l�𓮂���
			editingPlot->value = floatMousePos.y;

			// ���Ԃ͍ŏ��ƍŌ�͓������Ȃ�
			if (!IsStartOrEndPlot(editingPlot))
			{
				float changePrev = editingPlot->time;

				// ������������
				editingPlot->time = std::clamp(floatMousePos.x, MIN_TIME + Mathf::epsilon,
					MAX_TIME - Mathf::epsilon);

				// �\�[�g����(����)
				if (changePrev != editingPlot->time)
					plotPoints.sort(SortPointTime);
			}
		}

		// �N���b�N�𗣂�����
		if (ImGui::IsMouseReleased(0))
			dragingPlot = nullptr;

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

				if (!CheckReactPlot((float)vectorPoint.x, (float)vectorPoint.y))
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
			{
				isVectorEdit = false;
			}

		}

#endif // EDIT
	}

	void AnimationCurve::ImDrawGraph()
	{
		ImGui::Text("count:%d", static_cast<int>(plotPoints.size()));

		// ��Ԃ��ꂽ�J�[�u��`��
		constexpr int DRAW_POINT_CNT = 200;
		float timePoints[DRAW_POINT_CNT];
		float valuePoints[DRAW_POINT_CNT];

		if (isUsePlot)
		{
			// �O�ƌ�̃v���b�g�_���擾
			auto prevProtItr = plotPoints.begin();
			auto nextProtItr = std::next(prevProtItr);
			InterpKind curKind = prevProtItr->toNextInterp;
			for (int p_i = 0; p_i < DRAW_POINT_CNT; p_i++)
			{
				timePoints[p_i] = (float)p_i / (DRAW_POINT_CNT - 1);

				// ���Ԃ���̃v���b�g�̎��Ԃ𒴂�����
				if (timePoints[p_i] > (*nextProtItr).time)
				{
					// �v���b�g���X�V
					prevProtItr = nextProtItr;
					nextProtItr = std::next(nextProtItr);
					curKind = prevProtItr->toNextInterp;
				}

				// �l���擾����
				if (curKind == InterpKind::Hermite)
					valuePoints[p_i] = CalcHermiteCurve(timePoints[p_i], *prevProtItr, *nextProtItr);
				else
					valuePoints[p_i] = CalcLinearCurve(timePoints[p_i], *prevProtItr, *nextProtItr);
			}
		}
		else
		{
			for (int p_i = 0; p_i < DRAW_POINT_CNT; p_i++)
			{
				timePoints[p_i] = (float)p_i / (DRAW_POINT_CNT - 1);
				valuePoints[p_i] = Easing::EaseValue(timePoints[p_i], easeKind);
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
				if (CheckReactPlot(plot))
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
					if (ImGui::DragFloat("Time", &editingPlot->time, 0.001f, 0.0f, MAX_TIME - Mathf::smallValue))
					{
						plotPoints.sort(SortPointTime);
					}
				}

				ImGui::DragFloat("Value", &editingPlot->value, 0.001f);
				ImGui::DragFloat("Vector", &editingPlot->vector, 0.001f, -1.0f, 1.0f);

				// ��Ԏ�ނ�ς���
				std::vector<std::string> interpKindName
				{
					"Hermite",
					"Linear"
				};

				u_int id = static_cast<int>(editingPlot->toNextInterp);
				if (ImGuiMethod::ComboBox("##interpKind", id, interpKindName))
					editingPlot->toNextInterp = static_cast<InterpKind>(id);
			}

			if (ImGui::MenuItem("Add"))
			{
				// �}�E�X�̏ꏊ
				AddPlot(static_cast<float>(clickPos.x));
			}
			int plotCnt = static_cast<int>(plotPoints.size());

			if (!IsStartOrEndPlot(editingPlot))
			{
				if (ImGui::MenuItem("Delete", nullptr, false, plotCnt > MIN_PLOT_CNT && deletePlot))
				{
					if (deletePlot)
						RemovePlot(*deletePlot);
				}
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
		data["useHermite"] = isUsePlot;

		auto& plotDatas = data["plotDatas"];
		for (const auto& plot : plotPoints)
		{
			nlohmann::json plotData;

			// �n�_�ƏI�_
			if (&plot == &*plotPoints.begin())
			{
				data["startValue"] = plot.value;
				data["startVector"] = plot.vector;
				data["startInterp"] = plot.toNextInterp;
				continue;
			}
			else if (&plot == &*std::prev(plotPoints.end()))
			{
				data["endValue"] = plot.value;
				data["endVector"] = plot.vector;
				continue;
			}
			else // ����ȊO��
			{
				plotData["time"] = plot.time;
				plotData["value"] = plot.value;
				plotData["vector"] = plot.vector;
				plotData["interp"] = plot.toNextInterp;
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

		LoadJsonBoolean("useHermite", isUsePlot, _data);

		nlohmann::json plotDatas;
		if (LoadJsonDataArray("plotDatas", plotDatas, _data))
		{
			for (auto& plotData : plotDatas)
			{
				HermitePlotParam addPlot;
				LoadJsonFloat("time", addPlot.time, plotData);
				LoadJsonFloat("value", addPlot.value, plotData);
				LoadJsonFloat("vector", addPlot.vector, plotData);
				LoadJsonEnum<InterpKind>("interp", addPlot.toNextInterp, plotData);
				plotPoints.push_back(addPlot);
			}
		}

		plotPoints.sort(SortPointTime);

		float loadVector = 0.0f;
		if (LoadJsonFloat("startVector", loadVector, _data))
		{
			LoadJsonFloat("startValue", (*plotPoints.begin()).value, _data);
			LoadJsonEnum<InterpKind>("startInterp", (*plotPoints.begin()).toNextInterp, _data);

			(*plotPoints.begin()).vector = loadVector;
		}
		if (LoadJsonFloat("endVector", loadVector, _data))
		{
			LoadJsonFloat("endValue", (*std::prev(plotPoints.end())).value, _data);
			(*std::prev(plotPoints.end())).vector = loadVector;
		}
	}
}