#pragma once

// ���W�E��]�E�X�P�[�����܂Ƃ߂��p�����[�^
class Transform
{

public: 
	Float3 position;	// ���W
	Float3 rotation;	// ��]�p�x
	Float3 scale;	// �X�P�[��
	Float4 Quaternion;	// �N�H�[�^�j�I��

	Transform();
	~Transform();	
};

