#pragma once

class MyClass
{

public:
	MyClass();
	~MyClass();
};

// �R���X�g���N�^�ƃf�X�g���N�^�̃C�����C����`
inline MyClass::MyClass() { }
inline MyClass::~MyClass() { }