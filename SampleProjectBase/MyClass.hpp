#pragma once

class MyClass
{

public:
	MyClass();
	~MyClass();
};

// コンストラクタとデストラクタのインライン定義
inline MyClass::MyClass() { }
inline MyClass::~MyClass() { }