#pragma once

// ゲームオブジェクトのタグクラス
class Tag
{
public:
	enum Type	// タグの種類
	{
		Default,	// デフォルト
		MainCamera,	// カメラ
		Player,	// プレイヤー
		Enemy,	// 敵
		Ground,	// 地面
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

	/// @brief コンボボックスによる変更
	/// @param _curType 変更したい変数
	/// @return 変更したかどうか
	static bool ImGuiComboBox(Type& _curType);
};

// ゲームオブジェクトのレイヤークラス
class Layer
{
public:
	enum Type	// タグの種類
	{
		Default,	// デフォルト
		Object,
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

	/// @brief コンボボックスによる変更
	/// @param _curType 変更したい変数
	/// @return 変更したかどうか
	static bool ImGuiComboBox(Type& _curType);
};
