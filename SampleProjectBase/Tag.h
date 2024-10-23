#pragma once

// �Q�[���I�u�W�F�N�g�̃^�O�N���X
class Tag
{
public:
	enum Type	// �^�O�̎��
	{
		Default,	// �f�t�H���g
		MainCamera,	// �J����
		Player,	// �v���C���[
		Enemy,	// �G
		Ground,	// �n��
		Num
	};

private:
	Type type;	// ���

public:
	Tag() : type(Default) {};
	Tag(Type _type) : type(_type) {};
	~Tag() {};

	// ����E�擾
	void SetType(Type _type) { if (type >= Num)return; type = _type; }
	Type GetType()const { return type; }

	/// @brief �R���{�{�b�N�X�ɂ��ύX
	/// @param _curType �ύX�������ϐ�
	/// @return �ύX�������ǂ���
	static bool ImGuiComboBox(Type& _curType);
};

// �Q�[���I�u�W�F�N�g�̃��C���[�N���X
class Layer
{
public:
	enum Type	// �^�O�̎��
	{
		Default,	// �f�t�H���g
		Object,
		UI,	// UI
		Num
	};

private:
	Type type;	// ���

public:
	Layer() : type(Default) {};
	Layer(Type _type) : type(_type) {};
	~Layer() {};

	// ����E�擾
	void SetType(Type _type) { if (type >= Num)return; type = _type; }
	Type GetType()const { return type; }

	/// @brief �R���{�{�b�N�X�ɂ��ύX
	/// @param _curType �ύX�������ϐ�
	/// @return �ύX�������ǂ���
	static bool ImGuiComboBox(Type& _curType);
};
