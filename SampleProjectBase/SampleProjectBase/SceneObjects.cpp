#include "pch.h"
#include "SceneObjects.h"
#include <string>

SceneObjects::SceneObjects()
{
}

SceneObjects::~SceneObjects()
{
	list.clear();	// 解放する
}

void SceneObjects::Update()
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		itr->second->Update();
	}
}

void SceneObjects::LateUpdate()
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		itr->second->LateUpdate();
	}
}

void SceneObjects::Draw()
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		itr->second->Draw();
	}
}

void SceneObjects::SetObject(const std::string& _objectName, std::unique_ptr<GameObject> _objPtr)
{
	// 既にオブジェクトの名前があるなら
	u_int loop = 0;
	std::string setName;	// セットするときの名前
	while (true)	// セットできるまで
	{
		std::string number = std::to_string(loop);
		if (loop == 0)	// 最初は何もつけないようにする
		{
			number = "";
		}
		
		setName = _objectName + number;	// オブジェクトの名前＋数字

		auto itr = list.find(setName);
		if (itr == list.end())	// 探してなかったら
		{
			break;	// ループを終わる
		}

		// あるなら
		loop++;	// ループ回数を増やす
	}

	list.insert(std::pair<std::string, std::unique_ptr<GameObject>>(setName, std::move(_objPtr)));
}
