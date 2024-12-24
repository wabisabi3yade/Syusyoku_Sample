#pragma once
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	/// @brief �p�[�����m�C�Y�ŃV�F�C�N����x�N�g�����v�Z����N���X
	class CalcPerlinShakeVector
	{
		/// @brief ���ݗh�炵�Ă���p�����[�^
		PerlinShakeParameter curShakeParameter;

		/// @brief �����Ō��߂��p�[�����m�C�Y�̃I�t�Z�b�g�l(����ς��)
		DXSimp::Vector3 randPerlinOffset;

		/// @brief �h�炵�Ă���̌o�ߎ���
		float shakeElapsedTime;

		/// @brief �h��Ă��邩�H
		bool isShaking;
	public:
		CalcPerlinShakeVector();
		~CalcPerlinShakeVector() = default;

		void CalcurateVector(float _deltaTime, DXSimp::Vector3& _outVector);

		/// @brief �h����J�n����
		/// @param _vector �h��̕���
		/// @param _power �h��̗�
		/// @param _time �h�ꎞ��(0�ȉ��͉i��)
		/// @param _speed �h�炷���x
		/// @param _isFadeOut �t�F�[�h�A�E�g�����Ă������H
		void BeginShake(const DXSimp::Vector3& _vector,
			float _power,
			float _time, 
			float _speed,
			bool _isFadeOut = true);

		/// @brief �h����J�n����
		/// @param _shakeParam �J�����h��p�����[�^
		void BeginShake(const PerlinShakeParameter& _shakeParam);

		/// @brief �V�F�C�N���I��点��
		void EndShake();

		/// @brief �h��Ă��邩�擾����
		/// @return �h��Ă���t���O
		bool GetIsShaking() const;
	private:
		/// @brief �h��̃I�t�Z�b�g�l���擾����
		/// @param _outVector ���߂��J�����̗h��x�N�g��
		void CalcShakeOffset(DXSimp::Vector3& _outVector);
	};
}


