#pragma once

namespace HashiTaku
{
	/// @brief �A�j���[�V�����Ԃ̕�Ԏ��
	enum AnimInterpolateKind
	{
		CrossFade,	// �N���X�t�F�[�h
		Inertialization	// �������
	};

	/// @brief �L�[�Ԃ̕�Ԃ̎��
	enum KeyInterpolateKind
	{
		Lerp,	// ���`
		Bezier,	// �x�W�F
		Catmul	// Catmul
	};
}