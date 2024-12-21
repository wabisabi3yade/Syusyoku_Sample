#include "pch.h"
#include "CP_NotifyTest.h"

CP_NotifyTest::CP_NotifyTest(const CP_NotifyTest& _other)
{
	Copy(_other);
}

CP_NotifyTest& CP_NotifyTest::operator=(const CP_NotifyTest& _other)
{
	Copy(_other);

	return *this;
}

void CP_NotifyTest::Init()
{
	//pFactory = std::make_unique<AnimationNotifyFactory>();
}

void CP_NotifyTest::Update()
{
	curRatio += speed * DeltaTime();
	bool isLoop = false;

	if (curRatio > 1.0f)
	{
		curRatio -= 1.0f;
		isLoop = true;
	}

	for (auto& n : pNotifys)
	{
		n->Update(lastRatio, curRatio, isLoop);
	}

	lastRatio = curRatio;
}

void CP_NotifyTest::ImGuiDebug()
{
	ImGui::DragFloat("speed", &speed, 1.0f, 0.0f, 100.0f);
	ImGui::SliderFloat("curRatio", &curRatio, 0.0f, 1.0f);
	ImGui::SliderFloat("lastRatio", &lastRatio, 0.0f, 1.0f);
	ImGui::Text(std::to_string(pNotifys.size()).c_str());

	u_int i = 1;
	auto itr = pNotifys.begin();
	while (itr != pNotifys.end())
	{
		bool isDelete = false;
		std::string caption = std::to_string(i) + " " + (*itr)->GetNotifyName();
		if (ImGuiMethod::TreeNode(caption))
		{
			(*itr)->ImGuiCall();
			isDelete = ImGui::Button("Delete");
			ImGui::TreePop();
		}
		i++;
		if (isDelete)
			itr = pNotifys.erase(itr);
		else
			itr++;
	}

	std::unique_ptr<AnimationNotify_Base> pCreate;
	if (pFactory->ImGuiCombo(pCreate))
	{
		pNotifys.push_back(std::move(pCreate));
	}
}

json CP_NotifyTest::Save()
{
	auto data = Component::Save();

	auto& notifyData = data["notifys"];
	u_int idx = 0;
	for (auto& n : pNotifys)
	{
		/*auto a = TYPENAME_VAR_ROUGH(*n);
		notifyData[idx] = TYPENAME_VAR(*n);*/

		notifyData[idx]["notify"] = n->Save();
		notifyData[idx]["typeName"] = n->GetNotifyType();
		idx++;
	}

	data["speed"] = speed;
	return data;
}

void CP_NotifyTest::Load(const json& _data)
{
	Component::Load(_data);

	auto& notifyData = _data["notifys"];
	for (auto& nData : notifyData)
	{
		AnimationNotify_Base::NotifyType typeName; HashiTaku::LoadJsonEnum<AnimationNotify_Base::NotifyType>("typeName", typeName, nData);
		//auto notify = pFactory->Create(typeName);
		/*if (notify)
		{
			json d; HashiTaku::LoadJsonData("notify", d, nData);
			notify->Load(d);
			pNotifys.push_back(std::move(notify));
		}*/

	}

	HashiTaku::LoadJsonFloat("speed", speed, _data);
}

void CP_NotifyTest::Copy(const CP_NotifyTest& _other)
{
	if (this == &_other) return;

	Component::operator=(_other);

	for (auto& n : _other.pNotifys)
	{
		std::unique_ptr<AnimationNotify_Base> pCopy;
		*pCopy = *n;
	}

	lastRatio = _other.lastRatio;
	curRatio = _other.curRatio;
	speed = _other.speed;
}
