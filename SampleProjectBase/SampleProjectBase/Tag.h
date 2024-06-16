#pragma once

// ゲームオブジェクトのタグクラス
class Tag
{
public:
	enum Type	// タグの種類
	{
		Default,	// デフォルト
		Num
	};

private:
	Type type;	// 種類

public:
	Tag() : type(Default) {};
	Tag(Type _type) : type(_type) {};
	~Tag() {};

	// 代入・取得
	void SetType(Type _type) { if (type >= Num)return; type = _type; }
	Type GetType()const { return type; }
};

// ゲームオブジェクトのレイヤークラス
class Layer
{
public:
	enum Type	// タグの種類
	{
		Default,	// デフォルト
		UI,	// UI
		Num
	};

private:
	Type type;	// 種類

public:
	Layer() : type(Default) {};
	Layer(Type _type) : type(_type) {};
	~Layer() {};

	// 代入・取得
	void SetType(Type _type) { if (type >= Num)return; type = _type; }
	Type GetType()const { return type; }
};
